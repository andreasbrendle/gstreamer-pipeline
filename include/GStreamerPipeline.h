#pragma once

#include "FrameProcessor.h"
#include "FrameSaver.h"
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
 * Responsible for initializing, running and cleaning up a basic
 * GStreamer video pipeline that extracts and processes frames.
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
    void cleanup() const;
    
    /**
     * @brief Returns the number of processed frames.
     */
    int getFrameCount() const { return frameCount; }

private:
    PipelineData data;
    FrameProcessor frameProcessor;
    FrameSaver frameSaver;

    int frameCount = 0;

    /**
     * @brief Create all required GStreamer elements.
     *
     * @return true if all elements were created successfully.
     */
    bool createElements();

    /**
     * @brief Configure GStreamer elements (set properties and caps).
     *
     * @return true if configuration was successful.
     */
    bool configureElements() const;

    /**
     * @brief Link all GStreamer elements together and connect signals.
     *
     * @return true if linking was successful.
     */
    bool linkElements() const;

    /**
     * @brief Configure the output directory for the FrameSaver.
     */
    void setupFrameSaver();

    /**
     * @brief Main processing loop. Pulls samples from appsink until EOS.
     */
    void processSamples();

    /**
     * @brief Handles a single sample from the appsink.
     *
     * @param sample        The GStreamer sample to process
     * @param frameCount    Reference to the current frame counter
     * @param isFirstFrame  Reference to flag indicating the first frame
     */
    void handleSample(GstSample *sample, bool &isFirstFrame, int &frameCount);
    
    /**
     * @brief Processes a single BGR frame (conversion + optional saving).
     *
     * @param buffer   The GStreamer buffer containing the frame data
     * @param width    Width of the frame
     * @param height   Height of the frame
     * @param frameCount Reference to the current frame counter
     */
    void processBGRFrame(GstBuffer *buffer, gint width, gint height, int &frameCount);
};