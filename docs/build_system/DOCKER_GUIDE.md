# Docker Development Environment Guide

This guide explains how we use **Docker** to create a consistent development environment for this project. If you're new to Docker, this document will help you understand why it's valuable and how to use it.

## Table of Contents

1. [What is Docker?](#what-is-docker)
2. [Why Use Docker for Development?](#why-use-docker-for-development)
3. [Docker in Our Project](#docker-in-our-project)
4. [Understanding the Dockerfile](#understanding-the-dockerfile)
5. [Understanding docker-compose.yml](#understanding-docker-composeyml)
6. [Getting Started](#getting-started)
7. [Common Tasks](#common-tasks)
8. [How Docker Relates to CI](#how-docker-relates-to-ci)

---

## What is Docker?

**Docker** is a platform that packages applications and their dependencies into isolated units called **containers**. Think of a container as a lightweight, portable "mini computer" that has everything needed to run your code.

### Key Concepts

| Term               | Description                                                 |
|--------------------|-------------------------------------------------------------|
| **Image**          | A blueprint/template containing OS, tools, and dependencies |
| **Container**      | A running instance of an image (like a lightweight VM)      |
| **Dockerfile**     | Instructions to build an image                              |
| **docker-compose** | Tool to define and run multi-container setups               |
| **Volume**         | Shared folder between your machine and the container        |

### Containers vs Virtual Machines

```
┌─────────────────────────────────────────────────────────────┐
│                    Virtual Machines                          │
├─────────────────────────────────────────────────────────────┤
│  ┌─────────┐  ┌─────────┐  ┌─────────┐                      │
│  │  App A  │  │  App B  │  │  App C  │                      │
│  ├─────────┤  ├─────────┤  ├─────────┤                      │
│  │Guest OS │  │Guest OS │  │Guest OS │  ← Full OS each!     │
│  └─────────┘  └─────────┘  └─────────┘                      │
│  ┌─────────────────────────────────────┐                    │
│  │            Hypervisor               │                    │
│  └─────────────────────────────────────┘                    │
│  ┌─────────────────────────────────────┐                    │
│  │             Host OS                 │                    │
│  └─────────────────────────────────────┘                    │
└─────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────┐
│                       Containers                             │
├─────────────────────────────────────────────────────────────┤
│  ┌─────────┐  ┌─────────┐  ┌─────────┐                      │
│  │  App A  │  │  App B  │  │  App C  │                      │
│  └─────────┘  └─────────┘  └─────────┘                      │
│  ┌─────────────────────────────────────┐                    │
│  │          Docker Engine              │  ← Shared kernel   │
│  └─────────────────────────────────────┘                    │
│  ┌─────────────────────────────────────┐                    │
│  │             Host OS                 │                    │
│  └─────────────────────────────────────┘                    │
└─────────────────────────────────────────────────────────────┘
```

Containers are **faster** and **lighter** because they share the host OS kernel.

---

## Why Use Docker for Development?

### The "Works on My Machine" Problem

Without Docker:
```
Developer A: "The tests pass on my machine!"
Developer B: "They fail on mine..."
Developer A: "What version of GCC do you have?"
Developer B: "GCC? I'm using Clang..."
Developer A: "What about Ruby?"
Developer B: "Ruby 2.7"
Developer A: "I have 3.2..."
```

With Docker:
```
Developer A: "Run 'docker compose up'"
Developer B: "Tests pass! Same environment as you."
```

### Benefits for This Project

| Benefit                    | Explanation                                             |
|----------------------------|---------------------------------------------------------|
| **Consistent Environment** | Everyone uses the same GCC, Ruby, and Ceedling versions |
| **No Installation Hassle** | Don't need to install Ruby or Ceedling on your machine  |
| **Works on Any OS**        | Same container runs on Windows, macOS, and Linux        |
| **Isolated**               | Project dependencies don't conflict with other projects |
| **Reproducible**           | New team members get the exact same setup               |
| **Matches CI**             | Local environment mirrors what runs in GitHub Actions   |

---

## Docker in Our Project
### What's Included

Our Docker setup provides a complete development environment:

- **Ubuntu 22.04** - Base operating system
- **GCC** - C compiler (via build-essential)
- **Ruby 3.x** - Required for Ceedling
- **Ceedling 1.0.1** - Build system and test runner
- **Unity** - Unit testing framework (bundled with Ceedling)
- **Git, curl, wget** - Helpful utilities

### File Structure

```
project/
├── Dockerfile           # Defines the container image
├── docker-compose.yml   # Simplifies running the container
├── src/                 # Your code (mounted into container)
├── tests/               # Your tests (mounted into container)
└── ...
```

---

## Understanding the Dockerfile

The `Dockerfile` is a recipe for building our development image:

```dockerfile
# Start with Ubuntu 22.04 as our base
FROM ubuntu:22.04

# Install system packages
RUN apt-get update && apt-get install -y \
    build-essential \    # GCC, make, etc.
    ruby-full \          # Ruby interpreter
    ruby-dev \           # Ruby development headers
    git \                # Version control
    wget \               # Download utility
    curl \               # HTTP utility
    && rm -rf /var/lib/apt/lists/*  # Clean up to reduce image size

# Install Ceedling (our build system)
RUN gem update --system && gem install ceedling -v 1.0.1

# Clone Unity testing framework (for reference)
WORKDIR /opt/unity
RUN git clone https://github.com/ThrowTheSwitch/Unity.git . && \
    git checkout v2.5.2

# Set the working directory for our project
WORKDIR /workspace

# Default command when container starts
CMD ["/bin/bash"]
```

### Key Instructions

| Instruction | Purpose                                    |
|-------------|--------------------------------------------|
| `FROM`      | Base image to start from                   |
| `RUN`       | Execute a command (install packages, etc.) |
| `WORKDIR`   | Set the current directory                  |
| `CMD`       | Default command when container starts      |

---

## Understanding docker-compose.yml

`docker-compose.yml` makes it easy to run the container with the right settings:

```yaml
services:
  dev:                          # Service name
    build: .                    # Build image from ./Dockerfile
    container_name: tictactoe-dev
    volumes:
      - .:/workspace            # Mount current directory into container
    working_dir: /workspace     # Start in /workspace
    stdin_open: true            # Keep STDIN open (for interactive use)
    tty: true                   # Allocate a terminal
    command: /bin/bash          # Start a bash shell
```

### The Volume Mount

```yaml
volumes:
  - .:/workspace
```

This is the magic that makes development smooth:

```
Your Machine                    Container
┌──────────────┐               ┌──────────────┐
│  project/    │ ←──────────→  │  /workspace  │
│  ├── src/    │   (synced)    │  ├── src/    │
│  ├── tests/  │               │  ├── tests/  │
│  └── ...     │               │  └── ...     │
└──────────────┘               └──────────────┘
```

- Edit files on your machine with your favorite editor
- Changes instantly appear in the container
- Build and test inside the container
- Results appear on your machine

---

## Getting Started

### Prerequisites

1. **Install Docker Desktop**
   - macOS: [Docker Desktop for Mac](https://docs.docker.com/desktop/install/mac-install/)
   - Windows: [Docker Desktop for Windows](https://docs.docker.com/desktop/install/windows-install/)
   - Linux: [Docker Engine](https://docs.docker.com/engine/install/)

2. **Verify Installation**
   ```bash
   docker --version
   docker compose version
   ```

### First Time Setup

```bash
# Navigate to project directory
cd /path/to/the-dreaded-c

# Build the Docker image (only needed once, or after Dockerfile changes)
docker compose build

# Start the development container
docker compose run --rm dev
```

You're now inside the container! Try:
```bash
ceedling test:all    # Run tests
ceedling release     # Build the game
```

### Quick Reference

| Command                        | What It Does                    |
|--------------------------------|---------------------------------|
| `docker compose build`         | Build the image from Dockerfile |
| `docker compose run --rm dev`  | Start container and get a shell |
| `docker compose up -d`         | Start container in background   |
| `docker compose down`          | Stop and remove container       |
| `docker compose exec dev bash` | Open shell in running container |

---

## Common Tasks

### Running Tests in Docker

```bash
# Start container and run tests
docker compose run --rm dev ceedling test:all

# Or interactively:
docker compose run --rm dev
ceedling test:all
```

### Building the Game

```bash
docker compose run --rm dev ceedling release
# Output: build/release/tictactoe.exe (accessible on your machine too!)
```

### Interactive Development Session

```bash
# Start a shell in the container
docker compose run --rm dev

# Now you're inside the container:
ceedling test:all           # Run tests
ceedling test:test_board    # Run specific test
ceedling release            # Build game
exit                        # Leave container
```

### Rebuilding the Image

If you modify the `Dockerfile`:
```bash
docker compose build --no-cache
```

### Cleaning Up

```bash
# Remove the container
docker compose down

# Remove the image (to free disk space)
docker rmi tictactoe-dev

# Remove all unused Docker resources
docker system prune
```

---

## How Docker Relates to CI

Our development flow uses Docker locally and GitHub Actions in CI, but they serve the same purpose: **consistent environments**.

```
┌─────────────────────────────────────────────────────────────┐
│                    Local Development                        │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│   Your Machine          Docker Container                    │
│   ┌──────────┐         ┌──────────────────┐                 │
│   │  Editor  │ ──────→ │ Ubuntu 22.04     │                 │
│   │  (edit   │         │ GCC              │                 │
│   │   code)  │         │ Ruby + Ceedling  │                 │
│   └──────────┘         │                  │                 │
│                        │ ceedling test:all│                 │
│                        └──────────────────┘                 │
│                                                             │
└─────────────────────────────────────────────────────────────┘
                              │
                              │ git push
                              ▼
┌─────────────────────────────────────────────────────────────┐
│                      GitHub Actions                         │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│   GitHub Runner (Ubuntu/macOS VM)                           │
│   ┌──────────────────────────────────────┐                  │
│   │ 1. Install Ruby                      │                  │
│   │ 2. Install GCC                       │                  │
│   │ 3. Install Ceedling 1.0.1            │                  │
│   │ 4. ceedling test:all                 │                  │
│   │ 5. ceedling release                  │                  │
│   └──────────────────────────────────────┘                  │
│                                                             │
└─────────────────────────────────────────────────────────────┘
```

### Why Both?

| Tool               | Use Case                                                             |
|--------------------|----------------------------------------------------------------------|
| **Docker**         | Local development - no need to install Ruby/Ceedling on your machine |
| **GitHub Actions** | CI - automated testing on every push/PR                              |

Both ensure you're using:
- The same Ceedling version (1.0.1)
- Compatible GCC version
- Same build commands

---

## Troubleshooting

### "Cannot connect to Docker daemon"

Docker Desktop isn't running. Start it from your applications.

### "Permission denied" on Linux

Add your user to the docker group:
```bash
sudo usermod -aG docker $USER
# Then log out and back in
```

### Container Can't See My Files

Make sure you're running docker compose from the project root:
```bash
cd /path/to/the-dreaded-c
docker compose run --rm dev
```

### Changes to Dockerfile Aren't Reflected

Rebuild the image:
```bash
docker compose build --no-cache
```

### "No space left on device"

Clean up unused Docker resources:
```bash
docker system prune -a
```

### Build Artifacts Have Wrong Permissions (Linux)

Files created in the container may be owned by root. Fix with:
```bash
sudo chown -R $USER:$USER build/
```

---

## When to Use Docker vs Native

| Scenario                            | Recommendation              |
|-------------------------------------|-----------------------------|
| Don't want to install Ruby/Ceedling | Use Docker                  |
| Quick edit-test cycle               | Either works                |
| Windows without WSL                 | Use Docker                  |
| CI/CD pipeline                      | GitHub Actions (not Docker) |
| Debugging C code with GDB           | Native may be easier        |
| First time setup                    | Docker (faster)             |

---

## Further Reading

- [Docker Documentation](https://docs.docker.com/)
- [Docker Compose Documentation](https://docs.docker.com/compose/)
- [Docker for Development Best Practices](https://docs.docker.com/develop/dev-best-practices/)
- [Docker Desktop](https://www.docker.com/products/docker-desktop/)

