# Use Ubuntu as base image with GCC and build tools
FROM ubuntu:22.04

# Install dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    wget \
    curl \
    && rm -rf /var/lib/apt/lists/*

# Download and install Unity testing framework
WORKDIR /opt/unity
RUN git clone https://github.com/ThrowTheSwitch/Unity.git . && \
    git checkout v2.5.2

# Set working directory for project
WORKDIR /workspace

# Default command
CMD ["/bin/bash"]
