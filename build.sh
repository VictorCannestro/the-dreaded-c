#!/bin/bash
# Build script for Tic-Tac-Toe project
# This script sets up and builds the project on Linux/macOS

set -e  # Exit on first error

echo "==================================="
echo "Tic-Tac-Toe Project Builder"
echo "==================================="
echo ""

# Create build directory if it doesn't exist
if [ ! -d "build" ]; then
    echo "Creating build directory..."
    mkdir build
    echo "Build directory created."
    echo ""
fi

# Change to build directory
cd build

# Configure project if not already done
if [ ! -f "CMakeCache.txt" ]; then
    echo "Configuring project with CMake..."
    cmake ..
    if [ $? -ne 0 ]; then
        echo "ERROR: CMake configuration failed."
        echo "Make sure you have:"
        echo "  - CMake installed"
        echo "  - A C compiler (gcc, clang, etc.)"
        echo "  - Git (for downloading Unity test framework)"
        cd ..
        exit 1
    fi
    echo "Project configured successfully."
    echo ""
fi

echo "Building project..."
make

if [ $? -ne 0 ]; then
    echo "ERROR: Build failed."
    cd ..
    exit 1
fi

echo ""
echo "==================================="
echo "Build completed successfully!"
echo "==================================="
echo ""
echo "You can now:"
echo "1. Run the game:  ./tictactoe"
echo "2. Run tests:     ./run_tests"
echo "3. Run with ctest: ctest"
echo ""
echo "Or from the parent directory:"
echo "  - play the game: cd build && ./tictactoe"
echo "  - run tests:     cd build && ./run_tests"
echo ""

cd ..
