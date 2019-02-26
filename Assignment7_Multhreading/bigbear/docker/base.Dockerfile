### Base environment container ###
# Get the base Ubuntu image from Docker Hub
FROM ubuntu:latest as base

# Update the base image and install build environment
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    curl \
    gcovr \
    libboost-log-dev \
    libboost-signals-dev \
    libboost-system-dev \
    libboost-regex-dev \
    libgtest-dev
