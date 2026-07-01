#include <iostream>
#include "gstreamer_pipeline.h"

int main() {
    std::cout << "=== GStreamer C++ Pipeline ===" << std::endl;

    GStreamerPipeline pipeline;

    if (!pipeline.init()) {
        std::cerr << "Failed to initialize GStreamer pipeline!" << std::endl;
        return -1;
    }

    std::cout << "Pipeline initialized successfully." << std::endl;

    // Run the pipeline (blocking call)
    pipeline.run();

    pipeline.cleanup();
    std::cout << "Pipeline finished." << std::endl;

    return 0;
}