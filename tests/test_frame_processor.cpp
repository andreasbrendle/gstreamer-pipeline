#include <gtest/gtest.h>
#include "FrameProcessor.h"
#include <opencv2/opencv.hpp>

TEST(FrameProcessorTest, ProcessesImageInGrayscaleMode) {
    // Arrange
    const cv::Mat bgrImage(100, 100, CV_8UC3, cv::Scalar(255, 128, 64)); 

    FrameProcessor processor;

    // Act
    const cv::Mat grayImage = processor.process(bgrImage);

    // Assert
    EXPECT_EQ(grayImage.type(), CV_8UC1);           
    EXPECT_EQ(grayImage.channels(), 1);             
    EXPECT_EQ(grayImage.size(), bgrImage.size());   
}

TEST(FrameProcessorTest, ReturnsOriginalImageWhenModeIsNone) {
    FrameProcessor processor(FrameProcessor::ProcessingMode::None);
    const cv::Mat input(100, 100, CV_8UC3, cv::Scalar(50, 100, 150));

    const cv::Mat result = processor.process(input);

    EXPECT_EQ(result.type(), input.type());
    EXPECT_EQ(result.size(), input.size());
}