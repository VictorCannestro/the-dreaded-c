# Lesson 5: Arrays
Arrays store multiple values of the same type in contiguous memory.

## Declaring and Using Arrays
```c
int numbers[5];          // Array of 5 integers (indices 0-4)
numbers[0] = 10;         // First element
numbers[4] = 50;         // Last element (NOT numbers[5]!)

// Initialize at declaration
int primes[] = {2, 3, 5, 7, 11};  // Size inferred: 5 elements
```

## ⚠️ Critical: Arrays Are 0-Indexed!
```
Index:    [0]  [1]  [2]  [3]  [4]
Values:    10   20   30   40   50
```

- First element: `array[0]`
- Last element of size-N array: `array[N-1]`
- `array[N]` is **OUT OF BOUNDS** (undefined behavior!)

## The Board: 1D Array as 2D Grid
Our tic-tac-toe board uses a **linear array** to represent a grid:

```c
// constants.h - Configurable board size
#define BOARD_DIM 3                         // Change to 4, 5, etc.
#define BOARD_SIZE (BOARD_DIM * BOARD_DIM)  // 9 for 3x3, 16 for 4x4
#define WIN_LENGTH BOARD_DIM                // Symbols needed to win

// tictactoe.h
CellValue board[BOARD_SIZE];  // Uses the constant

// Visual mapping for 3x3:
//   board[0] | board[1] | board[2]
//   ---------+----------+---------
//   board[3] | board[4] | board[5]
//   ---------+----------+---------
//   board[6] | board[7] | board[8]
```

**Converting row/column to index:**
```c
int position = row * BOARD_DIM + col;

// Examples (3x3 board):
// (0,0) → 0*3+0 = 0  (top-left)
// (1,1) → 1*3+1 = 4  (center)
// (2,2) → 2*3+2 = 8  (bottom-right)
```

**Why configurable?** Change `BOARD_DIM` to 4 and rebuild — the entire game adapts to a 4x4 board!

## Arrays and Pointers
When passed to functions, arrays "decay" to pointers:

```c
void clear_board(CellValue board[9]) {
    // board is actually a pointer here!
    for (int i = 0; i < 9; i++) {
        board[i] = CELL_EMPTY;
    }
}

// These declarations are equivalent for parameters:
void func1(int arr[9]);   // Looks like array
void func2(int arr[]);    // Size optional
void func3(int *arr);     // Actually a pointer
```

## Common Array Operations

```c
// Iterate over all elements
for (int i = 0; i < BOARD_SIZE; i++) {
    if (board[i] == CELL_EMPTY) {
        // Found empty cell
    }
}

// Fill with a value (for single-byte values like 0)
#include <string.h>
memset(board, 0, sizeof(board));

// Copy an array
memcpy(destination, source, sizeof(source));
```

See [src/board.c](../../src/board.c) for array operations.

---

## Next Steps

Continue to [Lesson 6: Structs](LESSON_6_Structs.md) →

← Previous: [Lesson 4: Functions](LESSON_4_Functions.md)

← Back to [Learning Guide Index](LEARNING_GUIDE.md)

