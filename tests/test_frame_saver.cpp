#include <gtest/gtest.h>
#include "FrameSaver.h"
#include <opencv2/opencv.hpp>
#include <filesystem>

TEST(FrameSaverTest, SavesFileWithCorrectName) {
    // Arrange
    const cv::Mat testImage(50, 50, CV_8UC3, cv::Scalar(100, 150, 200));
    const std::string tempDir = testing::TempDir() + "frame_saver_test/";
    std::filesystem::create_directories(tempDir);

    FrameSaver saver(tempDir);

    // Act
    saver.save(testImage, "test", 42);  

    // Assert
    const std::string expectedFile = tempDir + "frame_42_test.png";
    EXPECT_TRUE(std::filesystem::exists(expectedFile));

    // Cleanup
    std::filesystem::remove(expectedFile);
    std::filesystem::remove(tempDir);
}