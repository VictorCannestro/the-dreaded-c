cmake_minimum_required(VERSION 3.10)
project(TicTacToe C)

# Set C standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Add source files
set(TICTACTOE_SOURCES
    src/tictactoe.c
    src/display.c
)

set(TICTACTOE_HEADERS
    include/tictactoe.h
    include/display.h
)

# Include directories
include_directories(include)

# ============================================================================
# Download and configure Unity Test Framework
# ============================================================================
include(FetchContent)
FetchContent_Declare(
    unity
    GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
    GIT_TAG v2.5.2
)
FetchContent_MakeAvailable(unity)

# ============================================================================
# Main executable: tic-tac-toe game
# ============================================================================
add_executable(tictactoe ${TICTACTOE_SOURCES} src/main.c)
target_include_directories(tictactoe PRIVATE include)

# ============================================================================
# Test executable
# ============================================================================
# Build game library without main
add_library(tictactoe_lib ${TICTACTOE_SOURCES})
target_include_directories(tictactoe_lib PUBLIC include)

# Build test executable
add_executable(run_tests 
    tests/test_tictactoe.c
)
target_link_libraries(run_tests 
    tictactoe_lib
    unity
)
target_include_directories(run_tests PRIVATE include ${unity_SOURCE_DIR}/src)

# ============================================================================
# Enable testing
# ============================================================================
enable_testing()
add_test(NAME TicTacToeTests COMMAND run_tests)

# ============================================================================
# Optional: Add custom targets for convenience
# ============================================================================
add_custom_target(build-game
    DEPENDS tictactoe
    COMMENT "Building the tic-tac-toe game..."
)

add_custom_target(run-game
    COMMAND $<TARGET_FILE:tictactoe>
    DEPENDS tictactoe
    COMMENT "Running the tic-tac-toe game..."
)

add_custom_target(run-tests
    COMMAND $<TARGET_FILE:run_tests>
    DEPENDS run_tests
    COMMENT "Running unit tests..."
)
