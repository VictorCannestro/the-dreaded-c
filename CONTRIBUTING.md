# Contributing to Tic-Tac-Toe in C

This document provides guidelines and information for contributors.

## Table of Contents

- [Development Environment Setup](#development-environment-setup)
- [Building and Testing](#building-and-testing)
- [Development Workflow](#development-workflow)
- [Code Standards](#code-standards)
- [Testing Guidelines](#testing-guidelines)
- [Pull Request Process](#pull-request-process)
- [Troubleshooting](#troubleshooting)
- [Getting Help](#getting-help)

## Development Environment Setup

Choose your preferred development environment:

### 🚀 Option 1: Docker Container (Recommended)

No installation required! Everything runs in a pre-configured container.

#### Prerequisites
1. **Install Docker Desktop**
   - Download: https://www.docker.com/products/docker-desktop
   - Start Docker Desktop after installation

2. **Verify Docker works**
   ```bash
   docker --version
   ```

#### Get Started with Docker

1. **Open terminal in project folder**
   ```bash
   cd path/to/the-dreaded-c
   ```

2. **Build the container (first time only)**
   ```bash
   docker-compose build
   ```

3. **Run tests**
   ```bash
   docker-compose run --rm dev ceedling test:all
   ```

4. **Build and play the game**
   ```bash
   docker-compose run --rm dev ceedling release
   docker-compose run --rm dev ./build/release/tictactoe.exe
   ```

#### Docker Workflow

**Test your code:**
```bash
docker-compose run --rm dev ceedling test:all
```

**Build the game:**
```bash
docker-compose run --rm dev ceedling release
```

**Play the game:**
```bash
docker-compose run --rm dev ./build/release/tictactoe.exe
```

**Clean everything:**
```bash
docker-compose run --rm dev ceedling clean
```

**Get a shell in the container:**
```bash
docker-compose run --rm dev bash
```

### 💻 Option 2: Local Development

If you prefer to install tools locally, choose your platform below.

#### Windows

**Option A: Using Visual Studio (Recommended)**
1. Install Visual Studio Community 2019 or 2022
   - During installation, check "Desktop development with C++"
   - Download: https://visualstudio.microsoft.com/vs/community/

2. Install Ruby (for Ceedling)
   - Download: https://rubyinstaller.org/
   - Choose Ruby+Devkit version
   - Add Ruby to PATH during installation

3. Install Ceedling
   ```cmd
   gem install ceedling
   ```

**Option B: Using MinGW (Lightweight)**
1. Install Chocolatey: https://chocolatey.dev/install
2. Run in PowerShell (as Administrator):
   ```powershell
   choco install mingw ruby
   gem install ceedling
   ```

#### Linux/macOS

**Ubuntu/Debian:**
```bash
sudo apt-get update
sudo apt-get install ruby ruby-dev build-essential
sudo gem install ceedling
```

**macOS:**
```bash
# Install Xcode Command Line Tools
xcode-select --install

# Install Ruby (comes pre-installed on recent macOS)
# If needed: brew install ruby

# Install Ceedling
sudo gem install ceedling
```

**Fedora/RHEL:**
```bash
sudo dnf install ruby ruby-devel gcc
sudo gem install ceedling
```

## Building and Testing

### Build the Project

**All platforms:**
```bash
ceedling build
```

This will:
- Create a `build` directory
- Download and configure the Unity testing framework
- Compile your code
- Place executables in `build/release/`

### Run Tests

```bash
ceedling test:all
```

Expected output:
```
Test 'test_game_init_session_creates_empty_board': PASS
Test 'test_game_new_game_resets_board': PASS
...

-----------------------
OVERALL TEST SUMMARY
-----------------------
TESTED:  22
PASSED:  22
FAILED:   0
IGNORED:  0
```

### Run the Game

```bash
ceedling release
./build/release/tictactoe.exe  # All platforms (Linux/macOS/Windows)
```

## Development Workflow
### Making Changes

1. **Edit source code** in the appropriate files
2. **Build and test** your changes:
   ```bash
   ceedling test:all    # Run all tests
   ceedling release     # Build the game
   ```
3. **Verify** that all tests pass and the game works correctly
4. **Commit** your changes with a descriptive message

### Recommended VS Code Extensions

- **C/C++** by Microsoft (for IntelliSense)
- **Ruby** by Stafford Brunk (for Ruby syntax highlighting)
- **Ceedling** by Ceedling (for Ceedling project support)

## Code Standards

### C Coding Conventions

- **Naming**: Use `snake_case` for functions and variables, `PascalCase` for types/structs
- **Headers**: Function declarations in `.h` files, implementations in `.c` files
- **Types**: Use `typedef` for custom types and enums for better readability
- **Comments**: Document complex logic, especially in game algorithms
- **Error Handling**: Check return values and handle edge cases

### Examples from the codebase:

```c
// Good: Clear type definitions
typedef enum {
    PLAYER_HUMAN,
    PLAYER_COMPUTER
} PlayerType;

// Good: Descriptive function names
void game_init_session(GameState* state);
int game_make_move(GameState* state, int position);

// Good: Consistent error checking
if (position < 0 || position >= BOARD_SIZE) {
    return GAME_INVALID_MOVE;
}
```

### File Organization

- **Headers** (`include/*.h`): Function declarations, type definitions, constants
- **Implementation** (`src/*.c`): Function implementations, internal logic
- **Tests** (`tests/*.c`): Unit tests following Unity framework conventions

## Testing Guidelines

### Test Coverage

- All 22 unit tests verify core game functionality
- Tests cover edge cases, error conditions, and game logic
- Test names clearly describe what is being tested
- New features require corresponding tests before being merged

### Test Structure

```c
// Example test pattern
void test_game_function_specific_scenario(void) {
    // Arrange: Set up test data
    GameState state;
    game_init_session(&state);

    // Act: Call the function being tested
    GameStatus result = game_make_move(&state, 0);

    // Assert: Verify expected behavior
    TEST_ASSERT_EQUAL(GAME_SUCCESS, result);
    TEST_ASSERT_EQUAL(CELL_X, state.board[0]);
}
```

### Running Tests

- **All tests**: `ceedling test:all` (runs all 22 tests)
- **Quick validation**: `ceedling` (runs default tasks: test:all then release)
- **Release only**: `ceedling release` (builds game executable)
- **Clean build**: `ceedling clean` (removes all build artifacts)

### Test Requirements

- All 22 tests must pass before submitting changes
- New features require corresponding tests
- Bug fixes should include regression tests
- Run `ceedling test:all` to verify all tests pass

## Pull Request Process

1. **Fork** the repository
2. **Create a feature branch** from `main`
3. **Make your changes** following the code standards
4. **Add tests** for new functionality
5. **Ensure all tests pass**
6. **Update documentation** if needed
7. **Commit** with clear, descriptive messages
8. **Push** to your fork
9. **Create a Pull Request** with:
   - Clear description of changes
   - Reference to any related issues
   - Screenshots/demo if UI changes

### Commit Message Guidelines

```
feat: add minimax AI algorithm for computer player
fix: correct board validation for edge positions
test: add tests for draw detection
docs: update build instructions for new platforms
```

## Troubleshooting

### "Ruby not found"
- Windows: Install Ruby from https://rubyinstaller.org/ (check "Add Ruby to PATH")
- Linux: `sudo apt-get install ruby-full`
- macOS: `brew install ruby`

### "Ceedling not found"
- Install Ruby first (see above)
- Then: `gem install ceedling`

### "No C compiler found"
- Windows: Install Visual Studio Build Tools or MinGW
- Linux: `sudo apt-get install build-essential`
- macOS: `xcode-select --install`

### "Git not found (build fails)"
- Windows: Install Git from https://git-scm.com/
- Linux: `sudo apt-get install git`
- macOS: `brew install git`

### Build Issues
```bash
# Clean and rebuild
ceedling clean
ceedling test:all
```

### Still having issues?
1. Check that Ruby is installed: `ruby --version`
2. Verify Ceedling is installed: `ceedling version`
3. Check that your compiler is installed: `gcc --version` or `cl.exe`
4. Read the full README.md for detailed setup instructions
5. Check the Learning Guide: `docs/LEARNING_GUIDE.md`

## Getting Help

- **Documentation**: Check `docs/` folder for detailed guides
- **Issues**: Open a GitHub issue for bugs or feature requests
- **Discussions**: Use GitHub Discussions for questions
- **Learning**: Read `docs/LEARNING_GUIDE.md` for C concepts explanation
