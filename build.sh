#!/bin/bash
# Build script for Tic-Tac-Toe project using Ceedling
# This script sets up and builds the project on Linux/macOS

set -e  # Exit on first error

echo "==================================="
echo "Tic-Tac-Toe Project Builder (Ceedling)"
echo "==================================="
echo ""

# Check if Ceedling is installed
if ! command -v ceedling &> /dev/null; then
    echo "ERROR: Ceedling is not installed."
    echo "Install it with: gem install ceedling"
    echo "Or run this in Docker container where it's pre-installed."
    exit 1
fi

echo "Building project with Ceedling..."
ceedling build

if [ $? -ne 0 ]; then
    echo "ERROR: Build failed."
    exit 1
fi

echo ""
echo "==================================="
echo "Build completed successfully!"
echo "==================================="
echo ""
echo "You can now:"
echo "1. Run tests:     ceedling test:all"
echo "2. Run specific test: ceedling test:test_tictactoe"
echo "3. Clean build:   ceedling clean"
echo "4. Release build: ceedling release"
echo ""
echo "The game executable will be in build/release/tictactoe.exe"
echo "Test results will be in build/test/results/"
