# Software Engineering Practices

This guide covers the software engineering principles applied in the Tic-Tac-Toe project.
It is a companion to [LEARNING_GUIDE.md](language_learning_guides/LEARNING_GUIDE.md), which covers core C language concepts.

## Table of Contents

1. [Separation of Concerns](#separation-of-concerns)
2. [API Design](#api-design)
3. [Defensive Programming](#defensive-programming)
4. [Testing](#testing)
5. [Code Smells and Refactoring](#code-smells-and-refactoring)

---

## Separation of Concerns
**The single most important principle:** Each module should do ONE thing well.

### The Problem: "God Objects"
When one file does everything, you get:
- 🔴 Files with 500+ lines that are hard to navigate
- 🔴 Changing one feature breaks unrelated features
- 🔴 Can't test pieces in isolation
- 🔴 Multiple developers can't work simultaneously

### The Solution: Single Responsibility
Ask: *"If I had to describe what this file does in one sentence, could I?"*

| File                         | One Sentence                                      | Lines |
|------------------------------|---------------------------------------------------|-------|
| `board.c`                    | Manages a NxN array of cells                      | ~30   |
| `win_condition_calculator.c` | Checks if someone won or it's a draw              | ~100  |
| `ai.c`                       | Picks the optimal move (minimax)                  | ~80   |
| `ai_easy.c`                  | Picks a random valid move                         | ~25   |
| `tictactoe.c`                | Coordinates a game session                        | ~170  |
| `ui_cli.c`                   | CLI implementation of UserInterface               | ~200  |
| `ui_interface.h`             | Defines abstract UI interface (function pointers) | ~45   |

If your sentence has "and" in it, you might need to split.

### How We Split This Project
**Step 1: Identify the data**
```c
// What's the core data? A 9-cell board.
CellValue board[9];
```

**Step 2: Identify operations on that data**
```c
// board.c - Pure data operations
void board_init(CellValue board[BOARD_SIZE]);
int board_place(CellValue board[BOARD_SIZE], int position, CellValue symbol);
int board_is_empty(const CellValue board[BOARD_SIZE], int position);
```

**Step 3: Identify algorithms that use the data**
```c
// win_condition_calculator.c - Algorithm, no side effects
CellValue wcc_check_winner(const CellValue board[BOARD_SIZE]);
int wcc_is_draw(const CellValue board[BOARD_SIZE], int move_count);

// Testable helper functions for each direction
CellValue wcc_check_rows(const CellValue board[BOARD_SIZE]);
CellValue wcc_check_columns(const CellValue board[BOARD_SIZE]);
CellValue wcc_check_main_diagonals(const CellValue board[BOARD_SIZE]);
CellValue wcc_check_anti_diagonals(const CellValue board[BOARD_SIZE]);
```

📄 **See [Win Condition Calculator Algorithm](algorithms/WIN_CONDITION_CALCULATOR.md)** for detailed breakdown.

**Step 4: Identify coordination logic**
```c
// tictactoe.c - Calls other modules, manages state
int game_make_move(GameState *state, int position) {
    CellValue current = game_get_current_player(state);
    if (board_place(state->board, position, current) != 0) return -1;
    state->move_count++;
    update_status(state);  // calls wcc_check_winner internally
    return 0;
}
```

**Step 5: Abstract I/O with function pointers**
```c
// ui_interface.h - Define the interface
typedef struct UserInterface {
    void (*display_board)(const GameState *state);
    void (*display_status)(GameStatus status);
    int  (*get_move)(GameState *game);
    // ... more functions
} UserInterface;

// ui_cli.c - CLI implementation
static void cli_display_board(const GameState *state) { /* printf... */ }
static UserInterface cli_interface = {
    .display_board = cli_display_board,
    // ...
};
UserInterface *ui_get_cli_interface(void) { return &cli_interface; }

// main.c - Uses the interface, doesn't know about implementation
UserInterface *ui = ui_get_cli_interface();
ui->display_board(game);  // Could be CLI, GUI, or web!
```

### Dependency Rules
```
┌─────────────────────────────────────────────────┐
│  main.c  (entry point, wires everything)        │
└─────────────────────────────────────────────────┘
         │
         ├─────────────────────────────┐
         ▼                             ▼
┌─────────────────────────┐  ┌─────────────────────────────┐
│  tictactoe.c            │  │  ui_interface.h             │
│  (game coordinator)     │  │  (UI abstraction layer)     │
└─────────────────────────┘  └─────────────────────────────┘
         │                             │
         │                             ▼
         │                   ┌─────────────────────────────┐
         │                   │  ui_cli.c (CLI impl)        │
         │                   │  ui_gui_example.c (GUI)     │
         │                   └─────────────────────────────┘
         │
    ┌────┴─────────────────┐
    ▼         ▼            ▼
┌────────┐ ┌────────┐ ┌────────────────────────────────────┐
│board.c │ │ai.c    │ │  win_condition_calculator.c        │
│(data)  │ │ai_easy │ │  (pure functions, testable helpers)│
└────────┘ └────────┘ └────────────────────────────────────┘
    │           │                      │
    └─────┬─────┴──────────────────────┘
          ▼
┌─────────────────────────────────────────────────┐
│  constants.h  (BOARD_DIM, BOARD_SIZE, types)    │
└─────────────────────────────────────────────────┘
```

**Rule:** Arrows point DOWN only. Lower modules never `#include` higher modules.

**UI Abstraction:** The `UserInterface` struct uses function pointers to allow swapping CLI/GUI implementations without changing game logic.

### Practical Test: Can You Swap?
Good separation means you can swap implementations:

```c
// Easy to add a new AI without touching tictactoe.c
int ai_medium_get_computer_move(GameState *state);  // New file!

// tictactoe.c just needs one more case:
if (state->difficulty == DIFFICULTY_MEDIUM) {
    return ai_medium_get_computer_move(state);
}
```

If adding a feature requires editing many files, your separation might need work.

---

## API Design
An **API** (Application Programming Interface) is the set of functions you expose for others to use. Good API design makes code easy to use correctly and hard to use incorrectly.

### Principle 1: Consistent Naming Convention
Pick a pattern and stick to it:

```c
// Pattern: module_verb_noun or module_verb

// ✅ All game functions start with game_
void game_init_session(GameState *state);
void game_new_game(GameState *state);
int  game_make_move(GameState *state, int position);
int  game_is_over(GameState *state);
int  game_is_valid_move(GameState *state, int position);

// ✅ All board functions start with board_
void board_init(CellValue board[BOARD_SIZE]);
int  board_place(CellValue board[BOARD_SIZE], int position, CellValue symbol);
int  board_is_empty(const CellValue board[BOARD_SIZE], int position);

// ✅ All win condition functions start with wcc_
CellValue wcc_check_winner(const CellValue board[BOARD_SIZE]);
int       wcc_is_draw(const CellValue board[BOARD_SIZE], int move_count);
```

**Benefit:** You can guess function names without looking them up.

### Principle 2: Predictable Return Values
Choose a convention and document it:

| Pattern            | Meaning                      | Example                     |
|--------------------|------------------------------|-----------------------------|
| `int` returns 0/-1 | 0 = success, -1 = error      | `game_make_move()`          |
| `int` returns 0/1  | 0 = false, non-zero = true   | `game_is_over()`            |
| Enum/value return  | The actual result            | `game_get_current_player()` |
| `void` return      | Always succeeds (or asserts) | `board_init()`              |

```c
// Caller knows exactly what to expect:
if (game_make_move(&game, pos) == -1) {
    printf("Move failed\n");
}

if (game_is_over(&game)) {
    printf("Game ended\n");
}

CellValue player = game_get_current_player(&game);  // Always valid
```

### Principle 3: First Parameter is the "Object"
In C, we simulate object-oriented style by passing the struct first:

```c
// The struct being operated on is always first
void game_init_session(GameState *state);
int  game_make_move(GameState *state, int position);
int  game_is_over(GameState *state);

// Makes calling code read naturally:
game_init_session(&game);
game_make_move(&game, 4);
game_is_over(&game);
```

### Principle 4: Minimize Required Knowledge
Each module should require the **minimum** knowledge to use:

```c
// board.c only needs to know about CellValue (a simple enum)
// It does NOT need to know about GameState, Player, or Difficulty
int board_place(CellValue board[BOARD_SIZE], int position, CellValue symbol);

// tictactoe.c knows about GameState and adapts for callers
int game_make_move(GameState *state, int position) {
    CellValue current = game_get_current_player(state);  // Figures out symbol
    return board_place(state->board, position, current);  // Delegates
}
```

**Benefit:** `board.c` can be reused in a completely different game.

### Principle 5: Symmetric Operations
If you can do something, you should be able to undo it or query it:

```c
// Set and get are symmetric
void game_set_difficulty(GameState *state, Difficulty difficulty);
// Missing: Difficulty game_get_difficulty(GameState *state);  // Could add!

// Init and cleanup are symmetric (if cleanup is needed)
void game_init_session(GameState *state);  // Setup
// void game_cleanup(GameState *state);    // Would add if we used malloc
```

### Principle 6: Named Constants for All Literals
```c
// ❌ What does 9 mean? What does 8 mean?
for (int i = 0; i < 9; i++) { ... }
const int combos[8][3] = { ... };

// ✅ Self-documenting
#define BOARD_SIZE     9
#define NUM_WIN_COMBOS 8

for (int i = 0; i < BOARD_SIZE; i++) { ... }
const int combos[NUM_WIN_COMBOS][3] = { ... };
```

### Anti-Pattern: Exposing Internal State
```c
// ❌ BAD: Caller must know internal rules
game.game_count = 0;  // Why? What breaks if I don't?
game_set_human_symbol_choice(&game, choice);
game.game_count = 1;  // Magic incantation

// ✅ GOOD: API handles the complexity
game_set_winner_symbol_choice(&game, choice);  // Just works
```

**Rule:** If callers need to manipulate struct fields directly to use your API, your API is incomplete.

---

## Defensive Programming
Defensive programming means **assuming things will go wrong** and writing code that handles failures gracefully.

### Decision Tree: How Should I Handle This?
```
Is this a PROGRAMMER error (bug)?
    │
    ├── YES → Use assert() — crash immediately in debug
    │         Examples: NULL where API requires valid pointer,
    │                   function called in wrong state
    │
    └── NO → Is this USER/RUNTIME error?
              │
              ├── YES → Return error code — let caller handle
              │         Examples: invalid input, file not found,
              │                   position already occupied
              │
              └── Can't happen → Still check anyway!
                                 Use assert() as documentation
```

### Technique 1: NULL Pointer Guards
**Every public function should check its pointer parameters:**

```c
void game_init_session(GameState *state) {
    if (state == NULL) {
        return;  // Graceful exit — don't crash
    }
    // Safe to dereference state
    state->status = ONGOING;
}
```

**Pattern:** Check at the top, return early.

```c
int game_make_move(GameState *state, int position) {
    // All guards first, all in one place
    if (state == NULL) return -1;
    if (state->status != ONGOING) return -1;
    if (position < 0 || position >= BOARD_SIZE) return -1;
    
    // Now do the actual work
    // ...
}
```

### Technique 2: Assertions for Invariants
Use `assert()` for conditions that **should never be false** if the code is correct:

```c
#include <assert.h>

int ai_get_computer_move(GameState *state) {
    // This function should only be called via game_get_computer_move,
    // which already validated state. If state is NULL, that's a bug.
    assert(state != NULL);
    
    // ... calculate best_move ...
    
    // If the board isn't full, we must find a move. If we don't,
    // there's a bug in our algorithm.
    assert(best_move != -1);
    
    return best_move;
}
```

**Key insight:** Assertions are **removed** in release builds (`-DNDEBUG`), so:
- ✅ Use for internal consistency checks
- ❌ Don't use for user input validation (would disappear in production!)

### Technique 3: Error Return Codes
For recoverable errors, return a status code:

```c
// Convention: 0 = success, -1 = error
int board_place(CellValue board[BOARD_SIZE], int position, CellValue symbol) {
    if (position < 0 || position >= BOARD_SIZE) {
        return -1;  // Out of bounds — caller can retry with valid input
    }
    if (board[position] != CELL_EMPTY) {
        return -1;  // Already occupied — caller can choose another cell
    }
    board[position] = symbol;
    return 0;
}

// Caller handles the error:
if (board_place(board, pos, CELL_X) == -1) {
    printf("Invalid move, try again\n");
}
```

### Technique 4: Const Correctness
Mark read-only data as `const` so the compiler enforces it:

```c
// Promise: this function won't modify the board
CellValue wcc_check_winner(const CellValue board[BOARD_SIZE]) {
    // board[0] = CELL_X;  // ❌ Compiler error!
    return board[0];        // ✅ Reading is fine
}

// Promise: this function won't modify state
void display_board(const GameState *state) {
    // state->status = X_WINS;  // ❌ Compiler error!
    printf("%d", state->status); // ✅ Reading is fine
}
```

**Benefits:**
1. **Documents intent** — readers know what won't change
2. **Compiler-enforced** — mistakes become compile errors
3. **Enables optimizations** — compiler can make assumptions

### Technique 5: Bounds Checking
**C does not check array bounds.** You must do it yourself.

```c
int board_is_empty(const CellValue board[BOARD_SIZE], int position) {
    // ⚠️ No bounds check! Caller must pass valid index.
    // This is OK because it's an internal function and the
    // public API (board_is_valid_move) does the check.
    return board[position] == CELL_EMPTY;
}

int board_is_valid_move(const CellValue board[BOARD_SIZE], int position) {
    // ✅ Public function: validate first
    if (position < 0 || position >= BOARD_SIZE) {
        return 0;  // Invalid
    }
    return board_is_empty(board, position);
}
```

**Rule:** Public functions validate; private functions can trust their callers.

### Technique 6: Fail Fast, Fail Loud
**❌ Silent failure (bad):**
```c
int ai_get_computer_move(GameState *state) {
    if (state == NULL) {
        return 4;  // Silently return center — masks bugs!
    }
}
```

**✅ Loud failure (good):**
```c
int ai_get_computer_move(GameState *state) {
    assert(state != NULL);  // Crash immediately — bug is obvious
}
```

**Principle:** Bugs should be **painful** during development so they get fixed, not hidden.

---

## Testing
### Why Write Tests?

| Benefit       | Without Tests            | With Tests           |
|---------------|--------------------------|----------------------|
| Finding bugs  | Discover in production   | Discover immediately |
| Changing code | "Did I break something?" | Run tests and know   |
| Documentation | Read the implementation  | Read the test names  |
| Onboarding    | "How does this work?"    | "Look at the tests"  |

### The Testing Mindset
**Don't ask:** "Does my code work?"  
**Ask:** "What are all the ways my code could fail?"

```c
// For board_place(), ask:
// - What if position is negative?
// - What if position is 9 (one past the end)?
// - What if the cell is already occupied?
// - What if I pass CELL_EMPTY as the symbol?
// - Does it actually write the value?
```

### Test Structure: Arrange-Act-Assert
Every test follows this pattern:

```c
void test_board_place_writes_symbol_to_cell(void) {
    // ARRANGE: Set up the test scenario
    CellValue board[BOARD_SIZE];
    board_init(board);

    // ACT: Do the thing being tested
    int result = board_place(board, 4, CELL_X);

    // ASSERT: Verify the results
    TEST_ASSERT_EQUAL(0, result);           // Return value
    TEST_ASSERT_EQUAL(CELL_X, board[4]);    // Side effect
}
```

### Test Naming Convention
Name tests so you can read them like a sentence:

```c
// Pattern: test_<function>_<scenario>_<expected>
void test_board_place_on_occupied_cell_returns_error(void);
void test_board_place_with_negative_position_returns_error(void);
void test_game_make_move_after_game_over_is_rejected(void);
void test_x_wins_when_completing_top_row(void);
```

**When a test fails, the name tells you exactly what broke.**

### What to Test: The Testing Pyramid
```
        /\
       /  \  Edge cases (weird inputs, boundaries)
      /----\
     /      \  Error cases (what should fail)
    /--------\
   /          \  Happy path (normal usage)
  /------------\
```

**1. Happy Path** — Does it work when used correctly?
```c
void test_board_place_writes_symbol_to_cell(void) {
    CellValue board[BOARD_SIZE];
    board_init(board);
    
    int result = board_place(board, 0, CELL_X);
    
    TEST_ASSERT_EQUAL(0, result);
    TEST_ASSERT_EQUAL(CELL_X, board[0]);
}
```

**2. Error Cases** — Does it reject invalid input?
```c
void test_board_place_returns_error_on_negative_position(void) {
    CellValue board[BOARD_SIZE];
    board_init(board);
    
    TEST_ASSERT_EQUAL(-1, board_place(board, -1, CELL_X));
}

void test_board_place_returns_error_on_occupied_cell(void) {
    CellValue board[BOARD_SIZE];
    board_init(board);
    board_place(board, 0, CELL_X);
    
    TEST_ASSERT_EQUAL(-1, board_place(board, 0, CELL_O));
}
```

**3. Edge Cases** — Does it handle boundaries correctly?
```c
void test_board_place_at_last_valid_position(void) {
    CellValue board[BOARD_SIZE];
    board_init(board);
    
    TEST_ASSERT_EQUAL(0, board_place(board, BOARD_SIZE - 1, CELL_X));
}

void test_board_place_at_first_invalid_position(void) {
    CellValue board[BOARD_SIZE];
    board_init(board);
    
    TEST_ASSERT_EQUAL(-1, board_place(board, BOARD_SIZE, CELL_X));
}
```

**4. State Preservation** — Does failure leave things unchanged?
```c
void test_board_place_does_not_modify_board_on_failure(void) {
    CellValue board[BOARD_SIZE];
    board_init(board);
    
    board_place(board, 100, CELL_X);  // Invalid position
    
    // Board should be completely unchanged
    for (int i = 0; i < BOARD_SIZE; i++) {
        TEST_ASSERT_EQUAL(CELL_EMPTY, board[i]);
    }
}
```

### Test Coverage in This Project

| Test File                         | Tests   | What's Covered                                     |
|-----------------------------------|---------|----------------------------------------------------|
| `test_board.c`                    | 12      | init, place, is_empty, is_valid_move               |
| `test_win_condition_calculator.c` | 36      | rows, columns, diagonals (testable helpers), draws |
| `test_tictactoe.c`                | 58      | All API functions, game flow, AI behavior          |
| **Total**                         | **106** |                                                    |

### Running Tests
```bash
# Run all tests
ceedling test:all

# Expected output:
# TESTED:  106
# PASSED:  106
# FAILED:   0
```

### When Tests Fail
A failing test tells you:
1. **What** broke (the test name)
2. **Where** it broke (file and line number)
3. **Expected vs Actual** values

```
test_board_place_writes_symbol_to_cell: FAIL
  Expected: CELL_X (1)
  Actual:   CELL_EMPTY (0)
  at test_board.c:45
```

**Don't change the test to make it pass.** Fix the code!

---

## Code Smells and Refactoring
A **code smell** is a pattern that suggests something might be wrong. Not necessarily a bug, but a sign of deeper problems.

### How to Spot Smells

| Symptom                          | Possible Smell                       |
|----------------------------------|--------------------------------------|
| Function > 50 lines              | God function                         |
| File > 300 lines                 | Missing separation of concerns       |
| Copy-pasted code                 | Missing abstraction                  |
| Lots of `if/else` chains         | Missing polymorphism or lookup table |
| Magic numbers everywhere         | Missing named constants              |
| Magic strings everywhere         | Missing string constants             |
| Caller manipulates struct fields | Incomplete API                       |
| Comments explaining tricky code  | Code should be clearer               |

### Smell 1: Magic Numbers
**Symptom:** Unexplained numeric literals

```c
// ❌ What do these numbers mean?
for (int i = 0; i < 9; i++) {
    if (board[i] == 0) { ... }
}
if (combos[7][2] == position) { ... }
```

**Fix:** Extract named constants

```c
// ✅ Self-documenting
#define BOARD_SIZE     9
#define NUM_WIN_COMBOS 8
#define CELL_EMPTY     0

for (int i = 0; i < BOARD_SIZE; i++) {
    if (board[i] == CELL_EMPTY) { ... }
}
```

**Why it matters:** When you need to change the board size, you change ONE place, not 47.

### Smell 2: Magic Strings
**Symptom:** Unexplained string literals scattered throughout code

```c
// ❌ What if we want to change the error message format?
printf("Invalid input. Please enter a number between 0 and %d.\n", max);
// ... later in the same file ...
printf("Invalid input. Please enter X or O.\n");
// ... and more scattered printf calls with similar strings ...
```

**Fix:** Extract named string constants at the top of the file

```c
// ✅ All strings in one place, categorized by purpose
static const char *ERR_INVALID_NUMBER   = "Invalid input. Please enter a number between 0 and %d.\n";
static const char *ERR_INVALID_INPUT_XO = "Invalid input. Please enter X or O.\n";
static const char *ERR_INVALID_MOVE     = "Invalid move! Position %d is already occupied or out of range.\n";

static const char *MSG_X_WINS           = "*** Player X WINS! ***\n";
static const char *MSG_O_WINS           = "*** Player O WINS! ***\n";
static const char *MSG_DRAW             = "*** GAME DRAW ***\n";

static const char *BOARD_CELL_PADDING   = "   ";
static const char *BOARD_CELL_SEPARATOR = "|";

// Usage:
printf(ERR_INVALID_NUMBER, max);
printf("%s", BOARD_CELL_SEPARATOR);
```

**Why it matters:**
- **Single source of truth:** Change a message in ONE place
- **Self-documenting:** `ERR_INVALID_MOVE` explains what it is
- **Categorization:** Prefixes (`ERR_`, `MSG_`, `PROMPT_`) group related strings
- **Localization-ready:** Easy to swap strings for different languages
- **Typo prevention:** Compiler catches misspelled constant names

**Naming conventions used in this project:**
| Prefix | Purpose | Example |
|--------|---------|---------|
| `MSG_` | Status/result messages | `MSG_X_WINS`, `MSG_DRAW` |
| `PROMPT_` | User input prompts | `PROMPT_POSITION`, `PROMPT_DIFFICULTY` |
| `ERR_` | Error messages | `ERR_INVALID_MOVE`, `ERR_INVALID_NUMBER` |
| `BOARD_` | Board display elements | `BOARD_CELL_PADDING`, `BOARD_ROW_DIVIDER` |

### Smell 3: God Function
**Symptom:** A function that does too many things

```c
// ❌ This function: validates, places, checks wins, checks draws, updates status
int game_make_move(GameState *state, int position) {
    // 50+ lines of mixed responsibilities
}
```

**Fix:** Extract helper functions and delegate

```c
// ✅ Each function does one thing
int game_make_move(GameState *state, int position) {
    if (state == NULL || state->status != ONGOING) return -1;
    
    CellValue current = game_get_current_player(state);      // Query
    if (board_place(state->board, position, current) != 0) { // Delegate
        return -1;
    }
    
    state->move_count++;
    update_status(state);  // Delegate
    return 0;
}

static void update_status(GameState *state) {
    CellValue winner = wcc_check_winner(state->board);  // Delegate
    if (winner == CELL_X) {
        state->status = X_WINS;
        state->last_winner = CELL_X;
    } else if (winner == CELL_O) {
        state->status = O_WINS;
        state->last_winner = CELL_O;
    } else if (wcc_is_draw(state->board, state->move_count)) {
        state->status = DRAW;
    }
}
```

### Smell 4: Leaking Abstraction
**Symptom:** Caller has to know internal rules to use the API

```c
// ❌ Caller must manipulate internal state to make API work
game.game_count = 0;  // Magic ritual #1
game_set_human_symbol_choice(&game, choice);
game.game_count = 1;  // Magic ritual #2
```

**Fix:** Provide a proper API function

```c
// ✅ New function that handles the complexity internally
void game_set_winner_symbol_choice(GameState *state, CellValue winner_symbol) {
    if (state == NULL) return;
    // No game_count check — can be called anytime
    // ... set player types ...
}

// Caller just uses it
game_set_winner_symbol_choice(&game, choice);
```

**Rule:** If callers need to touch struct fields directly, your API is incomplete.

### Smell 5: Duplicated Logic
**Symptom:** Same code pattern appears in multiple places

```c
// ❌ Win-checking logic duplicated in tictactoe.c AND ai.c
static CellValue check_winner(const CellValue board[9]) {
    const int combos[8][3] = { ... };
    for (int i = 0; i < 8; i++) {
        // ... same logic ...
    }
}
```

**Fix:** Extract to a single module

```c
// ✅ win_condition_calculator.c owns this logic
CellValue wcc_check_winner(const CellValue board[BOARD_SIZE]);

// Everyone else calls it:
// tictactoe.c
CellValue winner = wcc_check_winner(state->board);

// ai.c
CellValue winner = wcc_check_winner(board);
```

### Smell 6: Silent Failure
**Symptom:** Errors are hidden instead of reported

```c
// ❌ Returns a plausible value instead of signaling failure
int ai_get_computer_move(GameState *state) {
    if (state == NULL) {
        return 4;  // Center — looks valid, masks the bug
    }
}
```

**Fix:** Fail loudly in development

```c
// ✅ Bug is immediately visible
int ai_get_computer_move(GameState *state) {
    assert(state != NULL);  // Crash with stack trace
    // ...
}
```

**Principle:** Make bugs painful during development so they get fixed.

### Smell 6: Obsolete Code
**Symptom:** Unused constants, functions, or includes

```c
// ❌ This constant was added, used, then the usage was removed
#define BOARD_CENTER 4  // Nobody uses this anymore

// ❌ This include isn't needed
#include <stdint.h>  // No int32_t or uint8_t used
```

**Fix:** Delete it!

```bash
# Find unused code
grep -r "BOARD_CENTER" src/ include/  # If no results, delete it
```

### Smell 7: Inconsistent Style
**Symptom:** Mixed conventions in the same codebase

```c
// ❌ Some comments use //, some use /* */
int x = 5;  // inline comment
/* block comment */
int y = 10;

// ❌ Some functions use () for no params, some use (void)
int newline();      // Deprecated style
int get_count(void); // Correct style
```

**Fix:** Pick one convention and apply everywhere

```c
// ✅ Consistent style throughout
int newline(void);
int get_count(void);

/* All comments use the same style */
```

### Refactoring Checklist
Before committing changes, ask:

- [ ] Did I remove any magic numbers?
- [ ] Is each function < 50 lines?
- [ ] Is each file < 300 lines?
- [ ] Did I eliminate any copy-pasted code?
- [ ] Do tests still pass?
- [ ] Did I remove any dead code?
- [ ] Is the naming consistent?

---

