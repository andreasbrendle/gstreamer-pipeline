#include "gstreamer_pipeline.h"
#include <iostream>
#include <filesystem>
#include <gst/gst.h>
#include <gst/app/gstappsink.h>
#include <opencv2/opencv.hpp>

/* Pipeline elements */
static GstElement *pipeline     = nullptr;
static GstElement *source       = nullptr;
static GstElement *decodebin    = nullptr;
static GstElement *convert      = nullptr;
static GstElement *sink         = nullptr;


std::filesystem::path getExecutableDir() {
    std::filesystem::path exePath = std::filesystem::read_symlink("/proc/self/exe");
    return exePath.parent_path();
}

/**
 * @brief Callback for dynamic pads created by decodebin.
 */
static void on_pad_added(GstElement *element, GstPad *pad, gpointer data) {
    GstElement *videoconvert = (GstElement *)data;
    
    GstPad *sinkpad = gst_element_get_static_pad(videoconvert, "sink");
    if (!sinkpad) return;

    if (gst_pad_is_linked(sinkpad)) {
        gst_object_unref(sinkpad);
        return;
    }

    GstPadLinkReturn ret = gst_pad_link(pad, sinkpad);
    if (ret != GST_PAD_LINK_OK) {
        std::cerr << "Failed to link decodebin pad!" << std::endl;
    }

    gst_object_unref(sinkpad);
}

/**
 * @brief Initialize GStreamer and create a basic pipeline.
 */
bool GStreamerPipeline::init() {
    std::cout << "Initializing GStreamer pipeline..." << std::endl;

    gst_init(nullptr, nullptr);

    // Create empty pipeline
    pipeline    = gst_pipeline_new("video-pipeline");
    // Create Gstreamer elements
    source      = gst_element_factory_make("filesrc", "source");
    decodebin   = gst_element_factory_make("decodebin", "decodebin");
    convert     = gst_element_factory_make("videoconvert", "convert");
    sink        = gst_element_factory_make("appsink", "sink");

    if (!pipeline || !source || !decodebin || !convert || !sink) {
        std::cerr << "Failed to create GStreamer elements!" << std::endl;
        return false;
    }

    // Set the video file location
    g_object_set(G_OBJECT(source), "location", "../test_video.mp4", NULL);

    // Configure appsink
    // Enable signals (for later)
    // Don't sync to clock for now
    g_object_set(G_OBJECT(sink),
                 "emit-signals", TRUE,           
                 "sync", FALSE,                  
                 NULL);
    
    // Force output format to BGR to have compatibility with OpenCV
    GstCaps *caps = gst_caps_from_string("video/x-raw, format=BGR");
    g_object_set(G_OBJECT(sink), "caps", caps, NULL);
    gst_caps_unref(caps);

    // Add elements to the pipeline
    gst_bin_add_many(   GST_BIN(pipeline), 
                        source, 
                        decodebin, 
                        convert, 
                        sink, 
                        NULL);

    // Link source to decodebin
    if (!gst_element_link(source, decodebin)) {
        std::cerr << "Failed to link source to decodebin!" << std::endl;
        return false;
    }

    if (!gst_element_link(convert, sink)) {
        std::cerr << "Failed to link convert to sink!" << std::endl;
        return false;
    }

    // Connect to pad-added signal of decodebin
    g_signal_connect(decodebin, "pad-added", G_CALLBACK(on_pad_added), convert);

    // Create output directory for frames (in project root, same level as setup.sh and run.sh)
    auto projectRoot = getExecutableDir().parent_path();
    std::filesystem::create_directories(projectRoot / "output" / "frames");

    std::cout << "GStreamer pipeline created successfully." << std::endl;
    return true;
}

/**
 * @brief Run the GStreamer pipeline.
 */
void GStreamerPipeline::run() {
    std::cout << "Starting pipeline..." << std::endl;

    GstBus *bus = gst_element_get_bus(pipeline);

    GstStateChangeReturn ret = gst_element_set_state(pipeline, GST_STATE_PLAYING);
    if (ret == GST_STATE_CHANGE_FAILURE) {
        std::cerr << "Failed to start pipeline!" << std::endl;
        gst_object_unref(bus);
        return;
    }

    std::cout << "Pipeline is running. Processing frames..." << std::endl;

    int frame_count = 0;
    bool first_frame = true;

    while (true) {
        /* Pull frames from appsink*/
        GstSample *sample = gst_app_sink_pull_sample(GST_APP_SINK(sink));
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
            gint width, height;                
            gst_structure_get_int(structure, "width", &width);
            gst_structure_get_int(structure, "height", &height);

            if (first_frame) {
                std::cout << "Video info - Format: " << (format ? format : "unknown") 
                          << ", Size: " << width << "x" << height << std::endl;
                first_frame = false;
            }
            // Only process if format is BGR
            if (format && strcmp(format, "BGR") == 0) {
                GstMapInfo map;
                if (gst_buffer_map(buffer, &map, GST_MAP_READ)) {
                    cv::Mat frame(height, width, CV_8UC3, (char*)map.data);

                    // Convert to grayscale
                    cv::Mat gray;
                    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

                    frame_count++;
                    if (frame_count % 30 == 0) {
                        auto projectRoot = getExecutableDir().parent_path();
                        std::string filenameOriginal = (projectRoot / "output" / "frames" / ("frame_" + std::to_string(frame_count) + "_original.png")).string();
                        std::string filenameGray    = (projectRoot / "output" / "frames" / ("frame_" + std::to_string(frame_count) + "_gray.png")).string();
                        cv::imwrite(filenameOriginal, frame);
                        cv::imwrite(filenameGray, gray);

                        std::cout << "Processed frame " << frame_count 
                                  << " | Size: " << width << "x" << height 
                                  << " | Format: " << format << std::endl;
                    }

                    gst_buffer_unmap(buffer, &map);
                }
            } else {
                std::cout << "Skipping unsupported format: " << (format ? format : "unknown") << std::endl;
            }
        }

        gst_sample_unref(sample);
    }

    std::cout << "Finished processing " << frame_count << " frames." << std::endl;
}

/**
 * @brief Clean up GStreamer resources.
 */
void GStreamerPipeline::cleanup() {
    std::cout << "Cleaning up..." << std::endl;

    if (pipeline) {
        gst_element_set_state(pipeline, GST_STATE_NULL);
        gst_object_unref(pipeline);
    }
}