#include "FrameSaver.h"

//OpenCV
#include <opencv2/imgcodecs.hpp> // for cv::imwrite  

#include <utility> 
#include <iostream> 
#include <filesystem>
#include <string>

FrameSaver::FrameSaver(std::filesystem::path baseFolder)
    : m_baseFolder(std::move(baseFolder))
{
    ensureDirectoryExists();
}

void FrameSaver::setBaseFolder(const std::filesystem::path& baseFolder) {
    m_baseFolder = baseFolder;
    ensureDirectoryExists();
}

bool FrameSaver::save(const cv::Mat& frame, const std::string& suffix, int frameNumber) {
    if (frame.empty()) {
        return false;
    }

    const std::filesystem::path filename = buildFilename(frameNumber, suffix);

    if (cv::imwrite(filename.string(), frame)) {
        std::cout << "Saved: " << filename << '\n';
        return true;
    }
    
    std::cerr << "Failed to save: " << filename << '\n';
    return false;
}

/**
 * @brief Generates a filename in the format: frame_000042_original.png
 */
std::filesystem::path FrameSaver::buildFilename(int frameNumber, const std::string& suffix) const {
    return m_baseFolder / ("frame_" + std::to_string(frameNumber) + "_" + suffix + ".png");
}

void FrameSaver::ensureDirectoryExists() {
    try {
        std::filesystem::create_directories(m_baseFolder);
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Failed to create directory: " << e.what() << '\n';
    }
}