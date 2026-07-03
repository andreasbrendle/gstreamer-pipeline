#include <gtest/gtest.h>
#include "Logger.h"

/**
 * @brief Global test environment that disables logging during all unit and functional tests.
 * 
 * This prevents unwanted console output (e.g. "Saved: ...") from appearing in the test results.
 */
class DisableLoggingEnvironment : public ::testing::Environment {
public:
    void SetUp() override {
        Logger::setEnabled(false);
    }
};

// Register the environment globally. It will be initialized before any tests run.
static const auto* const disable_logging_env = 
    ::testing::AddGlobalTestEnvironment(new DisableLoggingEnvironment());