@echo off
REM Helper script for common Ceedling operations
REM Run from project root directory

setlocal

if "%1"=="build" goto build
if "%1"=="test" goto test
if "%1"=="release" goto release
if "%1"=="clean" goto clean
if "%1"=="clobber" goto clobber
if "%1"=="help" goto help

:help
echo Ceedling Helper Script
echo ======================
echo.
echo Usage: %0 [command]
echo.
echo Commands:
echo   build     - Build everything (tests + release)
echo   test      - Run all tests
echo   release   - Build release version only
echo   clean     - Clean build artifacts
echo   clobber   - Clean everything including dependencies
echo   help      - Show this help
echo.
echo Examples:
echo   %0 test      # Run tests
echo   %0 build     # Build everything
echo   %0 release   # Build game only
echo.
goto end

:check_ceedling
ceedling --version >nul 2>&1
if errorlevel 1 (
    echo ERROR: Ceedling is not installed.
    echo Install with: gem install ceedling
    exit /b 1
)
goto :eof

:build
call :check_ceedling
echo Building project with Ceedling...
ceedling build
goto end

:test
call :check_ceedling
echo Running tests...
ceedling test:all
goto end

:release
call :check_ceedling
echo Building release version...
ceedling release
goto end

:clean
call :check_ceedling
echo Cleaning build artifacts...
ceedling clean
goto end

:clobber
call :check_ceedling
echo Cleaning everything...
ceedling clobber
goto end

:end