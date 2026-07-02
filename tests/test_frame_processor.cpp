#include <gtest/gtest.h>
#include "FrameProcessor.h"
#include <opencv2/opencv.hpp>

TEST(FrameProcessorTest, ConvertsBGRToGrayscale) {
    // Arrange
    cv::Mat bgrImage(100, 100, CV_8UC3, cv::Scalar(255, 128, 64)); 

    FrameProcessor processor;

    // Act
    cv::Mat grayImage = processor.process(bgrImage);

    // Assert
    EXPECT_EQ(grayImage.type(), CV_8UC1);           
    EXPECT_EQ(grayImage.channels(), 1);             
    EXPECT_EQ(grayImage.size(), bgrImage.size());   
}