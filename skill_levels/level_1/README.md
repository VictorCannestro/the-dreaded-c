# Tic-Tac-Toe Level 1 - Naive Implementation
This level represents the "first attempt" code that a new programmer might write.

## Table of Contents
- [Building](#building)
- [Code Characteristics](#code-characteristics)
  - [File Structure](#file-structure)
  - [What Works](#what-works)
  - [Problems to Identify](#problems-to-identify)
- [Learning Goals](#learning-goals)
  1. [Identify Separation of Concerns](#1-identify-separation-of-concerns)
  2. [Understand Why Global Variables Make Testing Hard](#2-understand-why-global-variables-make-testing-hard)
  3. [Understand How Magic Numbers Reduce Readability](#3-understand-how-magic-numbers-reduce-readability)
  4. [Recognize Duplicated Code Patterns](#4-recognize-duplicated-code-patterns)
  5. [Spot Dangerous Patterns](#5-spot-dangerous-patterns)
  6. [Understand the "God Function" Anti-Pattern](#6-understand-the-god-function-anti-pattern)
  7. [Trace Hidden Dependencies](#7-trace-hidden-dependencies)
- [Hands-On Exercises](#hands-on-exercises)
  - [Exercise 1: The Annotation Challenge](#exercise-1-the-annotation-challenge)
  - [Exercise 2: The Bug Hunt](#exercise-2-the-bug-hunt)
  - [Exercise 3: The Extension Challenge](#exercise-3-the-extension-challenge)
  - [Exercise 4: The Impossible Test](#exercise-4-the-impossible-test)
- [Summary: Level 1 → Level 2 Transformation](#summary-level-1--level-2-transformation)


## Building
```bash
# Using GCC directly (no build system)
gcc -o tictactoe tictactoe.c -Wall

# Or use the Makefile
make
./tictactoe
```

## Code Characteristics
### File Structure
```
level_1/
├── README.md       # This file
├── tictactoe.c     # Everything in one file (~200 lines)
└── Makefile        # Simple: gcc -o tictactoe tictactoe.c
```

### What Works
- ✅ Basic gameplay
- ✅ Human vs Computer
- ✅ Win detection
- ✅ Draw detection
- ✅ Play again option

### Problems to Identify
This level intentionally contains common beginner mistakes and code smells:

1. **Single File** - All 200+ lines in one file
2. **Global Variables** - `board`, `currentPlayer`, `gameOver`, `winner`
3. **Magic Numbers** - `9` for board size, `0-3` for game status, `'-'` for empty
4. **No Enums** - Using chars and ints instead of meaningful types
5. **No Header Files** - No API separation
6. **No Structs** - Game state spread across globals
7. **Duplicated Logic** - Win checking repeats similar patterns 8 times
8. **Recursive Input** - `getHumanMove()` calls itself (stack overflow risk)
9. **Mixed Concerns** - I/O mixed with game logic
10. **No Error Handling** - `scanf` not checked
11. **Simple AI** - Just random moves
12. **Incomplete Requirements** - Winner doesn't choose symbol


## Learning Goals
After studying this level, you should be able to achieve the following:

### 1. Identify Separation of Concerns
**Goal:** Recognize which parts of the code are "game logic" vs "display" vs "input."

**Exercise:** Go through `tictactoe.c` and highlight or annotate:
- 🔵 **Blue** — Lines that print to the screen (`printf`)
- 🟢 **Green** — Lines that read user input (`scanf`)
- 🟡 **Yellow** — Lines that implement game rules (win checking, turn switching)
- 🔴 **Red** — Lines that manage state (`board`, `currentPlayer`, etc.)

**What you'll discover:** These colors are interleaved throughout the code. In Level 2, each color becomes its own file.

**Question to answer:** If you wanted to replace the terminal display with a graphical UI, how many lines would you need to change? (Answer: Almost all of them, because display is tangled with logic.)

---

### 2. Understand Why Global Variables Make Testing Hard
**Goal:** See why `board[9]`, `currentPlayer`, and other globals prevent unit testing.

**Thought experiment:** Imagine writing a test for `checkWinner()`:

```c
// Test: X wins with top row
void test_x_wins_top_row() {
    // Setup: Need to set board to a known state
    board[0] = 'X'; board[1] = 'X'; board[2] = 'X';  // ← Modifying global!
    
    int result = checkWinner();
    
    assert(result == 1);  // X wins
    
    // Cleanup: Must reset for next test!
    resetBoard();  // ← What if we forget this?
}

// Test: Empty board has no winner
void test_empty_board_no_winner() {
    // If previous test didn't clean up, this FAILS!
    int result = checkWinner();
    assert(result == 0);  // ← FAILS because board still has X's!
}
```

**Problems with globals:**
1. **Test pollution** — Tests affect each other's state
2. **Setup burden** — Every test must initialize ALL globals
3. **Cleanup burden** — Every test must reset ALL globals
4. **Hidden dependencies** — Functions secretly depend on global state
5. **No parallelism** — Can't run tests simultaneously

**Level 2 solution:** Pass a `GameState *state` parameter. Each test creates its own state.

---

### 3. Understand How Magic Numbers Reduce Readability
**Goal:** Experience the confusion of reading code with unexplained numeric literals.

**Exercise:** Without looking at comments, explain what each value means:

```c
if (winner == 0) { ... }   // What does 0 mean?
if (winner == 1) { ... }   // What does 1 mean?
if (winner == 2) { ... }   // What does 2 mean?  
if (winner == 3) { ... }   // What does 3 mean?

if (board[4] == '-') { ... }  // Why 4? What's '-'?

for (i = 0; i < 9; i++) { ... }  // Why 9?
```

**The problem:** You must memorize or constantly refer back to understand the code.

**Now compare with Level 2:**
```c
if (status == STATUS_ONGOING) { ... }   // Self-documenting!
if (status == STATUS_X_WINS) { ... }
if (status == STATUS_O_WINS) { ... }
if (status == STATUS_DRAW) { ... }

if (board[CENTER] == CELL_EMPTY) { ... }

for (i = 0; i < BOARD_SIZE; i++) { ... }
```

**Key insight:** Good code reads like prose. Magic numbers are like writing a novel where every character is referred to by their Social Security Number.

---

### 4. Recognize Duplicated Code Patterns
**Goal:** Spot the copy-paste pattern in win checking and understand its costs.

**Exercise:** Find the `checkWinner()` function and answer:

1. How many `if` statements check for a winner? (Answer: 8)
2. What's the pattern? (Answer: Check if 3 cells are equal and not empty)
3. How similar are lines 1-3 vs lines 4-6? (Answer: Nearly identical, just different indices)

**The duplication:**
```c
// Row 1
if (board[0] != '-' && board[0] == board[1] && board[1] == board[2]) { ... }
// Row 2  
if (board[3] != '-' && board[3] == board[4] && board[4] == board[5]) { ... }
// Row 3
if (board[6] != '-' && board[6] == board[7] && board[7] == board[8]) { ... }
// ... and 5 more similar lines
```

**Costs of duplication:**

| Problem          | Impact                                         |
|------------------|------------------------------------------------|
| Bug in one line? | Must fix in 7 other places                     |
| Typo in indices? | Subtle bug that's hard to spot in review       |
| Want 4×4 board?  | Rewrite all 10 winning conditions              |
| Code review?     | Reviewer must verify 8 nearly-identical blocks |

**Level 2 solution:** Define winning combinations as DATA, then loop:
```c
static const int WINS[8][3] = {{0,1,2}, {3,4,5}, ...};
for (int i = 0; i < 8; i++) {
    if (board[WINS[i][0]] == board[WINS[i][1]] && ...) { ... }
}
```

---

### 5. Spot Dangerous Patterns
**Goal:** Identify code that "works" but has hidden risks.

**Find these in `tictactoe.c`:**

#### A. Recursive Input (Stack Overflow Risk)
```c
int getHumanMove() {
    // ...
    if (invalid) {
        return getHumanMove();  // 🚨 Recursive!
    }
}
```
**Risk:** 10,000 bad inputs = 10,000 stack frames = crash.

**Why beginners write this:** Recursion feels elegant — "just call myself again!" But input loops should be *iterative*, not recursive. Each recursive call consumes stack memory that's never freed until the function returns.

**Level 2 solution:** Use `while(1)` with `continue`:
```c
int input_get_move() {
    while (1) {
        int move;
        scanf("%d", &move);
        if (is_valid(move)) return move;
        printf("Invalid!\n");
        // Loop continues — no stack growth
    }
}
```

#### B. Unchecked scanf (Undefined Behavior)
```c
scanf("%d", &move);  // What if user types "hello"?
```
**Risk:** `scanf` returns the number of items successfully read. If it returns `0`, `move` contains whatever garbage was in memory. The program continues with this garbage value.

**Even worse:** The invalid input stays in the buffer. The next `scanf` immediately fails again, causing an infinite loop that prints "Invalid!" forever.

**Level 2 solution:** Check return value and clear buffer:
```c
if (scanf("%d", &move) != 1) {
    // Clear the bad input
    while (getchar() != '\n');
    continue;
}
```

#### C. No Bounds Checking Before Array Access
```c
board[move] = currentPlayer;  // What if move == 100?
```
**Risk:** Writing outside array bounds is undefined behavior. It might:
- Corrupt other variables
- Crash immediately
- Crash later (hard to debug)
- Work fine (until it doesn't)

**Level 2 solution:** Always validate before use:
```c
if (move < 0 || move >= BOARD_SIZE) {
    return -1;  // Error code
}
board[move] = symbol;
```

---

### 6. Understand the "God Function" Anti-Pattern
**Goal:** See how `main()` doing everything creates problems.

**Exercise:** List everything `main()` does in Level 1:
1. Print welcome message
2. Get symbol choice from user
3. Set up player assignments
4. Run outer game loop (play again)
5. Run inner game loop (turns)
6. Display board each turn
7. Determine whose turn it is
8. Get human move OR computer move
9. Place piece on board
10. Check for winner/draw
11. Display result
12. Ask to play again
13. Reset board for new game

**That's 13+ responsibilities in ONE function!**

**Problems with God Functions:**

| Problem              | Example                                               |
|----------------------|-------------------------------------------------------|
| Can't unit test      | How do you test "print welcome" in isolation?         |
| Can't reuse          | Want the game logic in a GUI app? Rewrite everything. |
| Change amplification | Modifying turn logic might break play-again logic     |
| Cognitive overload   | Must hold 80+ lines in your head to understand flow   |
| Merge conflicts      | Two developers editing main() = guaranteed conflicts  |

**Level 2 solution:** Single Responsibility Principle — each module has ONE job:
```
main.c     → Coordinates high-level flow only
game.c     → Game state management
board.c    → Board data structure
display.c  → All printf calls
input.c    → All scanf calls
ai.c       → Computer decision-making
```

---

### 7. Trace Hidden Dependencies
**Goal:** Understand how global variables create invisible coupling.

**Exercise:** For each function, list which globals it reads or writes:

| Function            | Reads           | Writes                                         |
|---------------------|-----------------|------------------------------------------------|
| `printBoard()`      | `board`         | —                                              |
| `checkWinner()`     | `board`         | —                                              |
| `getComputerMove()` | `board`         | —                                              |
| `getHumanMove()`    | `board`         | —                                              |
| `makeMove()`        | —               | `board`                                        |
| `switchPlayer()`    | `currentPlayer` | `currentPlayer`                                |
| `resetBoard()`      | —               | `board`, `currentPlayer`, `gameOver`, `winner` |
| `main()`            | ALL             | ALL                                            |

**The problem:** You can't understand any function in isolation. To know what `makeMove()` does, you must know:
- What `board` looks like
- Who else modifies `board`
- When `board` gets reset

**Level 2 solution:** Explicit parameters make dependencies visible:
```c
// You can see EXACTLY what this function needs and affects
int game_make_move(GameState *state, int position) {
    state->board[position] = state->current_player;
    // ...
}
```

---

## Hands-On Exercises
### Exercise 1: The Annotation Challenge
Print out `tictactoe.c` (or open in an editor with multi-cursor highlighting) and color-code:
- 🔵 Blue = Display (`printf`)
- 🟢 Green = Input (`scanf`)
- 🟡 Yellow = Game Logic (win checks, turn switching)
- 🔴 Red = State Management (assigning to `board`, `winner`, etc.)

**Count:** How many times do colors alternate on consecutive lines? (More alternation = more tangled code.)

### Exercise 2: The Bug Hunt
Make these changes one at a time and observe:
1. Change `winner == 3` to `winner == 4` — What breaks? (Draw detection)
2. Delete one of the 8 win-check `if` statements — Can you still win that way? (No!)
3. Remove the `resetBoard()` call — What happens on game 2? (Old board persists)
4. Change `board[9]` to `board[8]` — What happens? (Buffer overflow!)

### Exercise 3: The Extension Challenge
Try to add a **4×4 board mode**. Count how many lines you must change:
- Array size: `board[16]`
- Loop bounds: Every `i < 9` becomes `i < 16`
- Win conditions: Rewrite ALL 8 (now need 10: 4 rows, 4 cols, 2 diagonals)
- Input validation: `move > 8` becomes `move > 15`
- Display: Completely rewrite `printBoard()`

**Total: 30+ line changes scattered throughout the file.**

In Level 2, you'd change:
- `BOARD_SIZE` constant: 1 line
- `WIN_COMBOS` data table: Add new rows
- `printBoard()`: Modify one function

### Exercise 4: The Impossible Test
Try to write a unit test for `checkWinner()` WITHOUT modifying the source:

```c
#include "tictactoe.c"  // Include entire file?

void test_x_wins() {
    // Problem 1: How do you set up board?
    // The global is defined in tictactoe.c!
    
    // Problem 2: How do you call checkWinner()?
    // It depends on the global board!
    
    // Problem 3: How do you reset for next test?
    // Must call resetBoard() — but that resets MORE than you want!
}
```

**Conclusion:** You literally cannot write isolated tests for this code. Testing requires refactoring to Level 2's design.

---

## Summary: Level 1 → Level 2 Transformation

| Level 1 Problem            | Level 2 Solution                          |
|----------------------------|-------------------------------------------|
| Global `board[9]`          | `GameState` struct with `board` member    |
| Magic number `9`           | `#define BOARD_SIZE 9`                    |
| `winner == 3`              | `status == STATUS_DRAW` enum              |
| 8 copy-pasted win checks   | `WIN_COMBOS` array + loop                 |
| Recursive `getHumanMove()` | Iterative `while(1)` loop                 |
| `main()` does everything   | Separate `game.c`, `display.c`, `input.c` |
| Unchecked `scanf`          | `if (scanf(...) != 1)` with buffer clear  |


### Next Level (Level 2 - Modular)
The next level addresses all these problems:
- Separate files for different concerns
- Enums for meaningful constants  
- A struct to hold game state
- Better input handling
- Data-driven win checking

See [Level 2 README](../level_2/README.md) for details.

