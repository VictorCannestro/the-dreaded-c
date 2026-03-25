# Win Condition Calculator Algorithm

## Overview

The Win Condition Calculator determines if a player has won the game by checking for `WIN_LENGTH` consecutive symbols in any row, column, or diagonal. The implementation is **fully configurable**, supporting any board size and win length.

**Key Properties:**
- **Configurable**: Works for 3x3, 4x4, 5x5, or any NxN board
- **Flexible win length**: Can require 3-in-a-row on a 5x5 board, or 4-in-a-row on a 4x4
- **Testable**: Split into four helper functions for granular unit testing
- **Efficient**: Early exit on first win found; O(n²) worst case

**Location**: `src/win_condition_calculator.c`

---

## Configuration Constants

Defined in `constants.h`:

```c
#define BOARD_DIM 3                    // Board dimension (3 = 3x3 board)
#define BOARD_SIZE (BOARD_DIM * BOARD_DIM)  // Total cells (9 for 3x3)
#define WIN_LENGTH BOARD_DIM           // Symbols needed to win (3 for standard)
```

**Example configurations:**

| Game Variant | BOARD_DIM | BOARD_SIZE | WIN_LENGTH |
|--------------|-----------|------------|------------|
| Standard Tic-Tac-Toe | 3 | 9 | 3 |
| 4x4 Tic-Tac-Toe | 4 | 16 | 4 |
| Gomoku (5-in-a-row) | 15 | 225 | 5 |
| Connect 4 variant | 5 | 25 | 4 |

---

## Architecture

The win condition checker is organized into a **hierarchy of functions**:

```
wcc_check_winner()           ← Main entry point
    ├── wcc_check_rows()     ← Check all horizontal lines
    ├── wcc_check_columns()  ← Check all vertical lines
    ├── wcc_check_main_diagonals()    ← Check \ diagonals
    └── wcc_check_anti_diagonals()    ← Check / diagonals
            │
            └── check_line()  ← Core helper (static, internal)
```

**Benefits of this structure:**
1. **Testability**: Each direction can be tested independently
2. **Readability**: Clear separation of concerns
3. **Debugging**: Easy to isolate which check found a winner
4. **Extensibility**: Easy to add new win patterns (e.g., corners)

---

## The Board as a 1D Array

The board is stored as a 1D array in **row-major order**:

```
Visual Grid:          1D Array:
 0 | 1 | 2            board[0], board[1], board[2],
---+---+---     →     board[3], board[4], board[5],
 3 | 4 | 5            board[6], board[7], board[8]
---+---+---
 6 | 7 | 8
```

**Index formula**: `position = row * BOARD_DIM + col`

---

## Core Algorithm: `check_line`

All win checking ultimately uses this single helper function:

```c
static CellValue check_line(const CellValue board[BOARD_SIZE], int start, int step) {
    CellValue first = board[start];
    if (first == CELL_EMPTY) {
        return CELL_EMPTY;
    }

    for (int i = 1; i < WIN_LENGTH; i++) {
        if (board[start + i * step] != first) {
            return CELL_EMPTY;
        }
    }
    return first;
}
```

### Parameters

| Parameter | Description | Example |
|-----------|-------------|---------|
| `board` | The game board array | `state->board` |
| `start` | Starting index of the line | `0` for top-left |
| `step` | Index increment to next cell | `1` for horizontal |

### Step Values for Different Directions

| Direction | Step Value | Formula | Visual |
|-----------|------------|---------|--------|
| Horizontal (→) | `1` | Next column | `0 → 1 → 2` |
| Vertical (↓) | `BOARD_DIM` | Next row | `0 → 3 → 6` |
| Main diagonal (↘) | `BOARD_DIM + 1` | Next row + next col | `0 → 4 → 8` |
| Anti-diagonal (↙) | `BOARD_DIM - 1` | Next row + prev col | `2 → 4 → 6` |

### Visual Example (3x3 board)

```
Horizontal (step=1):        Vertical (step=3):
[0] → [1] → [2]             [0]
                             ↓
                            [3]
                             ↓
                            [6]

Main diagonal (step=4):     Anti-diagonal (step=2):
[0]                               [2]
   ↘                             ↙
    [4]                       [4]
       ↘                     ↙
        [8]               [6]
```

### Algorithm Walkthrough

**Input**: `board = [X, X, X, O, O, -, -, -, -]`, `start = 0`, `step = 1`

```
Step 1: first = board[0] = X
        X is not CELL_EMPTY, continue

Step 2: Loop i = 1
        Check board[0 + 1*1] = board[1] = X
        X == first (X), continue

Step 3: Loop i = 2
        Check board[0 + 2*1] = board[2] = X
        X == first (X), continue

Step 4: Loop complete (i = 3 = WIN_LENGTH)
        Return first = X  ← Winner found!
```

---

## Row Checking: `wcc_check_rows`

Checks all horizontal lines for a winner.

```c
CellValue wcc_check_rows(const CellValue board[BOARD_SIZE]) {
    for (int row = 0; row < BOARD_DIM; row++) {
        for (int col = 0; col <= BOARD_DIM - WIN_LENGTH; col++) {
            int start = row * BOARD_DIM + col;
            CellValue winner = check_line(board, start, 1);
            if (winner != CELL_EMPTY) {
                return winner;
            }
        }
    }
    return CELL_EMPTY;
}
```

### Why the Inner Loop?

For larger boards where `WIN_LENGTH < BOARD_DIM`, there are multiple possible winning positions per row:

**5x5 board with WIN_LENGTH = 3:**
```
Row 0 has 3 possible horizontal wins:
  [0,1,2]  [1,2,3]  [2,3,4]
   ^^^      ^^^      ^^^
```

The inner loop iterates `col` from `0` to `BOARD_DIM - WIN_LENGTH` to check each starting position.

### For Standard 3x3 (WIN_LENGTH = BOARD_DIM)

The inner loop runs only once per row (`col` goes from 0 to 0):

```
Row 0: start = 0, check positions [0, 1, 2]
Row 1: start = 3, check positions [3, 4, 5]
Row 2: start = 6, check positions [6, 7, 8]
```

### Visual: All Row Starting Positions (3x3)

```
 [0] → → |  1  |  2      start=0, step=1
---------+-----+-----
 [3] → → |  4  |  5      start=3, step=1
---------+-----+-----
 [6] → → |  7  |  8      start=6, step=1
```

---

## Column Checking: `wcc_check_columns`

Checks all vertical lines for a winner.

```c
CellValue wcc_check_columns(const CellValue board[BOARD_SIZE]) {
    for (int col = 0; col < BOARD_DIM; col++) {
        for (int row = 0; row <= BOARD_DIM - WIN_LENGTH; row++) {
            int start = row * BOARD_DIM + col;
            CellValue winner = check_line(board, start, BOARD_DIM);
            if (winner != CELL_EMPTY) {
                return winner;
            }
        }
    }
    return CELL_EMPTY;
}
```

### Step Value Explanation

`step = BOARD_DIM` moves to the next row in the same column:

```
Column 0:  board[0] → board[0+3] → board[0+6]
           board[0] → board[3]   → board[6]
```

### Visual: All Column Starting Positions (3x3)

```
 [0]  |  [1]  |  [2]     start=0,1,2
  ↓   |   ↓   |   ↓      step=3
  3   |   4   |   5
  ↓   |   ↓   |   ↓
  6   |   7   |   8
```

---

## Main Diagonal Checking: `wcc_check_main_diagonals`

Checks all top-left to bottom-right (↘) diagonals.

```c
CellValue wcc_check_main_diagonals(const CellValue board[BOARD_SIZE]) {
    for (int row = 0; row <= BOARD_DIM - WIN_LENGTH; row++) {
        for (int col = 0; col <= BOARD_DIM - WIN_LENGTH; col++) {
            int start = row * BOARD_DIM + col;
            CellValue winner = check_line(board, start, BOARD_DIM + 1);
            if (winner != CELL_EMPTY) {
                return winner;
            }
        }
    }
    return CELL_EMPTY;
}
```

### Step Value Explanation

`step = BOARD_DIM + 1` moves one row down AND one column right:

```
From position 0:  board[0] → board[0+4] → board[0+8]
                  board[0] → board[4]   → board[8]

Visual:
[0]
   ↘
    [4]
       ↘
        [8]
```

### Why Both Loops Have Upper Bounds?

A diagonal starting too far right or too far down won't have enough cells:

```
5x5 board, WIN_LENGTH = 3:

Valid starts (can fit 3 cells diagonally):
[0] [1] [2]  .   .
[5] [6] [7]  .   .
[10][11][12] .   .
 .   .   .   .   .
 .   .   .   .   .

Invalid starts (would go out of bounds):
 .   .   .  [3] [4]   ← Too far right
 .   .   .   .   .
 .   .   .   .   .
[15][16][17] .   .    ← Too far down
[20][21][22] .   .
```

### Visual: Main Diagonal Starting Positions (3x3)

For standard 3x3 with WIN_LENGTH = 3, only position 0 is valid:

```
 [0] ↘    |  1  |  2
----------+-----+-----
  3  |  [4] ↘  |  5
----------+-----+-----
  6  |  7  |  [8] ←end
```

---

## Anti-Diagonal Checking: `wcc_check_anti_diagonals`

Checks all top-right to bottom-left (↙) diagonals.

```c
CellValue wcc_check_anti_diagonals(const CellValue board[BOARD_SIZE]) {
    for (int row = 0; row <= BOARD_DIM - WIN_LENGTH; row++) {
        for (int col = WIN_LENGTH - 1; col < BOARD_DIM; col++) {
            int start = row * BOARD_DIM + col;
            CellValue winner = check_line(board, start, BOARD_DIM - 1);
            if (winner != CELL_EMPTY) {
                return winner;
            }
        }
    }
    return CELL_EMPTY;
}
```

### Step Value Explanation

`step = BOARD_DIM - 1` moves one row down AND one column left:

```
From position 2:  board[2] → board[2+2] → board[2+4]
                  board[2] → board[4]   → board[6]

Visual:
        [2]
       ↙
    [4]
   ↙
[6]
```

### Why `col` Starts at `WIN_LENGTH - 1`?

The anti-diagonal goes LEFT, so we need enough columns to the left:

```
3x3 board, WIN_LENGTH = 3:

col must start at index 2 (WIN_LENGTH - 1 = 2):
  0   1  [2] ← Start here
          ↙
      [4]
       ↙
  [6]

Starting at col=0 or col=1 would go out of bounds to the left.
```

### Visual: Anti-Diagonal Starting Positions (3x3)

```
  0  |  1  |  [2] ↙
-----+-----+----------
  3  |  [4] ↙  |  5
-----+-----+----------
 [6] ← end |  7  |  8
```

---

## Main Entry Point: `wcc_check_winner`

Orchestrates all four direction checks:

```c
CellValue wcc_check_winner(const CellValue board[BOARD_SIZE]) {
    CellValue winner;

    winner = wcc_check_rows(board);
    if (winner != CELL_EMPTY) return winner;

    winner = wcc_check_columns(board);
    if (winner != CELL_EMPTY) return winner;

    winner = wcc_check_main_diagonals(board);
    if (winner != CELL_EMPTY) return winner;

    winner = wcc_check_anti_diagonals(board);
    if (winner != CELL_EMPTY) return winner;

    return CELL_EMPTY;
}
```

### Early Exit Pattern

The function returns immediately when a winner is found. This provides:
1. **Efficiency**: No unnecessary checks after win detected
2. **Determinism**: Same winner always returned for same board state
3. **Priority**: Rows checked before columns before diagonals

---

## Draw Detection: `wcc_is_draw`

```c
int wcc_is_draw(const CellValue board[BOARD_SIZE], int move_count) {
    return (move_count == BOARD_SIZE) && (wcc_check_winner(board) == CELL_EMPTY);
}
```

A game is a draw when:
1. All cells are filled (`move_count == BOARD_SIZE`)
2. No winner exists (`wcc_check_winner() == CELL_EMPTY`)

**Note**: We check `move_count` first (O(1)) before calling `wcc_check_winner` (O(n²)) for efficiency.

---

## Complexity Analysis

### Time Complexity

| Function | Complexity | Explanation |
|----------|------------|-------------|
| `check_line` | O(WIN_LENGTH) | Checks WIN_LENGTH cells |
| `wcc_check_rows` | O(n² × w) | n rows × (n-w+1) starts × w cells |
| `wcc_check_columns` | O(n² × w) | Same as rows |
| `wcc_check_main_diagonals` | O(n² × w) | (n-w+1)² starts × w cells |
| `wcc_check_anti_diagonals` | O(n² × w) | Same as main diagonals |
| `wcc_check_winner` | O(n² × w) | Sum of all four |
| `wcc_is_draw` | O(n² × w) | Calls wcc_check_winner |

Where `n = BOARD_DIM` and `w = WIN_LENGTH`.

For standard 3x3: O(9 × 3) = O(27) ≈ O(1) constant time.

### Space Complexity

| Function | Space | Explanation |
|----------|-------|-------------|
| All functions | O(1) | Only loop counters and temporary variables |

---

## Complete Example Walkthrough

**Board State:**
```
 X | O | X      board = [X, O, X, O, X, O, X, O, X]
---+---+---     indices:  0  1  2  3  4  5  6  7  8
 O | X | O
---+---+---
 X | O | X
```

**Execution trace:**

```
wcc_check_winner(board):
│
├── wcc_check_rows(board):
│   ├── Row 0: check_line(board, 0, 1)
│   │   └── board[0]=X, board[1]=O → X≠O, return CELL_EMPTY
│   ├── Row 1: check_line(board, 3, 1)
│   │   └── board[3]=O, board[4]=X → O≠X, return CELL_EMPTY
│   └── Row 2: check_line(board, 6, 1)
│       └── board[6]=X, board[7]=O → X≠O, return CELL_EMPTY
│   └── Return CELL_EMPTY
│
├── wcc_check_columns(board):
│   ├── Col 0: check_line(board, 0, 3)
│   │   └── board[0]=X, board[3]=O → X≠O, return CELL_EMPTY
│   ├── Col 1: check_line(board, 1, 3)
│   │   └── board[1]=O, board[4]=X → O≠X, return CELL_EMPTY
│   └── Col 2: check_line(board, 2, 3)
│       └── board[2]=X, board[5]=O → X≠O, return CELL_EMPTY
│   └── Return CELL_EMPTY
│
├── wcc_check_main_diagonals(board):
│   └── check_line(board, 0, 4)
│       ├── board[0]=X (first)
│       ├── board[4]=X == X ✓
│       └── board[8]=X == X ✓
│       └── Return X  ← WINNER FOUND!
│
└── Return X (early exit, anti-diagonals not checked)
```

---

## Testing Strategy

The modular design enables granular testing:

### Unit Tests for Each Direction

```c
// Test rows independently
void test_wcc_check_rows_x_wins_top_row(void) {
    CellValue board[BOARD_SIZE] = {
        CELL_X, CELL_X, CELL_X,
        CELL_O, CELL_O, CELL_EMPTY,
        CELL_EMPTY, CELL_EMPTY, CELL_EMPTY
    };
    TEST_ASSERT_EQUAL(CELL_X, wcc_check_rows(board));
}

// Verify columns don't detect row wins
void test_wcc_check_columns_no_winner_when_row_wins(void) {
    CellValue board[BOARD_SIZE] = {
        CELL_X, CELL_X, CELL_X,  // Row win
        CELL_O, CELL_O, CELL_EMPTY,
        CELL_EMPTY, CELL_EMPTY, CELL_EMPTY
    };
    TEST_ASSERT_EQUAL(CELL_EMPTY, wcc_check_columns(board));  // Should NOT detect
}
```

### Edge Cases to Test

| Test Case | Board State | Expected |
|-----------|-------------|----------|
| Empty board | All CELL_EMPTY | CELL_EMPTY |
| Partial row (2 in a row) | X, X, empty | CELL_EMPTY |
| Full board, no winner | Alternating X/O | CELL_EMPTY (draw) |
| Win on last move | 8 moves + winning move | Winner |
| Multiple wins possible | Row AND column | First found (row) |

---

## Extending the Algorithm

### Adding New Win Patterns

To add corner wins (all 4 corners same symbol):

```c
CellValue wcc_check_corners(const CellValue board[BOARD_SIZE]) {
    int corners[] = {0, BOARD_DIM-1, 
                     BOARD_SIZE-BOARD_DIM, BOARD_SIZE-1};
    
    CellValue first = board[corners[0]];
    if (first == CELL_EMPTY) return CELL_EMPTY;
    
    for (int i = 1; i < 4; i++) {
        if (board[corners[i]] != first) return CELL_EMPTY;
    }
    return first;
}
```

### Returning Win Information

To return which cells formed the win:

```c
typedef struct {
    CellValue winner;
    int positions[WIN_LENGTH];  // Winning cell indices
} WinResult;

WinResult wcc_check_winner_detailed(const CellValue board[BOARD_SIZE]);
```

### Performance Optimization

For very large boards, consider:

1. **Incremental checking**: Only check lines affected by the last move
2. **Bitboard representation**: Use bit manipulation for faster comparisons
3. **Early termination**: Skip checking if not enough moves for a win

---

## Comparison: Data-Driven vs Algorithmic Approach

### Previous Approach (Data-Driven)

```c
static const int WIN_COMBOS[8][3] = {
    {0, 1, 2}, {3, 4, 5}, {6, 7, 8},  // Rows
    {0, 3, 6}, {1, 4, 7}, {2, 5, 8},  // Columns
    {0, 4, 8}, {2, 4, 6}              // Diagonals
};

for (int i = 0; i < 8; i++) {
    if (board[WIN_COMBOS[i][0]] != CELL_EMPTY &&
        board[WIN_COMBOS[i][0]] == board[WIN_COMBOS[i][1]] &&
        board[WIN_COMBOS[i][1]] == board[WIN_COMBOS[i][2]]) {
        return board[WIN_COMBOS[i][0]];
    }
}
```

### Current Approach (Algorithmic)

```c
for (int row = 0; row < BOARD_DIM; row++) {
    int start = row * BOARD_DIM;
    CellValue winner = check_line(board, start, 1);
    if (winner != CELL_EMPTY) return winner;
}
// ... similar for columns and diagonals
```

### Comparison

| Aspect | Data-Driven | Algorithmic |
|--------|-------------|-------------|
| Board size change | Update WIN_COMBOS array | Change BOARD_DIM constant |
| Win length change | Restructure array | Change WIN_LENGTH constant |
| Memory usage | O(combinations × win_length) | O(1) |
| Code complexity | Simple loop | Nested loops with math |
| Testability | Test all 8 combos | Test each direction |
| Extensibility | Add to array | Add new function |

The algorithmic approach is preferred for **configurable board sizes**.

---

## See Also

- [Minimax Algorithm](MINIMAX_ALGORITHM.md) — Uses win condition checker for game tree evaluation
- [Alpha-Beta Pruning](ALPHA_BETA_PRUNING.md) — Optimization that relies on win detection
- [CLI Display Board](CLI_DISPLAY_BOARD.md) — Renders the board being checked
- [src/win_condition_calculator.c](../../src/win_condition_calculator.c) — Full implementation
- [tests/test_win_condition_calculator.c](../../tests/test_win_condition_calculator.c) — Unit tests (36 tests)

