# GStreamer Video Processing Pipeline

A small demonstration project created to gain practical experience with **GStreamer** in C++.  
The main goal was to build and understand a real video processing pipeline, including frame extraction and integration with OpenCV.

This project was developed to show my interest in multimedia technologies and my ability to work with GStreamer in a professional environment.

## Motivation

While I already have solid experience with modern C++ and software engineering practices, I wanted to gain practical, hands-on experience with GStreamer. In particular, I was interested in understanding how to build and debug real video pipelines and how to integrate GStreamer with OpenCV for frame-level processing.

## Project Overview

The application implements a GStreamer-based video pipeline that:

- Reads video files using `decodebin`
- Extracts individual frames using `appsink`
- Processes frames with OpenCV in a dedicated `FrameProcessor` class
- Saves selected frames using a `FrameSaver` class

The focus lies on understanding GStreamer pipeline construction, dynamic pad handling, and the integration between GStreamer and OpenCV.

## Technologies

- **GStreamer 1.24** — Core focus of the project (pipeline construction, dynamic pads, `appsink`)
- **OpenCV 4.6+** — Frame processing and image manipulation
- **C++17 + CMake** — Modern C++ development
- **Docker** — Reproducible build and execution environment
- **Google Test + Clang-Tidy** — Basic testing and static code analysis

## Project Structure

```text
gstreamer-pipeline/
├── src/                    # Main application code
├── include/                # Header files
├── tests/                  # Unit and functional tests
├── docker/                 # Docker configuration
├── CMakeLists.txt
├── .clang-tidy
└── README.md
```

## Current Status

The project features a working GStreamer pipeline with real-time frame extraction and processing. Pipeline logic, image processing, and output are cleanly separated into dedicated classes. Docker support is fully functional, and basic static analysis is in place using Clang-Tidy.

Testing is currently at a basic level.

## Learnings & Limitations

This project helped me understand the complexity and flexibility of GStreamer, especially dynamic pad linking and frame handling via `appsink`.

I am aware that a production-grade solution would require more robust error handling, significantly better test coverage, and a more decoupled architecture. Some Clang-Tidy warnings remain, mostly due to GStreamer's C-based APIs.

The project was intentionally kept focused on core GStreamer concepts.

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
