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
- Clean separation between pipeline logic and image processing

## Technologies Used

- **GStreamer** (1.24)
- **OpenCV** (4.6+)
- **C++17**
- **CMake**
- **Docker**

## Project Structure
gstreamer-pipeline/
├── src/                    # Source code
│   ├── main.cpp
│   ├── gstreamer_pipeline.cpp
│   ├── FrameProcessor.cpp
│   └── FrameSaver.cpp
├── include/                # Header files
├── docker/                 # Docker configuration
├── CMakeLists.txt
├── run.sh
├── setup.sh
└── README.md

## Requirements

- Ubuntu 22.04 / 24.04 (or equivalent)
- GStreamer and OpenCV development packages
- CMake 3.16 or higher
- C++17 compatible compiler

## Build & Run

### Option 1: Local Build (recommended for development)

```bash
# Install dependencies
./setup.sh

# Build and run the project
./run.sh

### Option 2: Run with docker

From the repository root:

```bash
docker compose -f docker/docker-compose.yml up --build
```
