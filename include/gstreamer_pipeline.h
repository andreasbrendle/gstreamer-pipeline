#pragma once

#include <string>

/**
 * @brief Simple GStreamer pipeline class.
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
    // Pipeline and elements will be added here later
};