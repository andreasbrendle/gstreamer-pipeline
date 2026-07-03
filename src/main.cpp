#include "GStreamerPipeline.h"
#include "Logger.h"


int main() {
    Logger::info("=== GStreamer Video Processing Pipeline ===");

    GStreamerPipeline pipeline;

    const bool initialized = pipeline.init();
    if (!initialized) {
        Logger::error("Failed to initialize GStreamer pipeline!");
        return -1;
    }

    pipeline.run();
    pipeline.cleanup();

    Logger::info("Pipeline finished successfully.");
    return 0;
}