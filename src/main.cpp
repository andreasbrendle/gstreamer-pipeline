#include <iostream>
#include "gstreamer_pipeline.h"

int main() {
    std::cout << "=== GStreamer C++ Pipeline ===" << '\n';

    GStreamerPipeline pipeline;

    if (!pipeline.init()) {
        std::cerr << "Failed to initialize GStreamer pipeline!" << '\n';
        return -1;
    }

    std::cout << "Pipeline initialized successfully." << '\n';

    // Run the pipeline (blocking call)
    pipeline.run();

    pipeline.cleanup();
    std::cout << "Pipeline finished." << '\n';

    return 0;
}