#!/bin/bash

echo "=== Setup for GStreamer + OpenCV project ==="

# System updated
echo "System update..."
sudo apt update && sudo apt upgrade -y

# GStreamer installieren
echo "GStreamer will be installed ..."
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
echo "OpenCV wird installiert..."
sudo apt install -y libopencv-dev python3-opencv

echo "=== Setup abgeschlossen! ==="
echo "Du kannst jetzt mit dem Build fortfahren."