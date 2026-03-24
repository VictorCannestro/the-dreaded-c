# Tic-Tac-Toe Skill Levels
This directory contains multiple skill levels of the same Tic-Tac-Toe game, progressing from a naive implementation to professional-quality code. Each level is **self-contained and buildable**, allowing you to compile and run any version independently.

## Purpose
This progression demonstrates how software engineering practices evolve as developers gain experience:

| Stage            | Mindset             | Result                        |
|------------------|---------------------|-------------------------------|
| **Beginner**     | "Just make it work" | Code that runs but is fragile |
| **Intermediate** | "Make it organized" | Code that's maintainable      |
| **Professional** | "Make it reliable"  | Code that's tested and robust |

By studying each level, you'll understand *why* best practices exist—not just *what* they are.

---

## Skill Level Overview

| Level   | Name               | Focus                                | AI      | Tests    | Lines of Code    |
|---------|--------------------|--------------------------------------|---------|----------|------------------|
| Level 1 | Naive              | Single file, magic numbers, globals  | Random  | None     | ~200 (1 file)    |
| Level 2 | Modular            | Separate files, enums, structs       | Random  | None     | ~350 (14 files)  |
| Level 3 | _(Root Directory)_ | Full features, minimax AI, 80+ tests | Minimax | Ceedling | ~800 (20+ files) |

---

## Learning Path
### 🔴 Level 1 - "The First Attempt"

> *"I just learned C and made it work!"*

A working game that a new C programmer might write. Everything in one file, global variables, magic numbers, and duplicated code.

**What to notice:**
- All 200 lines in one file — no organization
- Global `board[9]`, `currentPlayer`, `gameOver` — state scattered everywhere
- Win checking repeats the same pattern 8 times — copy-paste programming
- Recursive input handling — stack overflow risk on bad input
- No separation between display and logic — impossible to reuse or test
- Magic numbers like `0`, `1`, `2`, `3` — what do they mean?

**Anti-patterns demonstrated:**

| Anti-Pattern        | Example in Level 1                                    |
|---------------------|-------------------------------------------------------|
| Global State        | `char board[9]` at file scope                         |
| Magic Numbers       | `if (winner == 3)` instead of `if (status == DRAW)`   |
| Copy-Paste Code     | 8 nearly-identical win-check conditions               |
| God Function        | `main()` handles input, logic, display, and game loop |
| Primitive Obsession | Using `char` and `int` instead of meaningful types    |

**Build:** `cd level_1 && make && ./tictactoe`

---

### 🟡 Level 2 - "Learning to Organize"
> *"I learned about modules, structs, and enums!"*

The same game split into logical modules. Introduces enums, structs, and data-driven design.

**Key improvements:**
- 7 source files with clear responsibilities — separation of concerns
- `GameState` struct instead of globals — encapsulated state
- Enums: `CELL_X`, `STATUS_ONGOING`, `PLAYER_HUMAN` — self-documenting code
- Win combos in a data table (loop vs copy-paste) — DRY principle
- Iterative input (no stack overflow) — defensive programming begins

**What changes between Level 1 → Level 2:**

| Aspect    | Level 1            | Level 2                         | Why It Matters                   |
|-----------|--------------------|---------------------------------|----------------------------------|
| State     | 4 global variables | 1 `GameState` struct            | Testable, no hidden dependencies |
| Types     | `char`, `int`      | `CellValue`, `GameStatus` enums | Compiler catches errors          |
| Win logic | 24 lines, 8 copies | 10 lines, 1 loop + data         | Fix once, works everywhere       |
| Input     | Recursive          | Iterative `while(1)`            | No stack overflow                |
| Files     | 1                  | 14 (7 `.c` + 6 `.h` + main)     | Each file has one job            |

**Build:** `cd level_2 && make && ./tictactoe`

---

### 🟢 Level 3 - Best Practice (Root Directory)
> *"I write code that others can trust."*

The full implementation with:
- 10 focused modules — clean architecture
- Minimax AI with difficulty levels — algorithmic sophistication
- 80+ unit tests (100% pass rate) — confidence in correctness
- Comprehensive documentation — maintainable by others
- Ceedling build system — professional tooling
- NULL checks and assertions — defensive programming
- Full requirements compliance — winner chooses symbol, etc.

**What changes between Level 2 → Level 3:**

| Aspect  | Level 2         | Level 3                 | Why It Matters                |
|---------|-----------------|-------------------------|-------------------------------|
| Testing | None            | 80+ unit tests          | Catch bugs before users do    |
| AI      | Random moves    | Minimax (unbeatable)    | Real algorithm implementation |
| Safety  | Minimal         | NULL checks, assertions | Fails fast with clear errors  |
| Docs    | Basic README    | Full API docs, guides   | Team can maintain it          |
| Build   | Simple Makefile | Ceedling + CI ready     | Scales to real projects       |

**Build:** `cd .. && ceedling release && ./build/release/tictactoe.exe`

---

## Comparison Matrix

| Feature               | Level 1 | Level 2  | Level 3  |
|-----------------------|---------|----------|----------|
| Separate files        | ❌       | ✅        | ✅        |
| Enums/constants       | ❌       | ✅        | ✅        |
| GameState struct      | ❌       | ✅        | ✅        |
| Data-driven win check | ❌       | ✅        | ✅        |
| NULL checks           | ❌       | ❌        | ✅        |
| Unit tests            | ❌       | ❌        | ✅        |
| Smart AI (minimax)    | ❌       | ❌        | ✅        |
| Difficulty levels     | ❌       | ❌        | ✅        |
| Winner picks symbol   | ❌       | ❌        | ✅        |
| API documentation     | ❌       | Partial  | ✅        |
| Build system          | Basic   | Makefile | Ceedling |
| CI/CD ready           | ❌       | ❌        | ✅        |

---

## Code Smells Deep Dive
### What's Wrong with Level 1?
#### 1. Magic Numbers
```c
// Level 1: What does this mean?
if (winner == 3) { printf("Draw!\n"); }

// Level 2+: Self-documenting
if (status == STATUS_DRAW) { printf("Draw!\n"); }
```
**Problem:** Six months later, you won't remember what `3` means.

#### 2. Global State
```c
// Level 1: Any function can modify these
char board[9];
int gameOver;

// Level 2+: State is explicit
void game_make_move(GameState *state, int position);
```
**Problem:** Debugging requires tracing ALL code. Tests interfere with each other.

#### 3. God Function
```c
// Level 1: main() is 80+ lines doing everything
int main() {
    // welcome message
    // get player choice  
    // game loop
    // display board
    // get input
    // check winner
    // show result
    // play again?
}

// Level 2+: main() coordinates small functions
int main() {
    game_init(&state);
    while (!game_is_over(&state)) {
        display_board(&state);
        move = get_move(&state);
        game_make_move(&state, move);
    }
    display_result(state.status);
}
```
**Problem:** Can't test, can't reuse, can't understand at a glance.

#### 4. Duplicated Code
```c
// Level 1: Same pattern 8 times
if (board[0] != '-' && board[0] == board[1] && board[1] == board[2]) ...
if (board[3] != '-' && board[3] == board[4] && board[4] == board[5]) ...
// ... 6 more

// Level 2+: Define data, loop once
static const int WINS[8][3] = {{0,1,2}, {3,4,5}, ...};
for (int i = 0; i < 8; i++) { /* check WINS[i] */ }
```
**Problem:** Bug in one copy? Fix it 8 times. Miss one? Bug persists.

#### 5. Mixed Concerns
```c
// Level 1: Logic and display intertwined
if (board[0] == board[1] && board[1] == board[2]) {
    printf("Winner!\n");  // Display in logic function!
    return 1;
}

// Level 2+: Separation
CellValue winner = check_winner(board);  // Pure logic
display_result(winner);                   // Pure display
```
**Problem:** Want a GUI instead of terminal? Rewrite everything.

---

## Exercises
### After Level 1:
1. **Count duplication** — How many times does similar win-check code appear?
2. **Try to extend** — Add a 4th row winning condition. How many lines change?
3. **Try to test** — Write a test for `checkWinner()`. Why is it impossible?
4. **Find the bug risk** — What happens if user enters "hello" instead of a number?

### After Level 2:
1. **Add new AI** — Create `ai_smart.c` that picks center first. How many other files change? (Answer: zero)
2. **Change display** — Make the board show 1-9 instead of 0-8. How many files change? (Answer: one)
3. **Simulate testing** — Notice how `GameState *state` could be created in a test with known values

### With Level 3:
1. **Run tests** — Execute `ceedling test:all` and see 80+ tests pass
2. **Break something** — Change `CELL_X = 1` to `CELL_X = 5`. Which tests catch it?
3. **Read test names** — Tests like `test_game_make_move_rejects_occupied_cell` document behavior
4. **Trace a feature** — Follow how "winner picks symbol" is implemented across modules

---

## Key Takeaways
### Level 1 → Level 2: Organization
- **Enums** replace magic numbers
- **Structs** replace scattered globals  
- **Modules** replace monolithic files
- **Data tables** replace copy-paste code

### Level 2 → Level 3: Reliability
- **Tests** prove code works
- **Assertions** catch programmer errors
- **NULL checks** prevent crashes
- **Documentation** enables teamwork

### The Real Lesson
> "Working code" is the minimum bar. Professional code is **understandable**, **testable**, and **maintainable**.

---

## Directory Structure

```
skill_levels/
├── README.md           # This file
├── level_1/
│   ├── tictactoe.c     # Everything in one file (~200 lines)
│   ├── Makefile        # Simple: gcc -o tictactoe tictactoe.c
│   └── README.md       # Documents the anti-patterns
└── level_2/
    ├── include/        # Header files (API contracts)
    │   ├── types.h     # Enums and structs
    │   ├── board.h     # Board operations
    │   ├── game.h      # Game state management
    │   ├── display.h   # Output functions
    │   ├── input.h     # User input
    │   └── ai.h        # Computer player
    ├── src/            # Implementation files
    │   ├── main.c      # Thin coordinator
    │   ├── board.c     # Board logic
    │   ├── game.c      # Game flow
    │   ├── display.c   # Printf calls
    │   ├── input.c     # Scanf + validation
    │   └── ai.c        # Random moves
    ├── Makefile        # Compiles all sources
    └── README.md       # Explains improvements + Level 1 problems
```

Level 3 (best-practice implementation) is in the root project directory (parent of `skill_levels/`).

---

## Further Reading

- [Level 2 README](level_2/README.md) — Deep dive into why Level 1 practices are problematic
- [Learning Guide](../docs/LEARNING_GUIDE.md) — Core C concepts explained
- [Requirements](../docs/REQUIREMENTS.md) — Full game specification
- [Minimax Algorithm](../docs/algorithms/MINIMAX_ALGORITHM.md) — How the unbeatable AI works

