#pragma once

#include <opencv2/core.hpp>
#include <filesystem>
#include <string>
#include <vector>

/**
 * @brief Saves images to a target directory (single images or sequences).
 */
class FrameSaver {
public:
    /**
     * @brief Constructor with optional base folder.
     */
    explicit FrameSaver(std::filesystem::path baseFolder = "output/frames");

    /**
     * @brief Sets the base output folder.
     */
    void setBaseFolder(const std::filesystem::path& baseFolder);

    /**
     * @brief Saves a single image.
     * @param frame The image to save
     * @param suffix Suffix for the filename (e.g. "original" or "processed")
     * @param frameNumber Frame number (included in the filename)
     */
    void save(const cv::Mat& frame, const std::string& suffix, int frameNumber);

    /**
     * @brief Saves multiple images sequentially with incrementing frame numbers.
     */
    void save(const std::vector<cv::Mat>& frames, 
              const std::vector<std::string>& suffixes, 
              int frameNumber);

private:
    std::filesystem::path m_baseFolder;

    void ensureDirectoryExists();
    std::filesystem::path buildFilename(int frameNumber, const std::string& suffix) const;
};