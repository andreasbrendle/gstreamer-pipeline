# GStreamer Video Processing Pipeline

A small demonstration project built to gain practical experience with **GStreamer** and **OpenCV**.  
The goal was to build a working video pipeline that reads a video, processes frames, and saves selected frames to disk.

This project serves as a learning example to demonstrate basic competence in building multimedia pipelines with GStreamer.

## Project Goal

- Learn how to build and control a GStreamer pipeline in C++
- Extract and process video frames using OpenCV
- Implement clean separation between pipeline logic, image processing, and file output
- Explore Docker as a way to make the project easily reproducible

## Features

- GStreamer pipeline with `decodebin` and hardware-accelerated video conversion
- Frame processing handled in a dedicated `FrameProcessor` class
- Saving of both original and processed frames using a `FrameSaver` class
- Docker support for consistent execution across environments
- Basic code quality setup using Clang-Tidy

## Technologies Used

- **GStreamer** (1.24)
- **OpenCV** (4.6+)
- **C++17**
- **CMake**
- **Docker**

## Project Structure

```text
gstreamer-pipeline/
├── .vscode/                    # VS Code settings
├── docker/                     # Docker configuration
├── include/                    # Header files
├── src/                        # Source code
│   ├── main.cpp
│   ├── GStreamerPipeline.cpp
│   ├── FrameProcessor.cpp
│   └── FrameSaver.cpp
├── .clang-tidy                 # Clang-Tidy configuration
├── .dockerignore
├── .gitignore
├── CMakeLists.txt
├── README.md
├── run.sh
├── setup.sh
└── test_video.mp4              # Test video file
```

## Requirements

- Ubuntu 22.04 / 24.04 (or equivalent)
- GStreamer and OpenCV development packages
- CMake 3.16 or higher
- C++17 compatible compiler

## Build & Run

### Local Build (recommended for development)

```bash
# Install dependencies
./setup.sh

# Build and run the project
./run.sh
```

### Using Docker

The project includes full Docker support for easy and consistent execution:

```bash
cd docker
docker compose up --build
```

Processed frames will be saved to the `output/frames/` directory on your host machine.

**Advantages of using Docker:**

- No need to install GStreamer or OpenCV locally
- Reproducible environment
- Easy to share and run on other machines

## Current Status

- Functional GStreamer pipeline with frame extraction
- Image processing and frame saving logic separated into dedicated classes
- Docker environment fully working
- Initial static code analysis with Clang-Tidy in place

## Known Limitations

- Currently only supports video files as input
- No real-time optimizations implemented yet
- Testing coverage is still limited

## Future Ideas

- Add more image processing algorithms (e.g. CLAHE, denoising)
- Implement unit and functional tests
- Add support for live camera streams
