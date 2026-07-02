#include <gtest/gtest.h>
#include "FrameProcessor.h"
#include "FrameSaver.h"
#include <opencv2/opencv.hpp>
#include <filesystem>

TEST(PipelineFunctionalTest, ProcessesAndSavesFrames) {
    // Arrange
    cv::Mat testFrame(100, 100, CV_8UC3, cv::Scalar(50, 100, 150));
    std::string tempDir = testing::TempDir() + "pipeline_functional_test/";
    std::filesystem::create_directories(tempDir);

    FrameProcessor processor;
    FrameSaver saver(tempDir);

    int savedCount = 0;

    // Act - simuliere 100 Frames, speichere jeden 30. Frame
    for (int i = 1; i <= 100; ++i) {
        cv::Mat processed = processor.process(testFrame);

        if (i % 30 == 0) {
            saver.save(testFrame, "original", i);
            saver.save(processed, "processed", i);
            savedCount++;
        }
    }

    // Assert
    EXPECT_EQ(savedCount, 3); // Bei 100 Frames → Frames 30, 60, 90

    // Optional: Prüfen, ob Dateien wirklich existieren
    EXPECT_TRUE(std::filesystem::exists(tempDir + "frame_30_original.png"));
    EXPECT_TRUE(std::filesystem::exists(tempDir + "frame_60_processed.png"));

    // Cleanup
    std::filesystem::remove_all(tempDir);
}