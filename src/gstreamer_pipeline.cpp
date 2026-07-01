#include "gstreamer_pipeline.h"
#include <iostream>
#include <gst/gst.h>
#include <gst/app/gstappsink.h>

/* Pipeline elements */
static GstElement *pipeline     = nullptr;
static GstElement *source       = nullptr;
static GstElement *decodebin    = nullptr;
static GstElement *sink         = nullptr;

/**
 * @brief Callback for new pads created by decodebin.
 */
static void on_pad_added(GstElement *element, GstPad *pad, gpointer data) {
    GstElement *appsink = (GstElement *)data;
    GstPad *sinkpad = gst_element_get_static_pad(appsink, "sink");

    if (gst_pad_is_linked(sinkpad)) {
        gst_object_unref(sinkpad);
        return;
    }

    GstPadLinkReturn ret = gst_pad_link(pad, sinkpad);
    if (ret != GST_PAD_LINK_OK) {
        std::cerr << "Failed to link decodebin pad to appsink!" << std::endl;
    }

    gst_object_unref(sinkpad);
}

/**
 * @brief Initialize GStreamer and create a basic pipeline.
 */
bool GStreamerPipeline::init() {
    std::cout << "Initializing GStreamer pipeline with decodebin + appsink..." << std::endl;

    gst_init(nullptr, nullptr);

    // Create empty pipeline
    pipeline    = gst_pipeline_new("video-pipeline");
    // Create Gstreamer elements
    source      = gst_element_factory_make("filesrc", "source");
    decodebin   = gst_element_factory_make("decodebin", "decodebin");
    sink        = gst_element_factory_make("appsink", "sink");

    if (!pipeline || !source || !decodebin || !sink) {
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

    // Add elements to the pipeline
    gst_bin_add_many(GST_BIN(pipeline), source, decodebin, sink, NULL);

    // Link source to decodebin
    if (!gst_element_link(source, decodebin)) {
        std::cerr << "Failed to link source to decodebin!" << std::endl;
        return false;
    }
    // Connect to pad-added signal of decodebin
    g_signal_connect(decodebin, "pad-added", G_CALLBACK(on_pad_added), sink);

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

    std::cout << "Pipeline is running. Pulling frames from appsink..." << std::endl;

    // Simple loop to pull samples
    while (true) {
        /* Pull frames from appsink*/
        GstSample *sample = gst_app_sink_pull_sample(GST_APP_SINK(sink));
        if (!sample) {
            break; // No more samples (EOS or error)
        }

        gst_sample_unref(sample);
    }

    std::cout << "No more frames. Pipeline finished." << std::endl;
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