cmake_minimum_required(VERSION 3.16)
project(gstreamer_cpp_pipeline LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# === GStreamer finden ===
find_package(PkgConfig REQUIRED)
pkg_check_modules(GST REQUIRED gstreamer-1.0 gstreamer-video-1.0 gstreamer-app-1.0)

# === OpenCV finden ===
find_package(OpenCV REQUIRED)

# === Include-Verzeichnisse ===
include_directories(
    ${GST_INCLUDE_DIRS}
    ${OpenCV_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/include
)

# === Executable erstellen ===
add_executable(gstreamer_pipeline
    src/main.cpp
    src/gstreamer_pipeline.cpp
)

# === Bibliotheken verlinken ===
target_link_libraries(gstreamer_pipeline
    ${GST_LIBRARIES}
    ${OpenCV_LIBS}
)

# === Compiler Flags ===
target_compile_options(gstreamer_pipeline PRIVATE -O3 -Wall)