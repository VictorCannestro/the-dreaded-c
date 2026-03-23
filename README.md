# Tic-Tac-Toe in C

Tic-tac-toe with comprehensive unit testing, written in C with Ceedling build system. Features human vs computer gameplay with intelligent AI, symbol choice, and multiple game sessions - all satisfying the project requirements.

## �️ Tech Stack

| Component            | Technology              | Purpose                                                         |
| -------------------- | ----------------------- | --------------------------------------------------------------- |
| **Build System**     | Ceedling                | C project automation, compilation, and testing orchestration    |
| **Testing Framework**| Unity                   | C Unit testing, assertions, and test runner generation          |
| **Mocking**          | CMock                   | Automated mock generation for test isolation                    |
| **Compiler**         | GCC/MinGW               | C compilation for Windows, Linux, and macOS                     |
| **Language**         | C (C99 standard)        | Core implementation language                                    |
| **Runtime**          | Ruby                    | Ceedling dependency for build automation                        |
| **Containerization** | Docker + Docker Compose | Isolated development environment (optional)                     |


## �📋 Prerequisites

Before you begin, make sure you have:

1. **Ruby** (version 2.0+)
   - **Windows**: Download from https://rubyinstaller.org/
   - **Linux**: Usually pre-installed or `sudo apt-get install ruby`
   - **macOS**: Usually pre-installed or `brew install ruby`

2. **Ceedling**
   - Install with: `gem install ceedling`
   - This will also install Unity testing framework automatically

3. **C Compiler**
   - **Windows**: Visual Studio Build Tools or MinGW
     - Visual Studio Community: https://visualstudio.microsoft.com/vs/community/
     - Or: `choco install mingw` (using Chocolatey)
   - **Linux**: GCC is usually pre-installed
   - **macOS**: Install Xcode Command Line Tools: `xcode-select --install`

## 🏗️ Project Structure

```
the-dreaded-c/
├── project.yml             # Ceedling build configuration
├── README.md               # This file
├── include/                # Header files (.h)
│   ├── tictactoe.h        # Core game logic interface
│   └── display.h          # Display/UI interface
├── src/                    # Implementation files (.c)
│   ├── tictactoe.c        # Game logic implementation
│   ├── display.c          # Display implementation
│   └── main.c             # Interactive game program
├── tests/                  # Test files
│   └── test_tictactoe.c   # Unit tests using Unity framework
└── build/                  # Build output (created by Ceedling)
    ├── artifacts/
    ├── test/
    │   ├── results/       # Test results and reports
    │   └── out/          # Compiled test executables
    └── release/          # Release build executables
        └── tictactoe.exe # Main game executable
```

## 🚀 Getting Started

### Step 1: Clone or Download the Project

```bash
cd your-workspace-folder
# Project should be in: the-dreaded-c/
```

### Step 2: Build and Test the Project

```bash
cd the-dreaded-c

# Run all tests (recommended first)
ceedling test:all

# Build the game executable
ceedling release

# Or run both automatically (default tasks)
ceedling
```

**Note:** Ceedling will automatically download and configure the Unity test framework.

### Step 3: Run the Game

```bash
# Linux/macOS (in Docker container)
./build/release/tictactoe.exe

# Windows (native)
.\build\release\tictactoe.exe
```

### Step 4: View Test Results

```bash
# Run tests and see results
ceedling test:all

# Test results are saved in build/test/results/
# Open build/test/results/test_tictactoe.xml for detailed results
```

## 📝 How to Play

1. **Choose Your Symbol**: For the first game, choose to play as X or O
2. **Gameplay**: The game features human vs computer with intelligent AI
3. **Board Layout**: 3x3 grid with positions numbered 0-8:
   ```
      0 | 1 | 2
     -----------
      3 | 4 | 5
     -----------
      6 | 7 | 8
   ```
4. **Rules**:
   - X always makes the first move
   - Players alternate turns
   - Place your symbol in any empty position
   - First to get 3 in a row (horizontal, vertical, or diagonal) wins
   - If board fills with no winner, it's a draw
5. **Multiple Games**: Winner of each game chooses symbols for the next game
6. **AI Opponent**: Computer uses minimax algorithm for perfect play

### 🎯 **Game Flow:**
- First game: Human chooses X or O
- Subsequent games: Previous winner chooses symbols
- Draw games: Keep same symbol assignments

## 🧪 Understanding the Tests

The project includes 22 comprehensive unit tests covering:

- **Session Management**: Game session initialization and symbol choice
- **Player Types**: Human vs computer player management
- **AI Logic**: Computer move validation and strategic play
- **Game Flow**: Multiple game sessions with winner choice logic
- **Board Display**: Correct symbol rendering ('_', 'X', 'O')
- **Win Conditions**: All winning combinations and draw detection
- **Move Validation**: Position bounds and occupancy checking
- **Edge Cases**: Null pointers, invalid inputs, game state transitions

To view test results:
```bash
ceedling test:all
```

Test results show detailed Unity framework output with pass/fail status.
