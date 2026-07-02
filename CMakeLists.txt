cmake_minimum_required(VERSION 3.16)
project(gstreamer_cpp_pipeline LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# === Clang-Tidy aktivieren ===
find_program(CLANG_TIDY_EXE NAMES clang-tidy)
if(CLANG_TIDY_EXE)
    set(CMAKE_CXX_CLANG_TIDY
        "${CLANG_TIDY_EXE}"
        "-header-filter=.*"
    )
    message(STATUS "Clang-Tidy enabled")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# === GStreamer finden ===
find_package(PkgConfig REQUIRED)
pkg_check_modules(GST REQUIRED IMPORTED_TARGET gstreamer-1.0 gstreamer-video-1.0 gstreamer-app-1.0)

# === OpenCV finden ===
find_package(OpenCV REQUIRED COMPONENTS core imgproc imgcodecs)

# === Executable erstellen ===
add_executable(GStreamerPipeline
    src/main.cpp
    src/GStreamerPipeline.cpp
    src/FrameProcessor.cpp
    src/FrameSaver.cpp
)

target_include_directories(GStreamerPipeline PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

# === Bibliotheken verlinken ===
target_link_libraries(GStreamerPipeline PRIVATE
    PkgConfig::GST
    ${OpenCV_LIBS}
)

# === Compiler Flags ===
target_compile_options(GStreamerPipeline PRIVATE -O3 -Wall)