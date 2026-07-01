# gstreamer-pipeline

This project builds a small C++ GStreamer pipeline that reads a video file, processes frames, and writes selected outputs to the output/frames directory.

## Run locally

```bash
./run.sh
```

## Run with Docker

From the repository root:

```bash
docker compose -f docker/docker-compose.yml up --build
```

The compose setup mounts the project output directory and the sample video into the container so the demo works in a containerized environment.