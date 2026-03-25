# Tic-Tac-Toe Level 2 - Modular Implementation
This level takes the naive single-file implementation and splits it into logical modules.

## Table of Contents
- [Building](#building)
- [Code Characteristics](#code-characteristics)
  - [File Structure](#file-structure)
  - [What Works](#what-works)
  - [What's Improved Over Level 1](#whats-improved-over-level-1)
  - [What's Still Missing (for Level 3)](#whats-still-missing-for-level-3)
- [Learning Goals](#learning-goals)
  1. [Understand Separation of Concerns](#1-understand-separation-of-concerns)
  2. [Understand How Structs Enable Testing](#2-understand-how-structs-enable-testing)
  3. [Understand How Enums Prevent Bugs](#3-understand-how-enums-prevent-bugs)
  4. [Understand Data-Driven Design](#4-understand-data-driven-design)
  5. [Understand Header Files as Contracts](#5-understand-header-files-as-contracts)
  6. [Understand Iterative vs Recursive Input](#6-understand-iterative-vs-recursive-input)
  7. [Trace Module Dependencies](#7-trace-module-dependencies)
- [Hands-On Exercises](#hands-on-exercises)
  - [Exercise 1: The Module Mapping](#exercise-1-the-module-mapping)
  - [Exercise 2: Add a Feature Without Breaking Things](#exercise-2-add-a-feature-without-breaking-things)
  - [Exercise 3: The Testing Setup](#exercise-3-the-testing-setup)
  - [Exercise 4: The Extension Challenge](#exercise-4-the-extension-challenge)
- [Summary: Level 1 → Level 2 Transformation](#summary-level-1--level-2-transformation)
- [Summary: Level 2 → Level 3 Improvements Needed](#summary-level-2--level-3-improvements-needed)


## Building
```bash
make
./tictactoe
```

## Code Characteristics
### File Structure
```
level_2/
├── include/
│   ├── types.h     # Enums, structs, constants
│   ├── board.h     # Board operations
│   ├── game.h      # Game state management
│   ├── display.h   # Output functions
│   ├── input.h     # User input handling
│   └── ai.h        # Computer player
├── src/
│   ├── main.c      # Entry point (coordinator)
│   ├── types.c     # Constant definitions
│   ├── board.c     # Board implementation
│   ├── game.c      # Game logic
│   ├── display.c   # Display implementation
│   ├── input.c     # Input implementation
│   └── ai.c        # AI implementation (still random)
├── Makefile
└── README.md
```

### What Works
- ✅ Basic gameplay
- ✅ Human vs Computer
- ✅ Win detection
- ✅ Draw detection
- ✅ Play again option
- ✅ Modular architecture
- ✅ Type-safe enums
- ✅ Encapsulated state

### What's Improved Over Level 1
This level fixes the major problems from Level 1:

1. **Separate Files** — 7 source files + 6 headers instead of 1 monolithic file
2. **GameState Struct** — All state encapsulated, no globals
3. **Enums** — `CELL_X`, `STATUS_DRAW` instead of magic numbers
4. **Data-Driven Win Check** — Loop over `WIN_COMBOS` array instead of 8 copy-pasted conditions
5. **Iterative Input** — `while(1)` loop instead of risky recursion
6. **Input Validation** — `scanf` return value checked, buffer cleared
7. **Header Files** — Clear API contracts separate from implementation

### What's Still Missing (for Level 3)
- ❌ Unit tests
- ❌ Smart AI (minimax algorithm)
- ❌ Winner chooses symbol between games
- ❌ NULL pointer checks / defensive programming
- ❌ Multiple difficulty levels
- ❌ UI abstraction layer (swappable CLI/GUI)
- ❌ Configurable board size
- ❌ Testable win detection helpers


## Learning Goals
After studying this level, you should be able to achieve the following:

### 1. Understand Separation of Concerns
**Goal:** See how splitting code into modules makes it maintainable.

**Exercise:** For each module, identify its ONE responsibility:

| Module      | Responsibility        | What It Does NOT Do         |
|-------------|-----------------------|-----------------------------|
| `board.c`   | Board data structure  | No I/O, no game rules       |
| `game.c`    | Game state management | No display, no input        |
| `display.c` | Output to terminal    | No logic, no state changes  |
| `input.c`   | Read user input       | No game logic, no display   |
| `ai.c`      | Computer decisions    | No I/O, no state management |
| `main.c`    | Coordination          | No implementation details   |

**Question to answer:** If you wanted to replace the terminal display with a GUI, how many files would you change? (Answer: Just `display.c` and `main.c` — game logic untouched!)

**Compare with Level 1:** In Level 1, you'd have to modify almost EVERY line because display was tangled with logic.

**Note:** Level 3 takes this further with a `UserInterface` abstraction layer. You implement a single interface struct and swap implementations without touching `main.c` at all!

---

### 2. Understand How Structs Enable Testing
**Goal:** See why `GameState` makes unit testing possible.

**Level 1 problem:** Global state means tests pollute each other.

**Level 2 solution:** Each test creates its own `GameState`:

```c
// Test 1: X wins with top row
void test_x_wins_top_row() {
    GameState state;              // ← Local state!
    game_init(&state);
    state.board[0] = CELL_X;
    state.board[1] = CELL_X;
    state.board[2] = CELL_X;
    
    CellValue winner = board_check_winner(state.board);
    
    TEST_ASSERT_EQUAL(CELL_X, winner);
    // No cleanup needed — state is local!
}

// Test 2: Empty board has no winner
void test_empty_board_no_winner() {
    GameState state;              // ← Fresh state, not polluted!
    game_init(&state);
    
    CellValue winner = board_check_winner(state.board);
    
    TEST_ASSERT_EQUAL(CELL_EMPTY, winner);  // ✅ Works!
}
```

**Benefits of struct-based state:**
1. **Test isolation** — Each test has its own state
2. **No cleanup** — Local variables automatically go away
3. **Parallel tests** — Multiple tests can run simultaneously
4. **Explicit dependencies** — Functions declare what state they need

---

### 3. Understand How Enums Prevent Bugs
**Goal:** See how enums catch errors that magic numbers miss.

**Level 1 bug that compiles silently:**
```c
winner = 5;  // Compiler says "OK!" — but 5 is meaningless
if (winner == 3) { ... }  // What's 3 again?
```

**Level 2 equivalent that's safer:**
```c
status = STATUS_DRAW;  // Self-documenting
if (status == STATUS_DRAW) { ... }  // Clear meaning
```

**Exercise:** Try adding a new game status (e.g., `STATUS_TIMEOUT`):

In Level 1:
- Search for every `0`, `1`, `2`, `3` in the code
- Hope you find them all
- Hope you pick an unused number

In Level 2:
- Add `STATUS_TIMEOUT = 4` to the enum
- Compiler warns about unhandled cases in `switch` statements
- Done!

---

### 4. Understand Data-Driven Design
**Goal:** See how separating DATA from LOGIC reduces code and bugs.

**Level 1 approach (code-driven):**
```c
// 8 separate if statements — the LOGIC contains the DATA
if (board[0] != '-' && board[0] == board[1] && board[1] == board[2]) { ... }
if (board[3] != '-' && board[3] == board[4] && board[4] == board[5]) { ... }
// ... 6 more
```

**Level 2 approach (data-driven):**
```c
// DATA is separate from LOGIC
static const int WIN_COMBOS[8][3] = {
    {0, 1, 2}, {3, 4, 5}, {6, 7, 8},  // rows
    {0, 3, 6}, {1, 4, 7}, {2, 5, 8},  // columns
    {0, 4, 8}, {2, 4, 6}              // diagonals
};

// LOGIC is generic — works for ANY winning combinations
for (int i = 0; i < 8; i++) {
    int a = WIN_COMBOS[i][0];
    int b = WIN_COMBOS[i][1];
    int c = WIN_COMBOS[i][2];
    if (board[a] != CELL_EMPTY && board[a] == board[b] && board[b] == board[c]) {
        return board[a];
    }
}
```

**Benefits:**

| Aspect                | Level 1                    | Level 2           |
|-----------------------|----------------------------|-------------------|
| Lines of code         | 24                         | 10                |
| Add new win condition | Copy-paste, modify indices | Add row to array  |
| Bug in one condition  | Fix in 8 places            | Fix in 1 place    |
| Change board size     | Rewrite everything         | Update data table |

---

### 5. Understand Header Files as Contracts
**Goal:** See how headers create clear API boundaries.

**Level 1 problem:** Everything is visible. No way to say "this is internal."

**Level 2 solution:** Headers declare the PUBLIC API:

```c
// board.h — THE CONTRACT (what other files can use)
int board_place(CellValue board[], int position, CellValue symbol);
int board_is_valid_move(const CellValue board[], int position);
CellValue board_check_winner(const CellValue board[]);
```

```c
// board.c — THE IMPLEMENTATION (hidden details)
static const int WIN_COMBOS[8][3] = { ... };  // static = PRIVATE!

// Only functions declared in header are accessible from other files
int board_place(CellValue board[], int position, CellValue symbol) {
    // Implementation...
}
```

**The `static` keyword:** Makes `WIN_COMBOS` invisible outside `board.c`. Other files can't accidentally depend on it.

**Exercise:** Look at each `.h` file and note:
- What functions are exposed (public API)
- What's NOT in the header (private implementation)

---

### 6. Understand Iterative vs Recursive Input
**Goal:** See why Level 2's input handling is safer.

**Level 1 (recursive — dangerous):**
```c
int getHumanMove() {
    int move;
    scanf("%d", &move);
    if (move < 0 || move > 8) {
        printf("Invalid!\n");
        return getHumanMove();  // 🚨 Stack grows with each bad input
    }
    return move;
}
```

**Level 2 (iterative — safe):**
```c
int input_get_move(const GameState *state) {
    while (1) {
        int position;
        printf("Enter position (0-8): ");
        
        if (scanf("%d", &position) != 1) {
            printf("Invalid input!\n");
            input_clear_buffer();
            continue;  // ✅ No stack growth
        }
        
        if (board_is_valid_move(state->board, position)) {
            return position;
        }
        printf("Invalid move!\n");
    }
}
```

**Key differences:**

| Aspect            | Recursive (Level 1)       | Iterative (Level 2) |
|-------------------|---------------------------|---------------------|
| Stack usage       | Grows with each bad input | Constant            |
| 10,000 bad inputs | Stack overflow crash      | Works fine          |
| Debugging         | 10,000 stack frames       | Clean stack trace   |
| Memory            | Wasted                    | Efficient           |

---

### 7. Trace Module Dependencies
**Goal:** Understand how modules depend on each other.

**Exercise:** Draw the dependency graph:

```
main.c
   ├── game.h (uses GameState, game_init, game_make_move, etc.)
   ├── display.h (uses display_board, display_result)
   ├── input.h (uses input_get_move, input_get_symbol_choice)
   └── ai.h (uses ai_get_move)

game.c
   └── board.h (uses board_init, board_place, board_check_winner)

board.c
   └── types.h (uses CellValue, BOARD_SIZE)

display.c
   └── types.h (uses GameState, CellValue)

input.c
   ├── types.h (uses GameState, CellValue)
   └── board.h (uses board_is_valid_move)

ai.c
   ├── types.h (uses GameState)
   └── board.h (uses board_is_empty)
```

**Key insight:** Dependencies flow DOWNWARD. `main.c` depends on everything, but `types.h` depends on nothing. This is good architecture!

**Compare with Level 1:** Everything depends on everything (global variables). Circular dependencies everywhere.

---

## Hands-On Exercises
### Exercise 1: The Module Mapping
For each function call in `main.c`, identify which module provides it:

```c
game_init(&state);              // Provided by: ________
display_board(&state);          // Provided by: ________
input_get_move(&state);         // Provided by: ________
game_make_move(&state, move);   // Provided by: ________
ai_get_move(&state);            // Provided by: ________
display_result(state.status);   // Provided by: ________
```

### Exercise 2: Add a Feature Without Breaking Things
Add a "move counter" that prints "Move #3" before each turn:

1. Which file(s) do you modify? (Answer: Only `main.c` or `display.c`)
2. Do you need to change `game.c`? (Answer: No — separation of concerns!)
3. Do you need to recompile everything? (Answer: Only the file you changed)

### Exercise 3: The Testing Setup
Write a pseudo-test for `board_check_winner()`:

```c
void test_diagonal_win() {
    // Step 1: Create a GameState (no globals!)
    GameState state;
    game_init(&state);
    
    // Step 2: Set up the board for a diagonal win
    state.board[0] = ________;
    state.board[4] = ________;
    state.board[8] = ________;
    
    // Step 3: Call the function under test
    CellValue winner = board_check_winner(state.board);
    
    // Step 4: Assert the result
    assert(winner == ________);
}
```

**Why this works in Level 2 but not Level 1:** The board is a parameter, not a global!

### Exercise 4: The Extension Challenge
Add a 4×4 board mode. Count how many lines change:

**Level 1 would require:** 30+ scattered changes (see Level 1 README)

**Level 2 requires:**
- `types.h`: Change `BOARD_SIZE` from `9` to `16` — 1 line
- `board.c`: Update `WIN_COMBOS` array — add new rows
- `display.c`: Update `display_board()` — modify format string
- Everything else: **UNCHANGED**

**Total: ~10 lines in 3 files, vs 30+ lines scattered everywhere.**

---

## Summary: Level 1 → Level 2 Transformation

| Level 1 Problem            | Level 2 Solution            | Why It's Better               |
|----------------------------|-----------------------------|-------------------------------|
| Global `board[9]`          | `GameState.board` member    | Testable, explicit            |
| Magic number `9`           | `#define BOARD_SIZE 9`      | Change once, works everywhere |
| `winner == 3`              | `status == STATUS_DRAW`     | Self-documenting              |
| 8 copy-pasted win checks   | `WIN_COMBOS` array + loop   | DRY, maintainable             |
| Recursive `getHumanMove()` | Iterative `while(1)` loop   | No stack overflow             |
| `main()` does everything   | Separate modules            | Single responsibility         |
| Unchecked `scanf`          | Check return + clear buffer | No undefined behavior         |
| No headers                 | `.h` files define API       | Clear contracts               |


## Summary: Level 2 → Level 3 Improvements Needed

| Level 2 Gap               | Level 3 Solution            | Why It Matters               |
|---------------------------|-----------------------------|------------------------------|
| No tests                  | 100+ unit tests             | Prove correctness            |
| Random AI                 | Minimax algorithm           | Unbeatable computer          |
| No NULL checks            | Defensive programming       | Graceful failures            |
| Hardcoded printf          | `UserInterface` abstraction | Swap CLI/GUI without rewrite |
| Hardcoded board size      | `BOARD_DIM` configurable    | Extend to 4x4, 5x5, etc.     |
| Single win check function | Testable helpers            | `wcc_check_rows()`, etc.     |
| Basic docs                | Full API documentation      | Team maintainability         |
| Simple Makefile           | Ceedling build system       | Professional tooling         |
| Missing features          | Winner chooses symbol       | Full requirements            |

See the [root project README](../../README.md) for details.

