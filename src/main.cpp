#include <iostream>
#include "GStreamerPipeline.h"

int main() {
    std::cout << "=== GStreamer Video Processing Pipeline ===" << '\n';

    GStreamerPipeline pipeline;

    bool initialized = pipeline.init();
    if (!initialized) {
        std::cerr << "Failed to initialize GStreamer pipeline!" << '\n';
        return -1;
    }

    pipeline.run();
    pipeline.cleanup();

    std::cout << "Pipeline finished successfully." << '\n';
    return 0;
}