#pragma once

#include <string>

#include <gst/gst.h>

/**
 * @brief Holds all GStreamer pipeline elements.
 */
struct PipelineData {
    GstElement *pipeline = nullptr;
    GstElement *source = nullptr;
    GstElement *decodebin = nullptr;
    GstElement *convert = nullptr;
    GstElement *sink = nullptr;
};

/**
 * @brief GStreamer pipeline class.
 * 
 * This class is responsible for initializing and running 
 * a basic GStreamer pipeline that reads from a video source 
 * and pushes frames to an appsink.
 */
class GStreamerPipeline {
public:
    /**
     * @brief Initialize the GStreamer pipeline.
     * 
     * @return true if initialization was successful, false otherwise.
     */
    bool init();

    /**
     * @brief Start and run the pipeline.
     */
    void run();

    /**
     * @brief Clean up resources.
     */
    void cleanup();

private:
    PipelineData data; 
};