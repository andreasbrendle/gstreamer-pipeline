#include <gtest/gtest.h>
#include "GStreamerPipeline.h"

TEST(GStreamerPipelineFunctionalTest, InitializesSuccessfully) {
    GStreamerPipeline pipeline;

    bool result = pipeline.init();

    EXPECT_TRUE(result);
}

TEST(GStreamerPipelineFunctionalTest, ProcessesFrames) {
    GStreamerPipeline pipeline;

    ASSERT_TRUE(pipeline.init());

    pipeline.run();
    pipeline.cleanup();

    EXPECT_GT(pipeline.getFrameCount(), 0); // Es sollten Frames verarbeitet worden sein
}