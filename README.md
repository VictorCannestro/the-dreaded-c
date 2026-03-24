# Tic-Tac-Toe in C
A modular, well-tested implementation of Tic-Tac-Toe in C, designed as a teaching project for software engineering 
principles. Features human vs computer gameplay with two difficulty levels, comprehensive unit tests, and clean 
separation of concerns.


## Project Purpose
This project demonstrates:
- **Modular C architecture** — 10 focused source files, each < 200 lines
- **Comprehensive testing** — 80+ unit tests with 100% pass rate
- **Clean API design** — consistent naming, clear return values
- **Defensive programming** — NULL checks, bounds validation, assertions
- **Refactoring techniques** — before/after examples in the learning guide

## 🛠️ Tech Stack
| Component          | Technology        | Purpose                       |
|--------------------|-------------------|-------------------------------|
| **Language**       | C (C99)           | Core implementation           |
| **Build System**   | Ceedling          | Test automation & compilation |
| **Test Framework** | Unity             | Unit testing & assertions     |
| **Compiler**       | GCC/Clang         | Cross-platform compilation    |
| **Container**      | Docker (optional) | Isolated dev environment      |


## Architecture
```
┌─────────────────────────────────────────────────┐
│  main.c  (entry point, user interaction)        │
└─────────────────────────────────────────────────┘
         │
         ▼
┌─────────────────────────────────────────────────┐
│  tictactoe.c  (game session coordinator)        │
└─────────────────────────────────────────────────┘
         │
    ┌────┴────┬─────────────┬─────────────┐
    ▼         ▼             ▼             ▼
┌────────┐ ┌────────┐ ┌──────────┐ ┌──────────┐
│board.c │ │ ai.c   │ │ai_easy.c │ │display.c │
│ (data) │ │ (hard) │ │ (random) │ │ (output) │
└────────┘ └────────┘ └──────────┘ └──────────┘
    │           │           │
    └─────┬─────┴───────────┘
          ▼
┌─────────────────────────────────────────────────┐
│  win_condition_calculator.c  (game rules)       │
└─────────────────────────────────────────────────┘
          │
          ▼
┌─────────────────────────────────────────────────┐
│  constants.h  (shared types and enums)          │
└─────────────────────────────────────────────────┘
```


## Contributing
See [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines.


## Prerequisites
**Ruby** (for Ceedling):
```bash
# macOS
brew install ruby

# Linux
sudo apt-get install ruby

# Windows
# Download from https://rubyinstaller.org/
```

**Ceedling** (test framework):
```bash
gem install ceedling
```

**C Compiler**:
```bash
# macOS
xcode-select --install

# Linux
sudo apt-get install gcc

# Windows
# Install MinGW or Visual Studio Build Tools
```

## Quick Start
```bash
# Clone the project
git clone <repository-url>
cd the-dreaded-c

# Run all tests (84 tests)
ceedling test:all

# Build the game
ceedling release

# Play!
./build/release/tictactoe.exe   # Linux/macOS
.\build\release\tictactoe.exe   # Windows
```


## How to Play
1. **Choose difficulty**: Easy (random AI) or Hard (unbeatable minimax AI)
2. **Choose your symbol**: X or O (X always moves first)
3. **Enter moves**: Type position 0-8 when prompted

```
Board positions:
   0 | 1 | 2
  -----------
   3 | 4 | 5
  -----------
   6 | 7 | 8
```

4. **Win conditions**: 3 in a row (horizontal, vertical, or diagonal)
5. **After each game**: Winner chooses symbols for the next game


## Learning Resources
This project includes comprehensive documentation for learning C and software engineering:

### [Learning Guide](docs/LEARNING_GUIDE.md)
**Core C Concepts** — Types, enums, structs, arrays, pointers, functions, headers, memory

**Software Engineering Practices:**
- Separation of concerns
- API design principles
- Defensive programming
- Unit testing strategies
- Code smells & refactoring

**15 Common C Pitfalls** — With examples and fixes:
- Missing includes for NULL
- `()` vs `(void)` in function declarations
- Array bounds errors
- Uninitialized pointers
- And 11 more...


### [Requirements](docs/REQUIREMENTS.md)
Full specification of game features and behavior.

