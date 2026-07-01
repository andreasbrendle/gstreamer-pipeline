#include "gstreamer_pipeline.h"
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
 * @brief Initialize GStreamer and create a basic pipeline.
 */
bool GStreamerPipeline::init() {
    std::cout << "Initializing GStreamer pipeline..." << '\n';

    gst_init(nullptr, nullptr);

    // Create empty pipeline
    data.pipeline    = gst_pipeline_new("video-pipeline");
    // Create Gstreamer elements
    data.source      = gst_element_factory_make("filesrc", "source");
    data.decodebin   = gst_element_factory_make("decodebin", "decodebin");
    data.convert     = gst_element_factory_make("videoconvert", "convert");
    data.sink        = gst_element_factory_make("appsink", "sink");

    if (!data.pipeline || !data.source || !data.decodebin || !data.convert || !data.sink) {
        std::cerr << "Failed to create GStreamer elements!" << '\n';
        return false;
    }

    auto projectRoot = resolveProjectRoot();
    auto videoPath = resolveVideoPath(projectRoot);

    if (!std::filesystem::exists(videoPath)) {
        std::cerr << "Video file not found at: " << videoPath << '\n';
        return false;
    }

    std::string videoLocation = videoPath.string();
    g_object_set(G_OBJECT(data.source), "location", videoLocation.c_str(), NULL);

    // Configure appsink
    // Enable signals (for later)
    // Don't sync to clock for now
    g_object_set(G_OBJECT(data.sink),
                 "emit-signals", TRUE,           
                 "sync", FALSE,                  
                 NULL);
    
    // Force output format to BGR to have compatibility with OpenCV
    GstCaps *caps = gst_caps_from_string("video/x-raw, format=BGR");
    g_object_set(G_OBJECT(data.sink), "caps", caps, NULL);
    gst_caps_unref(caps);

    // Add elements to the pipeline
    gst_bin_add_many(   GST_BIN(data.pipeline), 
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

    // Configure the generic frame saver to write into the project output folder.
    auto outputDir = projectRoot / "output" / "frames";
    std::filesystem::create_directories(outputDir);
    frameSaver.setBaseFolder(outputDir);
    std::cout << "Using project root: " << projectRoot << '\n';
    std::cout << "Using video file: " << videoPath << '\n';

    std::cout << "GStreamer pipeline created successfully." << '\n';
    return true;
}

/**
 * @brief Run the GStreamer pipeline.
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

    int frame_count = 0;
    bool first_frame = true;

    while (true) {
        /* Pull frames from appsink*/
        GstSample *sample = gst_app_sink_pull_sample(GST_APP_SINK(data.sink));
        if (!sample) {
            break; // End of stream
        }

        // Pull the buffer and caps from the sample
        GstBuffer *buffer   = gst_sample_get_buffer(sample);
        GstCaps *caps       = gst_sample_get_caps(sample);

        // Map the buffer to access the data
        if (buffer && caps) {
            GstStructure *structure = gst_caps_get_structure(caps, 0);
            const gchar *format = gst_structure_get_string(structure, "format");

            // Get image info from caps
            gint width  = 0;
            gint height = 0;
            gst_structure_get_int(structure, "width", &width);
            gst_structure_get_int(structure, "height", &height);

            if (first_frame) {
                std::cout << "Video info - Format: " << (format ? format : "unknown") 
                          << ", Size: " << width << "x" << height << '\n';
                first_frame = false;
            }
            // Only process if format is BGR
            if (format && strcmp(format, "BGR") == 0) {
                GstMapInfo map;
                if (gst_buffer_map(buffer, &map, GST_MAP_READ)) {
                    cv::Mat frame(height, width, CV_8UC3, (char*)map.data);

                    // Use FrameProcessor for image processing
                    cv::Mat processed = frameProcessor.process(frame);

                    frame_count++;
                    
                    if (frame_count % 30 == 0) {
                        frameSaver.save(frame, "original", frame_count);
                        frameSaver.save(processed, "processed", frame_count);
                    }

                    gst_buffer_unmap(buffer, &map);
                }
            } else {
                std::cout << "Skipping unsupported format: " << (format ? format : "unknown") << '\n';
            }
        }

        gst_sample_unref(sample);
    }

    std::cout << "Finished processing " << frame_count << " frames." << '\n';
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