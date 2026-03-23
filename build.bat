@echo off
REM Build script for Tic-Tac-Toe project using Ceedling
REM This script sets up and builds the project on Windows

echo ===================================
echo Tic-Tac-Toe Project Builder (Ceedling)
echo ===================================
echo.

REM Check if Ceedling is installed
ceedling --version >nul 2>&1
if errorlevel 1 (
    echo ERROR: Ceedling is not installed.
    echo Install Ruby first, then run: gem install ceedling
    echo Or use the Docker container where it's pre-installed.
    exit /b 1
)

echo Building project with Ceedling...
ceedling build
if errorlevel 1 (
    echo ERROR: Build failed.
    exit /b 1
)

echo.
echo ===================================
echo Build completed successfully!
echo ===================================
echo.
echo You can now:
echo 1. Run tests:        ceedling test:all
echo 2. Run specific test: ceedling test:test_tictactoe
echo 3. Clean build:      ceedling clean
echo 4. Release build:    ceedling release
echo.
echo The game executable will be in build\release\tictactoe.exe
echo Test results will be in build\test\results\
