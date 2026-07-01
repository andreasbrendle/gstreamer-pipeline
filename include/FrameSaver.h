#pragma once

#include <opencv2/opencv.hpp>
#include <filesystem>
#include <string>
#include <vector>

/**
 * @brief Generic class for saving image frames to disk.
 */
class FrameSaver {
public:
    /**
     * @brief Constructor.
     */
    explicit FrameSaver(const std::filesystem::path& baseFolder = "output/frames");

    /**
     * @brief Sets the base output folder.
     */
    void setBaseFolder(const std::filesystem::path& baseFolder);

    /**
     * @brief Saves a single frame with a custom suffix.
     * 
     * Example: save(frame, "original", 30) → frame_30_original.png
     */
    void save(const cv::Mat& frame, const std::string& suffix, int frameNumber);

    /**
     * @brief Saves multiple frames at once with corresponding suffixes.
     */
    void save(const std::vector<cv::Mat>& frames, 
              const std::vector<std::string>& suffixes, 
              int frameNumber);

private:
    std::filesystem::path m_baseFolder;

    void ensureDirectoryExists();
    std::filesystem::path buildFilename(int frameNumber, const std::string& suffix) const;
};