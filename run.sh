#!/bin/bash

echo "=== Building GStreamer C++ Pipeline ==="

# Clean previous build
rm -rf build
mkdir build
cd build

# Run CMake
echo "Running CMake..."
cmake ..

if [ $? -ne 0 ]; then
    echo "❌ CMake failed!"
    exit 1
fi

# Compile the project
echo "Compiling..."
make -j$(nproc)

if [ $? -ne 0 ]; then
    echo "❌ Compilation failed!"
    exit 1
fi

echo "✅ Build successful!"
echo ""
echo "=== Starting Pipeline ==="

# Run the executable
./gstreamer_pipeline

echo ""
echo "=== Running Tests ==="
./pipeline_tests