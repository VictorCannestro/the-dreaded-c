# Continuous Integration & Build System Guide
This guide explains how we use **GitHub Actions** for continuous integration and **Ceedling** as our build system. If you're new to these concepts, this document will help you understand why they're valuable and how they work in our project.

## Table of Contents
1. [What is Continuous Integration (CI)?](#what-is-continuous-integration-ci)
2. [GitHub Actions in Our Project](#github-actions-in-our-project)
3. [Understanding the Workflow File](#understanding-the-workflow-file)
4. [What is Ceedling?](#what-is-ceedling)
5. [Helper Scripts](#helper-scripts)
6. [Why Ceedling Over Other Build Systems?](#why-ceedling-over-other-build-systems)
7. [How It All Works Together](#how-it-all-works-together)
8. [Common Tasks](#common-tasks)

---

## What is Continuous Integration (CI)?
**Continuous Integration (CI)** is a software development practice where code changes are automatically built and tested every time someone pushes to the repository.

### Why Do We Want CI?
| Problem Without CI | Solution With CI |
|--------------------|------------------|
| "It works on my machine!" | Tests run on a clean, consistent environment |
| Bugs discovered late in development | Bugs caught immediately when code is pushed |
| Manual testing is tedious and error-prone | Automated tests run on every change |
| Integration issues pile up | Small changes are validated continuously |
| No confidence in code quality | Green checkmarks give confidence to merge |


### The CI Feedback Loop
```
Developer pushes code
        ↓
CI server detects the push
        ↓
CI runs tests automatically
        ↓
    ┌───┴───┐
    ↓       ↓
  PASS    FAIL
    ↓       ↓
  ✅ ← Developer sees results → ❌
            ↓
      Fix and push again
```

---

## GitHub Actions in Our Project
**GitHub Actions** is GitHub's built-in CI/CD platform. It's free for public repositories and integrates seamlessly with your code.

### Where to Find It
- **Workflow file:** `.github/workflows/build-and-test.yml`
- **Results:** Go to your repository on GitHub → Click "Actions" tab
- **Status badge:** Shows ✅ or ❌ next to each commit


### What Our Workflow Does
Every time you push to `main` or open a pull request:

1. **Checks out your code** on a fresh virtual machine
2. **Sets up Ruby** (required for Ceedling)
3. **Installs GCC** (the C compiler)
4. **Installs Ceedling** (our build system)
5. **Runs all unit tests** (`ceedling test:all`)
6. **Builds the release executable** (`ceedling release`)
7. **Uploads artifacts** (test results and the compiled game)


### Multi-Platform Testing
Our workflow runs on **both Ubuntu and macOS**, ensuring the code works across different operating systems:

```yaml
matrix:
  os: [ubuntu-latest, macos-latest]
```

This catches platform-specific bugs like:
- Different path separators (`/` vs `\`)
- Compiler behavior differences
- Library availability issues

---

## Understanding the Workflow File
Let's break down `.github/workflows/build-and-test.yml`:

```yaml
name: Build and Test    # Name shown in GitHub UI

on:
  push:
    branches: [ "main" ]      # Trigger on pushes to main
  pull_request:
    branches: [ "main" ]      # Trigger on PRs targeting main
```

### Key Sections Explained
#### Triggers (`on:`)
Defines **when** the workflow runs:
- `push` - When code is pushed
- `pull_request` - When a PR is opened or updated

#### Jobs (`jobs:`)
A workflow contains one or more **jobs**. Ours has one job called `build`.

#### Strategy Matrix
```yaml
strategy:
  fail-fast: false    # Don't cancel other jobs if one fails
  matrix:
    os: [ubuntu-latest, macos-latest]
```
This runs the job multiple times with different configurations.

#### Steps
Each step is an action or command:

```yaml
- uses: actions/checkout@v4        # Pre-built action (from marketplace)
- run: ceedling test:all           # Shell command
```

### Artifacts
Artifacts are files saved from the workflow run:

```yaml
- name: Upload Test Results
  uses: actions/upload-artifact@v4
  with:
    name: test-results-${{ matrix.os }}
    path: build/artifacts/test/
```

You can download these from the Actions tab to inspect test results or get the compiled executable.

---

## What is Ceedling?
**Ceedling** is a build system specifically designed for C projects. It handles:

- **Compiling** source files
- **Linking** object files into executables
- **Running unit tests** with the Unity test framework
- **Generating mocks** with CMock (for isolating code under test)
- **Managing dependencies** between files

### Ceedling vs Manual Compilation
**❌ Manual compilation (tedious and error-prone):**
```bash
gcc -c src/board.c -o build/board.o -Iinclude
gcc -c src/tictactoe.c -o build/tictactoe.o -Iinclude
gcc -c src/display.c -o build/display.o -Iinclude
# ... repeat for every file ...
gcc build/*.o -o tictactoe
```

**✅ With Ceedling (one command):**
```bash
ceedling release
```

Ceedling automatically:
- Finds all source files
- Determines compilation order
- Recompiles only changed files
- Links everything together

---

## Helper Scripts
We provide convenience scripts to make working with Ceedling even easier. These scripts wrap common Ceedling commands and add helpful error checking.

### Script Overview
| Script | Platform | Purpose |
|--------|----------|---------|
| `build.sh` | Linux/macOS | One-command full build (tests + release) |
| `build.bat` | Windows | One-command full build (tests + release) |
| `ceedling.sh` | Linux/macOS | Menu of common Ceedling operations |
| `ceedling.bat` | Windows | Menu of common Ceedling operations |

### build.sh / build.bat

These scripts run a **complete build** with a single command. They:

1. Check if Ceedling is installed (and give helpful error messages if not)
2. Run `ceedling build` (which runs tests and builds the release)
3. Print success/failure status with next steps

**Usage:**
```bash
# Linux/macOS
./build.sh

# Windows
build.bat
```

**When to use:** When you want to quickly verify everything works—tests pass and the game compiles.

### ceedling.sh / ceedling.bat

These are **interactive helper scripts** that provide a simple menu of common operations:

| Command | What It Does |
|---------|--------------|
| `build` | Build everything (tests + release) |
| `test` | Run all unit tests |
| `release` | Build only the release executable |
| `clean` | Remove build artifacts |
| `clobber` | Remove everything including downloaded dependencies |
| `help` | Show available commands |

**Usage:**
```bash
# Linux/macOS
./ceedling.sh test       # Run tests
./ceedling.sh release    # Build game
./ceedling.sh clean      # Clean up
./ceedling.sh help       # Show help

# Windows
ceedling.bat test
ceedling.bat release
ceedling.bat clean
ceedling.bat help
```

**When to use:** When you want a quick shortcut without remembering exact Ceedling syntax.


### Why Have These Scripts?
| Benefit             | Explanation                                          |
|---------------------|------------------------------------------------------|
| **Discoverability** | New developers can see available commands via `help` |
| **Error checking**  | Scripts verify Ceedling is installed before running  |
| **Cross-platform**  | `.sh` for Unix, `.bat` for Windows                   |
| **Consistency**     | Everyone runs the same commands the same way         |
| **Documentation**   | Scripts serve as executable documentation            |

### Scripts vs Direct Ceedling Commands

You can always use Ceedling directly—the scripts are just convenience wrappers:

| Script Command          | Equivalent Ceedling Command |
|-------------------------|-----------------------------|
| `./build.sh`            | `ceedling build`            |
| `./ceedling.sh test`    | `ceedling test:all`         |
| `./ceedling.sh release` | `ceedling release`          |
| `./ceedling.sh clean`   | `ceedling clean`            |

---

## Why Ceedling Over Other Build Systems?
### Comparison with Alternatives
| Feature | Ceedling | Make | CMake |
|---------|----------|------|-------|
| Learning curve | Low | Medium | High |
| C-specific | ✅ Yes | No | No |
| Built-in testing | ✅ Unity/CMock | No | Requires setup |
| Configuration | YAML | Makefile syntax | CMake syntax |
| Cross-platform | ✅ Ruby-based | Needs tweaking | ✅ Yes |
| Dependency tracking | ✅ Automatic | Manual rules | Automatic |

### Why Ceedling is Ideal for This Project
1. **Test-First Design**
   - Ceedling was built around unit testing
   - Includes Unity (test framework) and CMock (mocking framework)
   - Running tests is a first-class operation, not an afterthought

2. **Simple Configuration**
   ```yaml
   # project.yml - Human-readable configuration
   :paths:
     :source:
       - src/**
     :include:
       - include/**
   ```

3. **Convention Over Configuration**
   - Test files start with `test_` → automatically discovered
   - Source files in `src/` → automatically compiled
   - Less boilerplate than Make or CMake

4. **Beginner Friendly**
   - No need to understand complex build systems
   - Clear error messages
   - One tool does everything

### When You Might Choose Something Else
- **Large, complex projects**: CMake has more features for managing huge codebases
- **Embedded systems**: Specialized toolchains may need custom Makefiles
- **No Ruby available**: Ceedling requires Ruby to be installed

---

## How It All Works Together
```
┌─────────────────────────────────────────────────────────────┐
│                     Developer's Machine                      │
├─────────────────────────────────────────────────────────────┤
│  1. Write code in src/ and tests in tests/                  │
│  2. Run locally: ceedling test:all                          │
│  3. Commit and push to GitHub                               │
└─────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────┐
│                      GitHub Actions                          │
├─────────────────────────────────────────────────────────────┤
│  1. Detects push/PR                                         │
│  2. Spins up Ubuntu + macOS VMs                             │
│  3. Installs Ruby, GCC, Ceedling                            │
│  4. Runs: ceedling test:all                                 │
│  5. Runs: ceedling release                                  │
│  6. Reports ✅ or ❌                                         │
│  7. Uploads artifacts (executable, test results)            │
└─────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────┐
│                     GitHub Repository                        │
├─────────────────────────────────────────────────────────────┤
│  - Commit shows ✅ or ❌ status                              │
│  - PR blocked from merging if tests fail                    │
│  - Artifacts downloadable from Actions tab                  │
└─────────────────────────────────────────────────────────────┘
```

---

## Common Tasks
### Running Tests Locally
```bash
# Run all tests
ceedling test:all

# Run a specific test file
ceedling test:test_board

# Run tests with verbose output
ceedling test:all --verbosity=obnoxious
```

### Building the Game
```bash
# Build release executable
ceedling release

# Output: build/release/tictactoe.exe
```

### Cleaning Up
```bash
# Remove build artifacts
ceedling clean

# Remove everything (including downloaded dependencies)
ceedling clobber
```

### Checking CI Status
1. Go to your repository on GitHub
2. Click the **Actions** tab
3. See the list of workflow runs
4. Click a run to see details and download artifacts

### Viewing Test Results
Test results are stored in:
- `build/test/results/` - Individual test results (`.pass` or `.fail` files)
- `build/artifacts/test/report.xml` - JUnit-style XML report

---

## Troubleshooting
### "Ceedling is not installed"
Install it with:
```bash
gem install ceedling -v 1.0.1
```

You need Ruby installed first. On macOS: `brew install ruby`

### CI Fails but Local Tests Pass
Common causes:
- **Missing files**: Did you forget to commit a new file?
- **Platform differences**: Does your code assume a specific OS?
- **Environment variables**: Are you relying on local config?

Check the GitHub Actions logs for the exact error.

### Tests Timeout
If tests hang, check for:
- Infinite loops in your code
- Waiting for user input (use mocks in tests!)
- Large data sets that take too long

---

## Further Reading
- [GitHub Actions Documentation](https://docs.github.com/en/actions)
- [Ceedling Documentation](https://github.com/ThrowTheSwitch/Ceedling)
- [Unity Test Framework](https://github.com/ThrowTheSwitch/Unity)
- [CMock Mocking Framework](https://github.com/ThrowTheSwitch/CMock)

