```
▄▄▄█████▓ ██▓ ▄████▄  ▄▄▄█████▓ ▄▄▄       ▄████▄  ▄▄▄█████▓ ▒█████  ▓█████ 
▓  ██▒ ▓▒▓██▒▒██▀ ▀█  ▓  ██▒ ▓▒▒████▄    ▒██▀ ▀█  ▓  ██▒ ▓▒▒██▒  ██▒▓█   ▀ 
▒ ▓██░ ▒░▒██▒▒▓█    ▄ ▒ ▓██░ ▒░▒██  ▀█▄  ▒▓█    ▄ ▒ ▓██░ ▒░▒██░  ██▒▒███   
░ ▓██▓ ░ ░██░▒▓▓▄ ▄██▒░ ▓██▓ ░ ░██▄▄▄▄██ ▒▓▓▄ ▄██▒░ ▓██▓ ░ ▒██   ██░▒▓█  ▄ 
  ▒██▒ ░ ░██░▒ ▓███▀ ░  ▒██▒ ░  ▓█   ▓██▒▒ ▓███▀ ░  ▒██▒ ░ ░ ████▓▒░░▒████▒
  ▒ ░░   ░▓  ░ ░▒ ▒  ░  ▒ ░░    ▒▒   ▓▒█░░ ░▒ ▒  ░  ▒ ░░   ░ ▒░▒░▒░ ░░ ▒░ ░
    ░     ▒ ░  ░  ▒       ░      ▒   ▒▒ ░  ░  ▒       ░      ░ ▒ ▒░  ░ ░  ░
  ░       ▒ ░░          ░        ░   ▒   ░          ░      ░ ░ ░ ▒     ░   
          ░  ░ ░                     ░  ░░ ░                   ░ ░     ░  ░
             ░                           ░                                     
```

A modular, well-tested implementation of Tic-Tac-Toe in C, designed as a teaching project for software engineering principles. Features human vs computer gameplay with two difficulty levels, comprehensive unit tests, and clean separation of concerns.

## Project Purpose
This project demonstrates:
- **Modular C architecture** — 10 focused source files, each < 200 lines
- **Comprehensive testing** — 100+ unit tests with 100% pass rate
- **Clean API design** — consistent naming, clear return values
- **UI abstraction** — swappable interface layer (CLI, GUI-ready)
- **Configurable board size** — easily extend to 4x4 or larger boards
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


## Learning Resources
### [Skill Levels](skill_levels/README.md)
Organized into three progressive levels of complexity. Each level is a complete, buildable version of the game, designed
to illustrate specific concepts and common pitfalls.

**Progressive learning levels**:
- **Level 1**: Single file, globals, magic numbers (common beginner mistakes)
- **Level 2**: Separate files, enums, structs (better organization)
- **Level 3**: This version (tests, minimax AI, full features)

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

# Run all tests 
ceedling test:all

# Build the game
ceedling release

# Play!
./build/release/tictactoe.exe   # Linux/macOS
.\build\release\tictactoe.exe   # Windows
```


## Contributing
See [CONTRIBUTING.md](CONTRIBUTING.md) for more details on how to build and test the project, as well as guidelines for contributing
code or improvements.


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


## Architecture
```
┌─────────────────────────────────────────────────────────────┐
│                        main.c                               │
│                  (game loop & flow)                         │
└─────────────────────────────────────────────────────────────┘
                            │
              ┌─────────────┴─────────────┐
              ▼                           ▼
┌──────────────────────────┐   ┌─────────────────────────────┐
│     tictactoe.c          │   │     ui_interface.h          │
│  (game session logic)    │   │   (UI abstraction layer)    │
└──────────────────────────┘   └─────────────────────────────┘
              │                           │
    ┌─────────┼─────────┐        ┌────────┴────────┐
    ▼         ▼         ▼        ▼                 ▼
┌────────┐ ┌────────┐ ┌────────┐ ┌──────────┐ ┌──────────────┐
│board.c │ │ ai.c   │ │ai_easy │ │ ui_cli.c │ │ui_gui_example│
│ (data) │ │ (hard) │ │(random)│ │  (CLI)   │ │   (SDL2)     │
└────────┘ └────────┘ └────────┘ └──────────┘ └──────────────┘
    │           │          │
    └─────┬─────┴──────────┘
          ▼
┌─────────────────────────────────────────────────────────────┐
│            win_condition_calculator.c                       │
│  (game rules - configurable board size & win length)        │
└─────────────────────────────────────────────────────────────┘
          │
          ▼
┌─────────────────────────────────────────────────────────────┐
│                      constants.h                            │
│      (BOARD_DIM, BOARD_SIZE, WIN_LENGTH, types, enums)      │
└─────────────────────────────────────────────────────────────┘
```

### Key Design Decisions

**UI Abstraction Layer** (`ui_interface.h`)
- Function pointer struct enables swapping CLI/GUI without changing game logic
- `ui_cli.c` provides terminal-based implementation
- `ui_gui_example.c` demonstrates SDL2 GUI implementation pattern

**Configurable Board Size** (`constants.h`)
```c
#define BOARD_DIM 3              // Change to 4, 5, etc.
#define BOARD_SIZE (BOARD_DIM * BOARD_DIM)
#define WIN_LENGTH BOARD_DIM     // Symbols needed to win
```

**Win Condition Calculator** — Testable helper functions:
- `wcc_check_rows()` — horizontal wins
- `wcc_check_columns()` — vertical wins
- `wcc_check_main_diagonals()` — top-left to bottom-right
- `wcc_check_anti_diagonals()` — top-right to bottom-left

