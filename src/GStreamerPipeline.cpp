#include "GStreamerPipeline.h"
#include "FrameProcessor.h"
#include "FrameSaver.h"
#include <iostream>
#include <filesystem>
#include <gst/gst.h>
#include <gst/app/gstappsink.h>
#include <opencv2/opencv.hpp>
#include <cstdlib>

namespace {
std::filesystem::path resolveProjectRoot() {
    const char *appRootEnv = std::getenv("APP_ROOT");
    if (appRootEnv && *appRootEnv) {
        return std::filesystem::absolute(appRootEnv);
    }

    std::filesystem::path exePath = std::filesystem::read_symlink("/proc/self/exe");
    std::filesystem::path exeDir = exePath.parent_path();
    if (exeDir.filename() == "build" || exeDir.filename() == "bin") {
        return exeDir.parent_path();
    }
    return exeDir;
}

std::filesystem::path resolveVideoPath(const std::filesystem::path &projectRoot) {
    return projectRoot / "test_video.mp4";
}
} // namespace

/**
 * @brief Callback for dynamic pads created by decodebin.
 */
static void on_pad_added(GstElement *element, GstPad *pad, gpointer data) {
    GstElement *convert = GST_ELEMENT(data);

    GstPad *sinkpad = gst_element_get_static_pad(convert, "sink");
    if (!sinkpad) return;

    if (gst_pad_is_linked(sinkpad)) {
        gst_object_unref(sinkpad);
        return;
    }

    GstPadLinkReturn ret = gst_pad_link(pad, sinkpad);
    if (ret != GST_PAD_LINK_OK) {
        std::cerr << "Failed to link decodebin pad!" << '\n';
    }

    gst_object_unref(sinkpad);
}

/**
 * @brief Initialize GStreamer and create the pipeline.
 */
bool GStreamerPipeline::init() {
    std::cout << "Initializing GStreamer pipeline..." << '\n';

    gst_init(nullptr, nullptr);

    if (!createElements()) {
        return false;
    }

    if (!configureElements()) {
        return false;
    }

    if (!linkElements()) {
        return false;
    }

    setupFrameSaver();

    std::cout << "GStreamer pipeline created successfully." << '\n';
    return true;
}

/**
 * @brief Create all required GStreamer elements.
 *
 * @return true if all elements were created successfully, false otherwise.
 */
bool GStreamerPipeline::createElements() {
    // Create empty pipeline
    data.pipeline    = gst_pipeline_new("video-pipeline");

    // Create Gstreamer elements
    data.source      = gst_element_factory_make("filesrc", "source");
    data.decodebin   = gst_element_factory_make("decodebin", "decodebin");
    data.convert     = gst_element_factory_make("videoconvert", "convert");
    data.sink        = gst_element_factory_make("appsink", "sink");

    if (    data.pipeline == nullptr
        ||  data.source == nullptr
        ||  data.decodebin == nullptr
        ||  data.convert == nullptr
        ||  data.sink == nullptr) {
        std::cerr << "Failed to create GStreamer elements!" << '\n';
        return false;
    }
}

/**
 * @brief Configure GStreamer elements (set properties, caps, etc.).
 *
 * @return true if configuration was successful, false otherwise.
 */
bool GStreamerPipeline::configureElements() {
    auto projectRoot    = resolveProjectRoot();
    auto videoPath      = resolveVideoPath(projectRoot);

    if (!std::filesystem::exists(videoPath)) {
        std::cerr << "Video file not found at: " << videoPath << '\n';
        return false;
    }

    g_object_set(G_OBJECT(data.source), "location", videoPath.string().c_str(), NULL);

    // Configure appsink
    g_object_set(G_OBJECT(data.sink),
                 "emit-signals", TRUE,           
                 "sync", FALSE,                  
                 NULL);
    
    // Force output format to BGR to have compatibility with OpenCV
    GstCaps *caps = gst_caps_from_string("video/x-raw, format=BGR");
    g_object_set(G_OBJECT(data.sink), "caps", caps, NULL);
    gst_caps_unref(caps);
    return true;
}

/**
 * @brief Link all GStreamer elements and connect signals.
 *
 * @return true if linking was successful, false otherwise.
 */
bool GStreamerPipeline::linkElements() {
    // Add elements to the pipeline
    gst_bin_add_many(GST_BIN(data.pipeline), 
                     data.source, 
                     data.decodebin, 
                     data.convert, 
                     data.sink, 
                     NULL);

    // Link source to decodebin
    if (!gst_element_link(data.source, data.decodebin)) {
        std::cerr << "Failed to link source to decodebin!" << '\n';
        return false;
    }

    if (!gst_element_link(data.convert, data.sink)) {
        std::cerr << "Failed to link convert to sink!" << '\n';
        return false;
    }

    // Connect to pad-added signal of decodebin
    g_signal_connect(data.decodebin, "pad-added", G_CALLBACK(on_pad_added), data.convert);
    return true;
}

/**
 * @brief Configure the output directory for the FrameSaver.
 */
void GStreamerPipeline::setupFrameSaver() {
    auto projectRoot    = resolveProjectRoot();
    auto outputDir      = projectRoot / "output" / "frames";

    std::filesystem::create_directories(outputDir);
    frameSaver.setBaseFolder(outputDir);

    std::cout << "Using project root: " << projectRoot << '\n';
}

/**
 * @brief Start the pipeline and run the main processing loop.
 */
void GStreamerPipeline::run() {
    std::cout << "Starting pipeline..." << '\n';

    GstBus *bus = gst_element_get_bus(data.pipeline);

    GstStateChangeReturn ret = gst_element_set_state(data.pipeline, GST_STATE_PLAYING);
    if (ret == GST_STATE_CHANGE_FAILURE) {
        std::cerr << "Failed to start pipeline!" << '\n';
        gst_object_unref(bus);
        return;
    }

    std::cout << "Pipeline is running. Processing frames..." << '\n';

    processSamples();

    std::cout << "Finished processing " << frameCount << " frames." << '\n';
}

/**
 * @brief Main processing loop. Pulls samples from appsink until end of stream.
 */
void GStreamerPipeline::processSamples() {
    int  localFrameCount = 0;
    bool isFirstFrame    = true;

    while (true) {
        /* Pull frames from appsink*/
        GstSample *sample = gst_app_sink_pull_sample(GST_APP_SINK(data.sink));
        if (!sample) {
            break; // End of stream reached
        }

        handleSample(sample, localFrameCount, isFirstFrame);
        gst_sample_unref(sample);
    }
    frameCount = localFrameCount; // store final count
}

/**
 * @brief Handles a single sample from the appsink.
 */
void GStreamerPipeline::handleSample(GstSample *sample, int &frameCount, bool &isFirstFrame) {
    // Pull the buffer and caps from the sample
    GstBuffer *buffer   = gst_sample_get_buffer(sample);
    GstCaps *caps       = gst_sample_get_caps(sample);

    if (!buffer || !caps) return;

    GstStructure *structure = gst_caps_get_structure(caps, 0);
    const gchar  *format    = gst_structure_get_string(structure, "format");

    // Get image info from caps
    gint width  = 0;
    gint height = 0;
    gst_structure_get_int(structure, "width", &width);
    gst_structure_get_int(structure, "height", &height);

    if (isFirstFrame) {
        std::cout << "Video info - Format: " << (format ? format : "unknown") 
                  << ", Size: " << width << "x" << height << '\n';
        isFirstFrame = false;
    }

    if (format && strcmp(format, "BGR") == 0) {
        processBGRFrame(buffer, width, height, frameCount);
    } else {
        std::cout << "Skipping unsupported format: " << (format ? format : "unknown") << '\n';
    }
}

/**
 * @brief Processes a single BGR frame (convert, process, optionally save).
 */
void GStreamerPipeline::processBGRFrame(GstBuffer *buffer, gint width, gint height, int &frameCount) {

    GstMapInfo map;
    if (!gst_buffer_map(buffer, &map, GST_MAP_READ)) return;

    cv::Mat frame(height, width, CV_8UC3, (char*)map.data);

    // Use FrameProcessor for image processing
    cv::Mat processed = frameProcessor.process(frame);

    frameCount++;
                    
    if (frameCount % 30 == 0) {
        frameSaver.save(frame, "original", frameCount);
        frameSaver.save(processed, "processed", frameCount);
    }

    gst_buffer_unmap(buffer, &map);
}

/**
 * @brief Clean up GStreamer resources.
 */
void GStreamerPipeline::cleanup() {
    std::cout << "Cleaning up..." << '\n';

    if (data.pipeline) {
        gst_element_set_state(data.pipeline, GST_STATE_NULL);
        gst_object_unref(data.pipeline);
    }
}