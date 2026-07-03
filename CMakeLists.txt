cmake_minimum_required(VERSION 3.16)
project(gstreamer_cpp_pipeline LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ====================== Clang-Tidy ======================
find_program(CLANG_TIDY_EXE NAMES clang-tidy)
if(CLANG_TIDY_EXE)
    # Simply call clang-tidy. It will automatically find and use your `.clang-tidy` file!
    set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")
    message(STATUS "Clang-Tidy enabled globally via .clang-tidy file")
endif()

# ====================== Google Test ======================
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# ====================== GStreamer ======================
find_package(PkgConfig REQUIRED)
pkg_check_modules(GST REQUIRED gstreamer-1.0 gstreamer-video-1.0 gstreamer-app-1.0)

# ====================== OpenCV ======================
find_package(OpenCV REQUIRED)

# ====================== Includes ======================
include_directories(
    ${GST_INCLUDE_DIRS}
    ${OpenCV_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/include
)

# ====================== Library ======================
add_library(pipeline_lib
    src/FrameProcessor.cpp
    src/FrameSaver.cpp
    src/GStreamerPipeline.cpp
    src/Logger.cpp
)

target_include_directories(pipeline_lib PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(pipeline_lib ${OpenCV_LIBS} ${GST_LIBRARIES})

# ====================== Main Executable ======================
add_executable(gstreamer_pipeline
    src/main.cpp
)

target_link_libraries(gstreamer_pipeline
    pipeline_lib
    ${GST_LIBRARIES}
)

target_compile_options(gstreamer_pipeline PRIVATE -O3 -Wall)

# ====================== Tests ======================
enable_testing()

add_executable(pipeline_tests
    tests/test_frame_processor.cpp
    tests/test_frame_saver.cpp
    tests/test_gstreamer_pipeline_functional.cpp
    tests/test_pipeline_functional.cpp
    tests/test_environment.cpp
    src/Logger.cpp

)

target_link_libraries(pipeline_tests
    pipeline_lib
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(pipeline_tests)

# ====================== Disable Clang-Tidy for Third-Party ======================
# Moving this to the very bottom ensures the GoogleTest targets actually exist before modifying them
if(CLANG_TIDY_EXE)
    set_target_properties(gtest gtest_main gmock gmock_main PROPERTIES CXX_CLANG_TIDY "")
endif()
