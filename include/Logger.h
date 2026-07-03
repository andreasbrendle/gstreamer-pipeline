#pragma once

#include <string>

/**
 * @brief Simple logger for info and error messages.
 */
class Logger {
public:
    enum class Level {
        Info,
        Error
    };

    /**
     * @brief Logs a message with the given level.
     */
    static void log(Level level, const std::string& message);

    static void info(const std::string& message);
    static void error(const std::string& message);

    /**
     * @brief Enables or disables logging globally.
     *        Useful to suppress output during unit tests.
     */
    static void setEnabled(bool enabled);

private:
    Logger() = default;

    static bool s_enabled;
};