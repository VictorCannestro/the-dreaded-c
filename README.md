# Tic-Tac-Toe in C

A professional tic-tac-toe game with comprehensive unit testing, written in C with CMake build system. Perfect for learning C fundamentals and professional C development practices.

## 📋 Prerequisites

Before you begin, make sure you have:

1. **CMake** (version 3.10+)
   - Download from: https://cmake.org/download/
   - Or install via package manager: `choco install cmake` (Windows)

2. **C Compiler**
   - **Windows**: Visual Studio Build Tools or MinGW
     - Visual Studio Community: https://visualstudio.microsoft.com/vs/community/
     - Or: `choco install mingw` (using Chocolatey)
   - **Linux**: GCC is usually pre-installed
   - **macOS**: Install Xcode Command Line Tools: `xcode-select --install`

3. **Git** (optional, but recommended)
   - Used by CMake to fetch the Unity test framework
   - Download from: https://git-scm.com/

## 🏗️ Project Structure

```
the-dreaded-c/
├── CMakeLists.txt          # Build configuration
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
└── build/                  # Build output (created by CMake)
    ├── tictactoe          # Main game executable
    └── run_tests          # Test executable
```

## 🚀 Getting Started

### Step 1: Clone or Download the Project

```bash
cd your-workspace-folder
# Project should be in: the-dreaded-c/
```

### Step 2: Create Build Directory

```bash
cd the-dreaded-c
mkdir build
cd build
```

### Step 3: Configure with CMake

```bash
# Windows (with Visual Studio)
cmake .. -G "Visual Studio 16 2019"

# Windows (with MinGW)
cmake .. -G "MinGW Makefiles"

# Linux/macOS
cmake ..
```

**Note:** This will automatically download the Unity test framework from GitHub.

### Step 4: Build the Project

```bash
# Windows (Visual Studio)
cmake --build . --config Release

# Windows (MinGW) / Linux / macOS
make
```

### Step 5: Run the Game

```bash
# Windows
.\Release\tictactoe.exe
# or
.\tictactoe.exe

# Linux/macOS
./tictactoe
```

### Step 6: Run the Tests

```bash
# Windows
.\run_tests.exe
# or
ctest

# Linux/macOS
./run_tests
# or
make test
```

## 📝 How to Play

1. The game starts with an empty 3x3 board
2. Positions are numbered 0-8 (left-to-right, top-to-bottom):
   ```
      0 | 1 | 2
     -----------
      3 | 4 | 5
     -----------
      6 | 7 | 8
   ```
3. X always goes first
4. Players take turns entering a position number
5. First player to get 3 in a row wins
6. If all 9 positions are filled with no winner, the game is a draw

## 🧪 Understanding the Tests

The project includes 15 comprehensive unit tests covering:

- **Initialization**: Game sets up correctly
- **Validation**: Moves are validated properly
- **Turn Management**: Players alternate correctly
- **Winning Conditions**: All win scenarios work
- **Edge Cases**: Draw detection, null pointer handling

To view test results:
```bash
./run_tests  # or run_tests.exe on Windows
```

## 📚 Learning C Concepts

This project demonstrates:

### 1. **Enums** (in `tictactoe.h`)
```c
typedef enum {
    CELL_EMPTY = 0,
    CELL_X = 1,
    CELL_O = 2
} CellValue;
```
Enums create human-readable constants instead of magic numbers.

### 2. **Structs** (in `tictactoe.h`)
```c
typedef struct {
    CellValue board[9];
    GameStatus status;
    int move_count;
} GameState;
```
Structs group related data together (like a simple class).

### 3. **Arrays**
```c
CellValue board[9];  // Array of 9 elements (0-indexed)
```
Arrays store multiple values of the same type.

### 4. **Pointers**
```c
void game_init(GameState *state)  // * means "pointer to"
```
Pointers let functions modify data passed to them.

### 5. **Static Functions** (in `tictactoe.c`)
```c
static CellValue check_winner(const CellValue board[9])
```
`static` limits visibility to just this file (encapsulation).

### 6. **Const Correctness**
```c
const CellValue *ptr  // Pointer to data that won't change
```

## 🛠️ Development Workflow

### Adding a New Function

1. **Add function declaration in header file** (`include/tictactoe.h`):
   ```c
   int game_get_board_position(GameState *state, int row, int col);
   ```

2. **Implement in source file** (`src/tictactoe.c`):
   ```c
   int game_get_board_position(GameState *state, int row, int col) {
       if (state == NULL || row < 0 || row > 2 || col < 0 || col > 2) {
           return -1;
       }
       return state->board[row * 3 + col];
   }
   ```

3. **Write tests** (`tests/test_tictactoe.c`):
   ```c
   void test_get_board_position(void) {
       GameState game;
       game_init(&game);
       game_make_move(&game, 0);
       
       TEST_ASSERT_EQUAL(CELL_X, game_get_board_position(&game, 0, 0));
   }
   ```

4. **Rebuild and test**:
   ```bash
   cd build
   cmake --build .
   ./run_tests
   ```

## 🐛 Common Issues

### CMake not found
- Install CMake from https://cmake.org/download/
- Add to PATH if needed

### Git not found (during CMake)
- Install Git from https://git-scm.com/
- Or download Unity manually and extract to `external/Unity/`

### No C compiler found
- **Windows with Visual Studio**: Install "Desktop development with C++"
- **Windows with MinGW**: `choco install mingw` (requires Chocolatey)
- **Linux**: `sudo apt-get install build-essential`
- **macOS**: `xcode-select --install`

### Build fails after code changes
```bash
cd build
cmake .  # Reconfigure
cmake --build .  # Rebuild
```

### Tests won't compile
- Ensure `external/Unity/` exists or run CMake to download it
- Check that header paths are correct

## 📖 Recommended Reading

- **K&R C**: "The C Programming Language" - the classic
- **Beej's Guide**: https://beej.us/guide/bgc/ - modern, free online
- **"Clean Code in C"**: Best practices for maintainable code

## 🎯 Next Steps

Try these enhancements:

1. **Difficulty Levels**: Add a computer AI opponent
2. **Game History**: Store and replay previous games
3. **Networking**: Play against someone over the network
4. **GUI**: Create a graphical interface (using SDL or similar)
5. **Statistics**: Track win/loss records

## 📄 License

This project is provided as-is for educational purposes.

## 💡 Tips for Learning

- **Read the tests first** to understand what code should do
- **Use a debugger** (add `-g` flag: `cmake .. -DCMAKE_BUILD_TYPE=Debug`)
- **Print/log everything** to understand program flow
- **Experiment** - try breaking things and fixing them
- **Comment your code** liberally when learning

---

Happy coding! 🎉