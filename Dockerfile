# Use Ubuntu as base image with GCC, Ruby, and build tools
FROM ubuntu:22.04

# Install Ruby and dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    ruby-full \
    ruby-dev \
    git \
    wget \
    curl \
    && rm -rf /var/lib/apt/lists/*

# Update RubyGems and install Ceedling
RUN gem update --system && gem install ceedling -v 1.0.1

# Download and install Unity testing framework (for reference, Ceedling handles this)
WORKDIR /opt/unity
RUN git clone https://github.com/ThrowTheSwitch/Unity.git . && \
    git checkout v2.5.2

# Set working directory for project
WORKDIR /workspace

# Default command
CMD ["/bin/bash"]
