#include "FrameSaver.h"
#include <filesystem>
#include <iostream>

FrameSaver::FrameSaver(const std::filesystem::path& baseFolder)
    : m_baseFolder(baseFolder)
{
    ensureDirectoryExists();
}

void FrameSaver::setBaseFolder(const std::filesystem::path& baseFolder) {
    m_baseFolder = baseFolder;
    ensureDirectoryExists();
}

void FrameSaver::save(const cv::Mat& frame, const std::string& suffix, int frameNumber) {
    if (frame.empty()) {
        return;
    }

    std::filesystem::path filename = buildFilename(frameNumber, suffix);

    if (cv::imwrite(filename.string(), frame)) {
        std::cout << "Saved: " << filename << '\n';
    } else {
        std::cerr << "Failed to save: " << filename << '\n';
    }
}

void FrameSaver::save(const std::vector<cv::Mat>& frames,
                      const std::vector<std::string>& suffixes,
                      int frameNumber)
{
    if (frames.size() != suffixes.size()) {
        std::cerr << "Error: Number of frames and suffixes must match!" << '\n';
        return;
    }

    for (size_t i = 0; i < frames.size(); ++i) {
        save(frames[i], suffixes[i], frameNumber);
    }
}

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