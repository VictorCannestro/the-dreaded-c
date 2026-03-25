# CLI Display Board Algorithm

## Overview

The `cli_display_board` function renders the game board to the terminal using ASCII art. It's designed to be **fully dynamic**, automatically adapting to any board size defined by `BOARD_DIM` in `constants.h`.

**Key Properties:**
- **Configurable**: Works for 3x3, 4x4, 5x5, or any NxN board
- **Zero hardcoding**: No magic numbers; all dimensions derived from `BOARD_DIM`
- **Clean output**: Proper grid lines, centered markers, no trailing separators

**Location**: `src/ui_cli.c`

---

## Visual Output

### 3x3 Board (BOARD_DIM = 3)
```
   |   |   
 X | O | X 
___|___|___
   |   |   
 O | X | O 
___|___|___
   |   |   
 X | O | X 
   |   |   
```

### 4x4 Board (BOARD_DIM = 4)
```
   |   |   |   
 X | O | X | O 
___|___|___|___
   |   |   |   
 O | X | O | X 
___|___|___|___
   |   |   |   
 X | O | X | O 
___|___|___|___
   |   |   |   
 O | X | O | X 
   |   |   |   
```

---

## Algorithm Structure

The function uses a **row-major traversal** with three sub-rows per logical row:

```
For each row (0 to BOARD_DIM-1):
    ├── [1] Print TOP PADDING ROW:     "   |   |   "
    ├── [2] Print CELL VALUES ROW:     " X | O | X "
    └── [3] Print BOTTOM BORDER ROW:   "___|___|___" (or spaces for last row)
```

### Why Three Sub-Rows?

Each cell in the grid is rendered as a 3x3 character block:

```
┌───┐
│   │  ← Top padding (visual breathing room)
│ X │  ← Cell value (centered marker)
│___│  ← Bottom border (grid line or spacing)
└───┘
```

This creates a visually balanced grid where markers are centered and easy to read.

---

## Complete Implementation

### String Constants

The board display uses four named constants to avoid magic strings and improve maintainability:

| Constant | Value | Purpose |
|----------|-------|---------|
| `BOARD_CELL_PADDING` | `"   "` | Empty padding for cell width (3 spaces) |
| `BOARD_CELL_SEPARATOR` | `"\|"` | Vertical separator between cells |
| `BOARD_CELL_FORMAT` | `" %c "` | Format string for cell values (centered marker) |
| `BOARD_ROW_DIVIDER` | `"___"` | Horizontal divider between rows (3 underscores) |

These constants are defined alongside other UI string constants in `ui_cli.c`:

```c
/* String Constants for Board Display */
static const char *BOARD_CELL_PADDING   = "   ";
static const char *BOARD_CELL_SEPARATOR = "|";
static const char *BOARD_CELL_FORMAT    = " %c ";
static const char *BOARD_ROW_DIVIDER    = "___";
```

### Function Implementation

```c
static void cli_display_board(const GameState *state) {
    if (state == NULL) {
        return;
    }

    printf("\n");

    for (int row = 0; row < BOARD_DIM; row++) {
        /* Top padding row */
        for (int col = 0; col < BOARD_DIM; col++) {
            printf("%s", BOARD_CELL_PADDING);
            if (col < BOARD_DIM - 1) {
                printf("%s", BOARD_CELL_SEPARATOR);
            }
        }
        printf("\n");

        /* Cell values row */
        for (int col = 0; col < BOARD_DIM; col++) {
            int pos = row * BOARD_DIM + col;
            printf(BOARD_CELL_FORMAT, cell_value_to_marker(state->board[pos]));
            if (col < BOARD_DIM - 1) {
                printf("%s", BOARD_CELL_SEPARATOR);
            }
        }
        printf("\n");

        /* Bottom border row (or spacing for last row) */
        for (int col = 0; col < BOARD_DIM; col++) {
            if (row < BOARD_DIM - 1) {
                printf("%s", BOARD_ROW_DIVIDER);
            } else {
                printf("%s", BOARD_CELL_PADDING);
            }
            if (col < BOARD_DIM - 1) {
                printf("%s", BOARD_CELL_SEPARATOR);
            }
        }
        printf("\n");
    }

    printf("\n");
}
```

---

## Step-by-Step Breakdown

### Step 1: Null Guard
```c
if (state == NULL) {
    return;
}
```
**Purpose**: Defensive programming. If passed a null pointer, exit gracefully instead of crashing.

### Step 2: Outer Loop (Rows)
```c
for (int row = 0; row < BOARD_DIM; row++) {
```
**Purpose**: Iterate through each logical row of the board (0, 1, 2 for a 3x3 board).

### Step 3: Top Padding Row
```c
for (int col = 0; col < BOARD_DIM; col++) {
    printf("%s", BOARD_CELL_PADDING);             // 3 spaces for cell width
    if (col < BOARD_DIM - 1) {
        printf("%s", BOARD_CELL_SEPARATOR);       // Vertical separator
    }
}
printf("\n");
```

**Output**: `   |   |   \n`

**Key Detail**: The `if (col < BOARD_DIM - 1)` check prevents a trailing `|` after the last column.

| col | printf("   ") | printf("\|") | Result |
|-----|---------------|--------------|--------|
| 0   | "   "         | "\|"         | "   \|" |
| 1   | "   "         | "\|"         | "   \|" |
| 2   | "   "         | (skipped)    | "   "  |

### Step 4: Cell Values Row
```c
for (int col = 0; col < BOARD_DIM; col++) {
    int pos = row * BOARD_DIM + col;      // 2D → 1D index conversion
    printf(BOARD_CELL_FORMAT, cell_value_to_marker(state->board[pos]));
    if (col < BOARD_DIM - 1) {
        printf("%s", BOARD_CELL_SEPARATOR);
    }
}
printf("\n");
```

**Output**: ` X | O | - \n`

**Index Conversion Formula**: `pos = row * BOARD_DIM + col`

This converts 2D grid coordinates to a 1D array index:

| row | col | pos = row × 3 + col | Board Position |
|-----|-----|---------------------|----------------|
| 0   | 0   | 0                   | Top-left       |
| 0   | 1   | 1                   | Top-center     |
| 0   | 2   | 2                   | Top-right      |
| 1   | 0   | 3                   | Middle-left    |
| 1   | 1   | 4                   | Center         |
| 1   | 2   | 5                   | Middle-right   |
| 2   | 0   | 6                   | Bottom-left    |
| 2   | 1   | 7                   | Bottom-center  |
| 2   | 2   | 8                   | Bottom-right   |

**The `cell_value_to_marker` Function**:
```c
char cell_value_to_marker(CellValue cell) {
    switch (cell) {
        case CELL_EMPTY: return '-';  // or ' '
        case CELL_X:     return 'X';
        case CELL_O:     return 'O';
        default:         return '-';
    }
}
```

### Step 5: Bottom Border Row
```c
for (int col = 0; col < BOARD_DIM; col++) {
    if (row < BOARD_DIM - 1) {
        printf("%s", BOARD_ROW_DIVIDER);    // Underscores for grid lines
    } else {
        printf("%s", BOARD_CELL_PADDING);   // Spaces for last row (clean bottom edge)
    }
    if (col < BOARD_DIM - 1) {
        printf("%s", BOARD_CELL_SEPARATOR);
    }
}
printf("\n");
```

**Output**: 
- Rows 0-1: `___|___|___\n`
- Row 2: `   |   |   \n`

**Why the conditional?** The last row shouldn't have underscores below it—that would look like an incomplete grid. Instead, we print spaces to maintain alignment.

---

## Key Design Decisions

| Decision | Rationale |
|----------|-----------|
| **String constants** (`BOARD_CELL_PADDING`, etc.) | Centralizes display strings for easy modification and consistency |
| **3-character cell width** (`" X "`) | Centers the marker with padding for readability |
| **Conditional separators** (`if col < BOARD_DIM - 1`) | Prevents trailing `\|` at end of each line |
| **Conditional bottom border** (`if row < BOARD_DIM - 1`) | Clean visual edge—no dangling underscores |
| **Index formula** (`row * BOARD_DIM + col`) | Standard row-major mapping from 2D to 1D |
| **Null guard at start** | Defensive programming—prevents crashes |
| **Using `BOARD_DIM` constant** | Automatically adapts to any board size |

---

## Complexity Analysis

| Metric | Value | Explanation |
|--------|-------|-------------|
| **Time Complexity** | O(n²) | Where n = BOARD_DIM. Visits each cell once. |
| **Space Complexity** | O(1) | Only uses loop counters; no dynamic allocation |
| **I/O Operations** | O(n²) | One `printf` per cell segment |

For a 3x3 board: 9 cells × 3 sub-rows = 27 cell-prints + separators ≈ 50 printf calls.

---

## Extending the Algorithm

### Adding Row/Column Labels

To help players identify positions, you could add labels:

```c
// Before each row, print row number
printf(" %d ", row);

// Before the grid, print column headers
printf("   ");
for (int col = 0; col < BOARD_DIM; col++) {
    printf(" %d  ", col);
}
printf("\n");
```

**Output with labels:**
```
     0   1   2
   +---+---+---+
 0 | X | O | X |
   +---+---+---+
 1 | O | X | O |
   +---+---+---+
 2 | X | O | X |
   +---+---+---+
```

### Color Output (ANSI Escape Codes)

```c
#define RED     "\033[31m"
#define BLUE    "\033[34m"
#define RESET   "\033[0m"

// In the cell values section:
if (marker == 'X') {
    printf(" %s%c%s ", RED, marker, RESET);
} else if (marker == 'O') {
    printf(" %s%c%s ", BLUE, marker, RESET);
} else {
    printf(" %c ", marker);
}
```

### Unicode Box Drawing

For a more polished look:

```c
printf("┌───┬───┬───┐\n");
printf("│ X │ O │ X │\n");
printf("├───┼───┼───┤\n");
// ...
printf("└───┴───┴───┘\n");
```

---

## Testing Considerations

Since `cli_display_board` produces output to `stdout`, testing requires either:

1. **Visual inspection**: Run the game and verify output looks correct
2. **Output capture**: Redirect stdout to a buffer and compare against expected string
3. **Mock the interface**: Use the `UserInterface` abstraction to inject a test implementation

Example test approach with output capture:

```c
void test_display_board_empty(void) {
    GameState game;
    game_init_session(&game);
    
    // Capture stdout
    char buffer[1024];
    FILE *stream = fmemopen(buffer, sizeof(buffer), "w");
    FILE *old_stdout = stdout;
    stdout = stream;
    
    cli_display_board(&game);
    
    fflush(stream);
    stdout = old_stdout;
    fclose(stream);
    
    // Verify output contains expected patterns
    TEST_ASSERT_NOT_NULL(strstr(buffer, " - | - | - "));
}
```

---

## Comparison: Template vs Dynamic Approach

### Original Template Approach (Hardcoded)
```c
const char *board_template =
    "   |   |   \n"
    " %c | %c | %c \n"
    "___|___|___\n"
    // ... 9 format specifiers
    ;

printf(board_template,
    marker(board[0]), marker(board[1]), marker(board[2]),
    marker(board[3]), marker(board[4]), marker(board[5]),
    marker(board[6]), marker(board[7]), marker(board[8])
);
```

**Pros**: Simple, single printf call  
**Cons**: Hardcoded for 3x3 only, must rewrite for 4x4

### Current Dynamic Approach (Loop-Based)
```c
for (int row = 0; row < BOARD_DIM; row++) {
    for (int col = 0; col < BOARD_DIM; col++) {
        // ... render cell
    }
}
```

**Pros**: Works for any board size, easy to extend  
**Cons**: More code, multiple printf calls

| Aspect | Template | Dynamic |
|--------|----------|---------|
| Lines of code | ~15 | ~25 |
| Board sizes supported | 3x3 only | Any NxN |
| Adding features | Rewrite template | Modify loop body |
| Performance | Single printf | Multiple printfs |
| Readability | Visual template | Algorithmic |

The dynamic approach is preferred for maintainability and extensibility.

---

## See Also

- [Minimax Algorithm](MINIMAX_ALGORITHM.md) — AI decision-making
- [Alpha-Beta Pruning](ALPHA_BETA_PRUNING.md) — Optimization for minimax
- [src/ui_cli.c](../../src/ui_cli.c) — Full CLI implementation
- [include/ui_interface.h](../../include/ui_interface.h) — UI abstraction layer

