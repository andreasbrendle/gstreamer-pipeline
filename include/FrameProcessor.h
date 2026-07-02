#pragma once

#include <cstdint>

//OpenCV
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>

/**
 * @brief Processes images (e.g. conversion to grayscale).
 */
class FrameProcessor {
public:
    /**
    * @brief Available processing modes for image frames.
    */
    enum class ProcessingMode : std::uint8_t {
        None,
        Grayscale,
        // Add more modes later, e.g.:
        // CLAHE,
        // Denoise
    };

    /**
     * @brief Constructor with optional processing mode.
     */
    explicit FrameProcessor(ProcessingMode mode = ProcessingMode::Grayscale);

    /**
     * @brief Processes the input frame according to the current mode.
     * 
     * @param input Input image in BGR format
     * @return Processed frame
     */
    cv::Mat process(const cv::Mat& input);

    /**
     * @brief Changes the current processing mode at runtime.
     */
    void setMode(ProcessingMode mode);

private:
    ProcessingMode m_mode;

    static cv::Mat convertToGrayscale(const cv::Mat& input);
    // Future methods:
    // cv::Mat applyCLAHE(const cv::Mat& input);
    // cv::Mat applyDenoising(const cv::Mat& input);
};