# Quick Start Guide

## For Windows Users

### 1. Install Prerequisites (First Time Only)

**Option A: Using Visual Studio (Recommended)**
1. Install Visual Studio Community 2019 or 2022
   - During installation, check "Desktop development with C++"
   - Download: https://visualstudio.microsoft.com/vs/community/

2. Install CMake
   - Download: https://cmake.org/download/
   - Add to PATH during installation

3. Install Git (optional, but recommended)
   - Download: https://git-scm.com/

**Option B: Using MinGW (Lightweight)**
1. Install Chocolatey: https://chocolatey.dev/install
2. Run in PowerShell (as Administrator):
   ```powershell
   choco install mingw cmake git
   ```

### 2. Build the Project

**Double-click `build.bat`** or run in Command Prompt:
```cmd
cd c:\Users\vccan\VSCProjects\the-dreaded-c
build.bat
```

This will:
- Create a `build` directory
- Download the Unity testing framework
- Compile your code
- Place executables in `build\Release\`

### 3. Run the Game

```cmd
cd build
.\Release\tictactoe.exe
```

Or just double-click: `build\Release\tictactoe.exe`

### 4. Run Tests

```cmd
cd build
.\run_tests.exe
```

Expected output:
```
...

Tic-Tac-Toe unit tests
15 Tests 0 Failures 0 Ignored
OK
```

---

## For Linux/macOS Users

### 1. Install Prerequisites (First Time Only)

**Ubuntu/Debian:**
```bash
sudo apt-get update
sudo apt-get install cmake build-essential git
```

**macOS:**
```bash
# Install Xcode Command Line Tools
xcode-select --install

# Or install via Homebrew (if installed)
brew install cmake git
```

**Fedora/RHEL:**
```bash
sudo dnf install cmake gcc git
```

### 2. Build the Project

```bash
cd ~/VSCProjects/the-dreaded-c
chmod +x build.sh          # Make script executable
./build.sh                 # Run build script
```

Or manually:
```bash
mkdir build
cd build
cmake ..
make
```

### 3. Run the Game

```bash
cd build
./tictactoe
```

### 4. Run Tests

```bash
cd build
./run_tests
```

Expected output:
```
...

Tic-Tac-Toe unit tests
15 Tests 0 Failures 0 Ignored
OK
```

---

## Troubleshooting

### "CMake not found"
- Windows: Reinstall CMake and check "Add to PATH"
- Linux: `sudo apt-get install cmake`
- macOS: `brew install cmake`

### "No C compiler found"
- Windows: Install Visual Studio Build Tools
- Linux: `sudo apt-get install build-essential`
- macOS: `xcode-select --install`

### "Git not found (build fails)"
- Windows: Install Git from https://git-scm.com/
- Linux: `sudo apt-get install git`
- macOS: `brew install git`

### Build folder has old stuff (weird errors)
```bash
cd the-dreaded-c
rm -rf build        # Remove old build
mkdir build         # Create fresh build
cd build
cmake ..            # Reconfigure
make                # Rebuild
```

### Still having issues?
1. Check that your compiler is installed: `gcc --version` or `cl.exe`
2. Verify CMake is in PATH: `cmake --version`
3. Read the full README.md for detailed setup instructions
4. Check the Learning Guide: LEARNING_GUIDE.md

---

## Project Navigation in VS Code

### Open the Project
1. File → Open Folder
2. Select: `C:\Users\vccan\VSCProjects\the-dreaded-c`

### Useful Extensions (Optional)
- **C/C++** by Microsoft (for IntelliSense)
- **CMake** by twxs (for CMake syntax highlighting)
- **CMake Tools** by Microsoft (for building in VS Code)

### File Organization
- **include/** - Header files with function declarations
  - `tictactoe.h` - Game logic interface
  - `display.h` - Display function interface
- **src/** - Implementation files
  - `tictactoe.c` - Game logic implementation
  - `display.c` - Display implementation
  - `main.c` - Interactive game program
- **tests/** - Unit tests
  - `test_tictactoe.c` - 15+ tests using Unity framework
- **CMakeLists.txt** - Build configuration
- **README.md** - Full documentation
- **LEARNING_GUIDE.md** - C concepts explained

---

## Making Changes

### After editing source code:

**Windows:**
```cmd
cd build
cmake --build . --config Release
```

**Linux/macOS:**
```bash
cd build
make
```

### Running tests after changes:

**Windows:**
```cmd
.\run_tests.exe
```

**Linux/macOS:**
```bash
./run_tests
```

If tests fail, it means your code broke something! This helps catch bugs.

---

## Next Steps

1. **Play the game** - Make sure it works
2. **Read LEARNING_GUIDE.md** - Understand what the code does
3. **Look at test_tictactoe.c** - See example code patterns
4. **Try modifying** - Make a small change and rebuild
5. **Read the source** - Follow along with your new C knowledge

---

## File Overview

```
the-dreaded-c/
├── build.bat              ← Double-click to build (Windows)
├── build.sh               ← Run ./build.sh (Linux/macOS)
├── CMakeLists.txt         ← Build configuration (don't modify yet)
│
├── include/               ← Headers (what functions exist)
│   ├── tictactoe.h       ← Game functions interface
│   └── display.h         ← Display functions interface
│
├── src/                   ← Implementation (how they work)
│   ├── tictactoe.c       ← Game logic code
│   ├── display.c         ← Display code
│   └── main.c            ← Interactive game
│
├── tests/                 ← Unit tests
│   └── test_tictactoe.c  ← 15+ test cases
│
├── README.md              ← Full documentation
├── LEARNING_GUIDE.md      ← Learn C concepts used here
└── QUICK_START.md         ← This file!
```

---

Happy coding! 🎮
