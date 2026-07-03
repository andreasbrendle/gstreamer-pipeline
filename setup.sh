#!/bin/bash

echo "=== Setup for GStreamer + OpenCV project ==="

# System updated
echo "System update..."
sudo apt update && sudo apt upgrade -y

# === Development Tools ===
echo "Installing development tools..."
sudo apt install -y \
    clang-tidy \
    clang-format

# GStreamer installieren
echo "Installing GStreamer..."
sudo apt install -y \
    gstreamer1.0-tools \
    gstreamer1.0-plugins-good \
    gstreamer1.0-plugins-bad \
    gstreamer1.0-plugins-ugly \
    gstreamer1.0-libav \
    libgstreamer1.0-dev \
    libgstreamer-plugins-base1.0-dev \
    libgstreamer-plugins-good1.0-dev \
    libgstreamer-plugins-bad1.0-dev

# OpenCV installieren
echo "Installing OpenCV..."
sudo apt install -y libopencv-dev python3-opencv

echo "=== Setup completed! ==="