#include "FrameProcessor.h"

// OpenCV
#include <opencv2/core.hpp>     
#include <opencv2/imgproc.hpp>

FrameProcessor::FrameProcessor(ProcessingMode mode)
    : m_mode(mode) {}

void FrameProcessor::setMode(ProcessingMode mode) {
    m_mode = mode;
}

cv::Mat FrameProcessor::process(const cv::Mat& input) {
    if (input.empty()) {
        return cv::Mat{};
    }

    switch (m_mode) {
        case ProcessingMode::Grayscale:
            return convertToGrayscale(input);
        // case ProcessingMode::CLAHE:
        //     return applyCLAHE(input);
        default:
            return input; // fallback: return original
    }
}

/**
 * @brief Converts a BGR image to grayscale.
 */
cv::Mat FrameProcessor::convertToGrayscale(const cv::Mat& input) {
    cv::Mat output;
    cv::cvtColor(input, output, cv::COLOR_BGR2GRAY);
    return output;
}