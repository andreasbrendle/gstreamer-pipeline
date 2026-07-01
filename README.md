# GStreamer Video Processing Pipeline

A small example project for processing video data using **GStreamer** and **OpenCV**.

The project demonstrates how to build a video pipeline with GStreamer, extract frames, and process them using OpenCV. It serves as a practical introduction to real-time image processing and embedded video pipelines.

## Project Goal

The goal of this project was to gain practical experience with GStreamer by building a working pipeline that:

- Reads a video file
- Processes the frames using OpenCV (e.g. grayscale conversion)
- Saves every Nth frame (both original and processed version) to disk

This project is intended as a **learning and demonstration example** to show basic competence in working with GStreamer and OpenCV.

## Features

- GStreamer pipeline using `decodebin` and hardware-accelerated conversion
- Frame processing using OpenCV (`FrameProcessor`)
- Saving of original and processed frames (`FrameSaver`)
- Docker support for easy reproducibility
- Clean separation between pipeline logic, image processing and file output

## Technologies Used

- **GStreamer** (1.24)
- **OpenCV** (4.6+)
- **C++17**
- **CMake**
- **Docker**

## Project Structure

```
gstreamer-pipeline/
├── .vscode/                    # VS Code settings
├── docker/                     # Docker configuration
├── include/                    # Header files
├── src/                        # Source code
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

- Basic GStreamer pipeline with frame extraction is working
- Image processing (grayscale conversion) is handled in a separate `FrameProcessor` class
- Saving of original and processed frames is implemented via `FrameSaver`
- Docker setup is fully functional
- Initial code quality tooling (Clang-Tidy) has been introduced

## Known Limitations

- Currently only supports video files as input (camera support is partially prepared)
- The pipeline is not yet optimized for real-time or high-performance use cases

## Future Ideas

- Add more image processing algorithms (e.g. CLAHE, denoising)
- Implement unit and functional tests
- Add support for live camera streams
