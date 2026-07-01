#pragma once

#include <opencv2/opencv.hpp>

/**
 * @brief Available processing modes for image frames.
 */
enum class ProcessingMode {
    Grayscale,
    // Add more modes later, e.g.:
    // CLAHE,
    // Denoise
};

/**
 * @brief Responsible for processing image frames (color conversion, filtering, etc.).
 */
class FrameProcessor {
public:
    /**
     * @brief Constructor with optional processing mode.
     */
    explicit FrameProcessor(ProcessingMode mode = ProcessingMode::Grayscale);

    /**
     * @brief Processes the input frame according to the current mode.
     * 
     * @param input Input frame (BGR expected)
     * @return Processed frame
     */
    cv::Mat process(const cv::Mat& input);

    /**
     * @brief Changes the current processing mode at runtime.
     */
    void setMode(ProcessingMode mode);

private:
    ProcessingMode m_mode;

    cv::Mat convertToGrayscale(const cv::Mat& input);
    // Future methods:
    // cv::Mat applyCLAHE(const cv::Mat& input);
    // cv::Mat applyDenoising(const cv::Mat& input);
};