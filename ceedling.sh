#!/bin/bash
# Helper script for common Ceedling operations
# Run from project root directory

set -e

show_help() {
    echo "Ceedling Helper Script"
    echo "======================"
    echo ""
    echo "Usage: $0 [command]"
    echo ""
    echo "Commands:"
    echo "  build     - Build everything (tests + release)"
    echo "  test      - Run all tests"
    echo "  release   - Build release version only"
    echo "  clean     - Clean build artifacts"
    echo "  clobber   - Clean everything including dependencies"
    echo "  help      - Show this help"
    echo ""
    echo "Examples:"
    echo "  $0 test      # Run tests"
    echo "  $0 build     # Build everything"
    echo "  $0 release   # Build game only"
}

check_ceedling() {
    if ! command -v ceedling &> /dev/null; then
        echo "ERROR: Ceedling is not installed."
        echo "Install with: gem install ceedling"
        exit 1
    fi
}

case "${1:-help}" in
    "build")
        check_ceedling
        echo "Building project with Ceedling..."
        ceedling build
        ;;
    "test")
        check_ceedling
        echo "Running tests..."
        ceedling test:all
        ;;
    "release")
        check_ceedling
        echo "Building release version..."
        ceedling release
        ;;
    "clean")
        check_ceedling
        echo "Cleaning build artifacts..."
        ceedling clean
        ;;
    "clobber")
        check_ceedling
        echo "Cleaning everything..."
        ceedling clobber
        ;;
    "help"|*)
        show_help
        ;;
esac