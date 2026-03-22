@echo off
REM Build script for Tic-Tac-Toe project
REM This script sets up and builds the project

setlocal enabledelayedexpansion

echo ===================================
echo Tic-Tac-Toe Project Builder
echo ===================================
echo.

REM Check if build directory exists
if not exist "build" (
    echo Creating build directory...
    mkdir build
    echo Build directory created.
    echo.
)

REM Change to build directory
cd build

REM Check if CMakeCache.txt exists (project already configured)
if not exist "CMakeCache.txt" (
    echo Configuring project with CMake...
    cmake .. -G "Visual Studio 16 2019"
    if errorlevel 1 (
        echo ERROR: CMake configuration failed.
        echo Make sure you have:
        echo   - CMake installed (https://cmake.org/download/)
        echo   - Visual Studio Build Tools or Community Edition
        echo   - Git (for downloading Unity test framework)
        cd ..
        exit /b 1
    )
    echo Project configured successfully.
    echo.
)

echo Building project...
cmake --build . --config Release
if errorlevel 1 (
    echo ERROR: Build failed.
    cd ..
    exit /b 1
)

echo.
echo ===================================
echo Build completed successfully!
echo ===================================
echo.
echo You can now:
echo 1. Run the game: .\Release\tictactoe.exe
echo 2. Run tests:    .\run_tests.exe
echo 3. Run with ctest: ctest
echo.
echo Or from the parent directory:
echo   - play the game: cd build ^& .\Release\tictactoe.exe
echo   - run tests:     cd build ^& .\run_tests.exe
echo.

cd ..
