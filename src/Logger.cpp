#include "Logger.h"
#include <iostream>

bool Logger::s_enabled = true;

void Logger::log(Level level, const std::string& message) {
    if (!s_enabled) {
        return;
    }

    if (level == Level::Info) {
        std::cout << "[INFO]  " << message << '\n';
    } else if (level == Level::Error) {
        std::cerr << "[ERROR] " << message << '\n';
    }
}

void Logger::info(const std::string& message) {
    log(Level::Info, message);
}

void Logger::error(const std::string& message) {
    log(Level::Error, message);
}

void Logger::setEnabled(bool enabled) {
    s_enabled = enabled;
}