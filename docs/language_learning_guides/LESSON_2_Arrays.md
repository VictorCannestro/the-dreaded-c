# Lesson 2: Arrays
Arrays let you store multiple values of the **same type** side-by-side in memory — instead of writing
`score0`, `score1`, `score2`, … you write `scores[0]`, `scores[1]`, `scores[2]`, and loop over them.
In this lesson you'll go from declaring your first array all the way to understanding how the tic-tac-toe
board is built from one.

---

## Table of Contents
### Core Concepts
1. [What Is an Array?](#1-what-is-an-array)
2. [Declaring Arrays](#2-declaring-arrays)
3. [Arrays Are 0-Indexed — Always](#3--arrays-are-0-indexed--always)
4. [Reading and Writing Elements](#4-reading-and-writing-elements)
5. [The Board: A 1D Array Representing a 2D Grid](#5-the-board-a-1d-array-representing-a-2d-grid)
6. [Multi-Dimensional Arrays (2D and 3D)](#6-multi-dimensional-arrays-2d-and-3d)
7. [Strings in C — A Special Kind of `char` Array](#7-strings-in-c--a-special-kind-of-char-array)
8. [Real Code — `src/board.c` Annotated](#8-real-code--srcboardc-annotated)

### Practice
9. [Quick Reference](#9-quick-reference)
10. [Practical Exercises](#10-practical-exercises)

### Going Further
11. [Next Steps](#next-steps)
12. [Bonus: Advanced Topics](#bonus-advanced-topics) *(return after Lessons 4 & 5)*
    - What `const char *` Actually Means
    - Common String Functions (`<string.h>`)
    - Arrays in Memory — Why Contiguous Matters
    - Arrays and Pointers — What Really Happens
    - `const` — Protecting Array Contents

---


## 1. What Is an Array?
An array is a **fixed-size, contiguous block of memory** that holds elements of one type.
"Contiguous" means the elements are right next to each other — no gaps.

```
Memory layout for  int scores[5] = {10, 20, 30, 40, 50};

Address:  1000  1004  1008  1012  1016
Value:      10    20    30    40    50
Index:     [0]   [1]   [2]   [3]   [4]
```

> *Note*: Each `int` takes 4 bytes, so the next element is exactly 4 bytes further along in memory.


## 2. Declaring Arrays
```c
// Syntax:  type  name[size];
int  scores[5];           // 5 integers — values are GARBAGE until assigned
char letters[3];          // 3 chars
```

> **⚠️ Uninitialized arrays contain garbage!**
> C does **not** zero-out memory for you. Always initialise before use.

### Declaring with an Initialiser List
```c
// Size is explicit
int scores[5] = {10, 20, 30, 40, 50};

// Size inferred from the list (preferred when you list all values)
int primes[] = {2, 3, 5, 7, 11};   // compiler counts: size = 5

// Partial initialisation — remaining elements are set to 0
int partial[5] = {1, 2};            // {1, 2, 0, 0, 0}

// Zero-initialise the whole array (idiomatic C trick)
int zeros[100] = {0};               // all 100 elements are 0
```


## 3. ⚠️ Arrays Are 0-Indexed — Always
This is the **single most common source of bugs** for new C programmers.

```
Array:  scores[5] = {10, 20, 30, 40, 50}

Index:   [0]  [1]  [2]  [3]  [4]
Value:    10   20   30   40   50
                                ↑
                        Last valid index = size - 1 = 4
```

| Rule              | Example (size = 5)                       |
|-------------------|------------------------------------------|
| First element     | `scores[0]` → `10`                       |
| Last element      | `scores[4]` → `50`                       |
| **Out of bounds** | `scores[5]` → 💥 **Undefined Behaviour** |

```c
int scores[5] = {10, 20, 30, 40, 50};

printf("%d\n", scores[0]);  // ✅ 10
printf("%d\n", scores[4]);  // ✅ 50
printf("%d\n", scores[5]);  // ❌ Out of bounds — anything can happen
```

> **Undefined Behaviour** means the compiler is allowed to do anything: return garbage, crash, or —
> most dangerously — silently corrupt other data. C gives you no safety net here.


## 4. Reading and Writing Elements
```c
int board[9];           // Declare
board[0] = 42;          // Write to element 0
int val = board[0];     // Read element 0 → val is 42
board[0]++;             // Increment element 0 → now 43
```

### Iterating with a `for` Loop
The pattern `for (int i = 0; i < SIZE; i++)` is the bread-and-butter of array work:

```c
#define SIZE 5
int scores[SIZE] = {10, 20, 30, 40, 50};

// Print every element
for (int i = 0; i < SIZE; i++) {
    printf("scores[%d] = %d\n", i, scores[i]);
}

// Sum all elements
int total = 0;
for (int i = 0; i < SIZE; i++) {
    total += scores[i];
}
printf("Total: %d\n", total);  // 150
```

> The condition is `i < SIZE`, **not** `i <= SIZE`.
> That keeps `i` in the valid range `0 .. SIZE-1`.


## 5. The Board: A 1D Array Representing a 2D Grid
Our tic-tac-toe board is **conceptually** a 3×3 grid, but it is stored as a **flat, 9-element array**.
Why? Because it is simpler to pass to functions and works for any board size.

```
Visual grid          Linear array indices

 0 | 1 | 2           board[0]  board[1]  board[2]
---+---+---    →     board[3]  board[4]  board[5]
 3 | 4 | 5           board[6]  board[7]  board[8]
---+---+---
 6 | 7 | 8
```

The project constants (`constants.h`):

```c
#define BOARD_DIM  3                        // Grid side length
#define BOARD_SIZE (BOARD_DIM * BOARD_DIM)  // 9 cells total for 3×3
#define WIN_LENGTH BOARD_DIM                // Symbols in a row needed to win
```

The board variable in game code:

```c
CellValue board[BOARD_SIZE];   // CellValue is an enum: CELL_EMPTY, CELL_X, CELL_O
```

### Converting Row/Column ↔ Index
```c
// row and col are both 0-based
int index = row * BOARD_DIM + col;

// Examples for a 3×3 board:
// Top-left     (row=0, col=0) → 0*3+0 = 0
// Centre       (row=1, col=1) → 1*3+1 = 4
// Bottom-right (row=2, col=2) → 2*3+2 = 8
```

Going the other way (index → row/column):

```c
int row = index / BOARD_DIM;   // integer division drops the remainder
int col = index % BOARD_DIM;   // remainder gives the column

// Example: index 7
// row = 7 / 3 = 2
// col = 7 % 3 = 1  → third row, middle column ✓
```

### Why a 1D Array and Not a 2D Array?
You *could* write `CellValue board[3][3]`, but a flat array:

- is simpler to pass to functions — one pointer, one size constant
- scales to **any** board dimension — change `BOARD_DIM` to `4` and the whole game rebuilds correctly
- works cleanly with `memset` and `memcpy` (contiguous memory)


## 6. Multi-Dimensional Arrays (2D and 3D)

Although this project uses a flat 1D array, C does support true multi-dimensional arrays.
Understanding the differences helps you choose the right tool for the job.

### Declaring 2D Arrays

```c
// A 3×3 grid of integers
int grid[3][3];

// With initialisation
int grid[3][3] = {
    {1, 2, 3},
    {4, 5, 6},
    {7, 8, 9}
};

// Accessing elements: grid[row][col]
int center = grid[1][1];   // 5
grid[0][2] = 42;           // top-right cell is now 42
```

### Declaring 3D Arrays

```c
// A 2×3×4 "cube" of integers (2 layers, 3 rows, 4 columns)
int cube[2][3][4];

// Accessing elements: cube[layer][row][col]
cube[0][1][2] = 99;
```

### Memory Layout — Still Contiguous

Multi-dimensional arrays are stored in **row-major order**: the rightmost index varies fastest.

```
int grid[2][3] = { {1,2,3}, {4,5,6} };

Memory layout (contiguous):
Address:  ...  +0   +4   +8  +12  +16  +20  ...
Value:          1    2    3    4    5    6
               ↑── row 0 ──↑  ↑── row 1 ──↑
```

This means `grid[0][2]` and `grid[1][0]` are adjacent in memory — one is at offset 8, the other at offset 12.

### Looping Over 2D Arrays

Use nested loops — outer loop for rows, inner loop for columns:

```c
#define ROWS 3
#define COLS 4

int matrix[ROWS][COLS] = {0};

for (int r = 0; r < ROWS; r++) {
    for (int c = 0; c < COLS; c++) {
        matrix[r][c] = r * COLS + c;   // fill with 0, 1, 2, ...
    }
}
```

### Passing Multi-Dimensional Arrays to Functions

Here is where 2D arrays get tricky. Unlike 1D arrays, you **must** specify all dimensions except the first:

```c
// ✅ Valid — column count is specified
void print_grid(int grid[][3], int rows) {
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < 3; c++) {
            printf("%d ", grid[r][c]);
        }
        printf("\n");
    }
}

// ❌ Invalid — compiler cannot calculate row offsets without column count
void print_grid(int grid[][], int rows, int cols);  // ERROR
```

The compiler needs the column count to compute `grid[r][c]` → `*(grid + r*COLS + c)`.

### 1D vs 2D — When to Use Which

| Consideration        | 1D Array                          | 2D Array                                         |
|----------------------|-----------------------------------|--------------------------------------------------|
| Simplicity           | Single index, one size constant   | Natural row/col syntax                           |
| Passing to functions | Easy — just a pointer + size      | Must hard-code all but first dimension           |
| Flexible dimensions  | Change one `#define` and rebuild  | Dimensions baked into type                       |
| `memset`/`memcpy`    | Works directly                    | Works (still contiguous), but syntax is clunkier |
| Conceptual fit       | Good for lists, queues, flat data | Good for grids, matrices, images                 |

**For this project**, the 1D approach wins because:
1. We pass the board to many functions — a flat array is simpler.
2. The board dimension might change (`BOARD_DIM = 4` for a 4×4 game) — a single constant handles it.
3. The row/col ↔ index conversion is trivial (`row * BOARD_DIM + col`).

> **Tip:** If you ever work with images, matrices, or game maps where the grid size is fixed at compile
> time, 2D arrays can make code more readable. For dynamic or variable-size grids, consider a 1D array
> with explicit stride calculations, or allocate memory at runtime (covered in [Lesson 9](LESSON_9_Memory_Management.md)).


## 7. Strings in C — A Special Kind of `char` Array

A string in C is just a `char` array with one rule: the last element must be `'\0'` (the **null terminator**, value `0`). This sentinel byte is how every string function knows where the string ends.

```
"Hello"  stored in memory:

  ┌───┬───┬───┬───┬───┬────┐
  │ H │ e │ l │ l │ o │ \0 │
  └───┴───┴───┴───┴───┴────┘
   [0] [1] [2] [3] [4] [5]      ← 5 visible characters + 1 terminator = 6 bytes
```

This is exactly the same memory layout as any other array — a contiguous block of `char` elements,
addressed by index. The only thing that makes it a "string" is the `'\0'` at the end.

### Declaring Strings

```c
// String literal — read-only, stored in the data segment
const char *greeting = "Hello";

// Character array — writable, stored on the stack
// Must be large enough for the characters PLUS the '\0'
char name[6] = "Hello";     // 5 chars + '\0' = 6 bytes needed

// Explicit array with initialiser list
char word[] = {'C', 'a', 't', '\0'};  // Same as "Cat"

// Size inferred from the literal (compiler counts for you)
char auto_sized[] = "Hello";  // compiler allocates 6 bytes
```

> **⚠️ Off-by-one danger:** Always allocate `length + 1` bytes to leave room for `'\0'`.
> `char name[5]` can only hold a 4-character string safely — the 5th byte must be `'\0'`.

### Reading Individual Characters

Because a string is a `char` array, you index it identically:

```c
const char *msg = "Hi!";

char first  = msg[0];   // 'H'
char second = msg[1];   // 'i'
char third  = msg[2];   // '!'
char term   = msg[3];   // '\0'  ← the terminator, always there
```

### String Literals vs. Character Arrays

This distinction trips up almost every C beginner:

```c
// 1. Pointer to a string literal — lives in read-only memory
const char *s1 = "Hello";
s1[0] = 'h';   // ❌ undefined behaviour — you can't modify a string literal

// 2. Character array — a writable copy on the stack
char s2[] = "Hello";
s2[0] = 'h';   // ✅ fine — s2 is your own copy
```

You'll notice `const char *` appearing whenever a string is passed to a function that only reads it.
The full explanation of what `const` and `*` each do requires pointer knowledge from
[Lesson 5](LESSON_5_Pointers.md) — a deeper look is in the More Advanced Topics section below.
For now, the practical rule is:

- Use `const char *` for a string you will **read**
- Use `char []` for a string you own and may **modify**


### Implicit String Concatenation

Adjacent string literals in C source code are **automatically joined** by the compiler into a
single string before your program runs. No function call, no operator — just whitespace between
the literals:

```c
const char *msg = "taco" " " "bell";
// Exactly the same as:
const char *msg = "taco bell";
```

The compiler splices them together, including a single `'\0'` at the very end:

```
"taco" " " "bell"

  t  a  c  o     b  e  l  l  \0
 [0][1][2][3][4][5][6][7][8][9]
```

This is a **compile-time** operation — there is zero runtime cost and no intermediate buffers.

#### Why This Matters

**1. Splitting long strings across lines** without needing a line continuation `\`:

```c
// Hard to read — one enormous literal
const char *error = "Error: position out of range. Valid positions are 0-8.";

// Clean — split at a natural boundary, identical result
const char *error = "Error: position out of range. "
                    "Valid positions are 0-8.";
```

**2. Building strings from macros:**

```c
#define PREFIX "Game: "

const char *msg = PREFIX "X wins!";   // → "Game: X wins!"
```

**3. `printf` format strings that would exceed line length:**

```c
printf("Player %s made move %d. "
       "Board has %d empty cells remaining.\n",
       name, move, empty);
```

#### Watch Out: Accidental Concatenation

The same mechanism can produce a subtle bug when you forget a comma in an array of strings:

```c
const char *options[] = {
    "Easy",
    "Medium"          // ← missing comma!
    "Hard",           // ← this silently merges with "Medium" → "MediumHard"
};
```

The compiler sees `"Medium" "Hard"` as a single literal `"MediumHard"` — no warning, no error.
Always double-check array initialisers of string literals for missing commas.



### Printing Strings

```c
const char *player = "Alice";
printf("Player: %s\n",        player);           // Player: Alice
printf("First char: %c\n",    player[0]);         // First char: A
printf("Length: %zu\n",       strlen(player));    // Length: 5
printf("Third letter: %c\n",  player[2]);         // Third letter: i
```

### In This Project

Strings appear mainly in the UI layer for displaying messages:

```c
// From src/ui_cli.c
static void cli_display_message(const char *message) {
    printf("%s\n", message);
}
```

`const char *message` means "a read-only string passed in from the caller". The `printf` reads it
character-by-character using `%s` until it hits `'\0'`. A full explanation of `const char *` is in
the [More Advanced Topics](#11-more-advanced-topics) section below.

---

## 8. Real Code — `src/board.c` Annotated

Every concept from this lesson appears in the project's `board.c`. Read through it and trace each idea:

```c
// ── board_init ──────────────────────────────────────────────────────────────
// memset fills the whole array in one shot (contiguous memory makes this fast)
void board_init(CellValue board[BOARD_SIZE]) {
    memset(board, CELL_EMPTY, BOARD_SIZE * sizeof(CellValue));
}

// ── board_is_empty ──────────────────────────────────────────────────────────
// const protects the array; direct index access reads one element
int board_is_empty(const CellValue board[BOARD_SIZE], int position) {
    return board[position] == CELL_EMPTY;
}

// ── board_place ─────────────────────────────────────────────────────────────
// Bounds check BEFORE indexing — never touch board[position] if it might be OOB
// Returns 0 on success, -1 on any error (idiomatic C error signalling)
int board_place(CellValue board[BOARD_SIZE], int position, CellValue symbol) {
    if (position < 0 || position >= BOARD_SIZE) {
        return -1;                   // out of range → reject
    }
    if (!board_is_empty(board, position)) {
        return -1;                   // already occupied → reject
    }
    board[position] = symbol;        // write through the pointer
    return 0;
}

// ── board_is_valid_move ─────────────────────────────────────────────────────
// Combines range check and emptiness check into one readable predicate
int board_is_valid_move(const CellValue board[BOARD_SIZE], int position) {
    if (position < 0 || position >= BOARD_SIZE) {
        return 0;
    }
    return board_is_empty(board, position);
}
```


## 9. Quick Reference

| Task                   | Code                                      |
|------------------------|-------------------------------------------|
| Declare                | `int arr[5];`                             |
| Declare + initialise   | `int arr[] = {1, 2, 3};`                  |
| Zero-initialise        | `int arr[5] = {0};`                       |
| Read element           | `arr[i]`                                  |
| Write element          | `arr[i] = 42;`                            |
| Last valid index       | `arr[SIZE - 1]`                           |
| Zero-fill (memset)     | `memset(arr, 0, SIZE * sizeof(int));`     |
| Copy (memcpy)          | `memcpy(dst, src, SIZE * sizeof(int));`   |
| Row/col → index        | `row * BOARD_DIM + col`                   |
| Index → row            | `index / BOARD_DIM`                       |
| Index → col            | `index % BOARD_DIM`                       |
| Declare string         | `char s[] = "Hello";`                     |
| String length          | `strlen(s)` — excludes `'\0'`             |
| Read-only string param | `const char *s`                           |

---

## 10. Practical Exercises
Try to answer each question *before* clicking to reveal. The goal is to predict — then verify.


### Exercise 1: Spot the Bug
What is wrong with this code? How would you fix it?

```c
int scores[5] = {10, 20, 30, 40, 50};

for (int i = 0; i <= 5; i++) {
    printf("%d\n", scores[i]);
}
```

<details>
<summary>Click to reveal answer</summary>

**Bug:** The loop condition `i <= 5` allows `i` to reach `5`, but the last valid index is `4`.
`scores[5]` is **out of bounds** — undefined behaviour.

**Fix:**
```c
for (int i = 0; i < 5; i++) {   // strictly less-than
    printf("%d\n", scores[i]);
}
```

The invariant to memorise: valid indices are `0` through `SIZE - 1`, so the loop condition is always `i < SIZE`.

</details>

---

### Exercise 2: Zero-Initialise an Array
Declare an integer array called `moves` of size `BOARD_SIZE` and ensure every element starts at `0`.
Write it two ways: using an initialiser list at declaration time, and using a `for` loop after declaration.

<details>
<summary>Click to reveal answer</summary>

```c
// Way 1 — initialiser list (only works at declaration time)
int moves[BOARD_SIZE] = {0};

// Way 2 — for loop (works any time, including mid-program resets)
int moves[BOARD_SIZE];
for (int i = 0; i < BOARD_SIZE; i++) {
    moves[i] = 0;
}
```

Both result in every element being `0`.

> **Preview:** once you've read the More Advanced Topics section you'll see a third way —
> `memset(moves, 0, BOARD_SIZE * sizeof(int))` — which fills the whole array in a single call by
> working directly on the underlying bytes.

</details>

---

### Exercise 3: Row/Column Conversion
Given a 3×3 board (`BOARD_DIM = 3`), answer without running the code:

1. What index corresponds to row `2`, column `0`?
2. What row and column does index `5` map to?

<details>
<summary>Click to reveal answer</summary>

**1. Row 2, column 0 → index:**
```
index = row * BOARD_DIM + col
      = 2 * 3 + 0
      = 6
```
That is the bottom-left cell.

**2. Index 5 → row and column:**
```
row = 5 / 3 = 1   (integer division)
col = 5 % 3 = 2
```
Row 1, column 2 — the rightmost cell of the middle row.

</details>

---

### Exercise 4: Count Empty Cells
You have a `CellValue board[BOARD_SIZE]` that has already been declared and populated.
Write a loop that counts how many cells are `CELL_EMPTY` and prints the result.

<details>
<summary>Click to reveal answer</summary>

```c
int count = 0;
for (int i = 0; i < BOARD_SIZE; i++) {
    if (board[i] == CELL_EMPTY) {
        count++;
    }
}
printf("Empty cells: %d\n", count);
```

Key points:
- Loop condition is `i < BOARD_SIZE` (not `<=`) — the last valid index is `BOARD_SIZE - 1`
- `count` starts at `0` and is incremented each time an empty cell is found
- Print *after* the loop, once the full count is known

</details>

---

### Exercise 5: Copy and Modify
You want to **try** a move without permanently altering the real board (essential for the AI).
Write code that:
1. Copies `board` into a new array called `trial`
2. Places `CELL_X` at position `4` in `trial`
3. Leaves the original `board` unchanged

<details>
<summary>Click to reveal answer</summary>

```c
CellValue trial[BOARD_SIZE];

// 1. Copy the board element-by-element
for (int i = 0; i < BOARD_SIZE; i++) {
    trial[i] = board[i];
}

// 2. Place X in the copy
trial[4] = CELL_X;

// 3. 'board' is untouched — we only wrote to 'trial'
```

This pattern (copy → mutate copy → evaluate) is exactly how the minimax AI explores future game
states without destroying the current one.

> **Preview:** once you've read the More Advanced Topics section you'll see a one-liner —
> `memcpy(trial, board, BOARD_SIZE * sizeof(CellValue))` — that copies the whole array in a
> single call by working directly on the underlying bytes.

</details>

---

### Exercise 6: Find the First Empty Cell
You have a `CellValue board[BOARD_SIZE]` that has already been populated.
Write a loop that finds the **index** of the first empty cell and stores it in a variable called
`first`. If no empty cell exists, `first` should be `-1`.

<details>
<summary>Click to reveal answer</summary>

```c
int first = -1;   // assume "not found" until proven otherwise

for (int i = 0; i < BOARD_SIZE; i++) {
    if (board[i] == CELL_EMPTY) {
        first = i;
        break;    // stop as soon as we find the first match
    }
}

printf("First empty cell: %d\n", first);   // -1 means the board is full
```

`-1` as a "not found" sentinel is idiomatic C — you will see this pattern throughout the project
(e.g. `board_place` returns `-1` on failure). The `break` is important: without it the loop would
keep running and `first` would end up as the *last* empty index, not the first.

</details>

---

### Exercise 7: Does a Row Belong to One Player?
You have a `CellValue board[BOARD_SIZE]` and two variables already declared:

```c
int row = 1;          // the row you want to check (0-based)
CellValue player = CELL_X;  // the player to check for
```

Write a loop that sets an `int` variable called `belongs` to `1` if every cell in `row` is owned
by `player`, or `0` if any cell differs. Use the row-to-index formula from section 5.

<details>
<summary>Click to reveal answer</summary>

```c
int belongs = 1;       // optimistically assume the row belongs to player

for (int col = 0; col < BOARD_DIM; col++) {
    int index = row * BOARD_DIM + col;
    if (board[index] != player) {
        belongs = 0;   // found a cell that does not match — disprove the assumption
        break;         // no need to check further
    }
}

printf("Row %d belongs to player: %d\n", row, belongs);
```

This "optimistic flag" pattern — assume true, disprove on first mismatch — is a very common loop
idiom in C. It is a simplified version of what the win-condition calculator does for every row,
column, and diagonal on the full board.

</details>

---

### Exercise 8: Declare and Initialise a 2D Array
Declare a 3×3 integer grid and initialise it so the diagonal contains `1`s and everything else is `0`:

```
1  0  0
0  1  0
0  0  1
```

<details>
<summary>Click to reveal answer</summary>

```c
int grid[3][3] = {
    {1, 0, 0},
    {0, 1, 0},
    {0, 0, 1}
};
```

Alternatively, you can zero-initialise and then set the diagonal:

```c
int grid[3][3] = {0};   // all zeros

for (int i = 0; i < 3; i++) {
    grid[i][i] = 1;     // diagonal: [0][0], [1][1], [2][2]
}
```

</details>

---

### Exercise 9: Sum All Elements in a 2D Array
You have a 2×4 matrix already declared:

```c
int matrix[2][4] = {
    {1, 2, 3, 4},
    {5, 6, 7, 8}
};
```

Write nested loops to compute the sum of all elements and store it in a variable called `total`.

<details>
<summary>Click to reveal answer</summary>

```c
int total = 0;

for (int r = 0; r < 2; r++) {
    for (int c = 0; c < 4; c++) {
        total += matrix[r][c];
    }
}

printf("Total: %d\n", total);   // 36
```

Key points:
- Outer loop iterates over rows (`r < 2`)
- Inner loop iterates over columns (`c < 4`)
- The order matters for clarity, but since the array is contiguous in memory, you could also iterate
  in column-major order — though row-major (as shown) is conventional in C

</details>

---

### Exercise 10: 1D Index to 2D Coordinates
Given a flat array with 12 elements representing a 3×4 grid (3 rows, 4 columns), write expressions
to convert a 1D index to 2D row/column coordinates.

If `index = 7`, what are the row and column?

<details>
<summary>Click to reveal answer</summary>

```c
#define ROWS 3
#define COLS 4

int index = 7;
int row = index / COLS;   // 7 / 4 = 1
int col = index % COLS;   // 7 % 4 = 3
```

So index `7` corresponds to **row 1, column 3** (the last cell of the second row).

To go the other way:
```c
int index = row * COLS + col;   // 1 * 4 + 3 = 7 ✓
```

This is the same formula from Section 5, generalised — the divisor/multiplier is always the
number of **columns** (the inner dimension).

</details>

---

### Exercise 11: Why Does This Function Signature Fail?
Explain why the following function signature will not compile:

```c
void process_grid(int grid[][], int rows, int cols) {
    // ...
}
```

How would you fix it if the grid always has 4 columns?

<details>
<summary>Click to reveal answer</summary>

**Why it fails:**
The compiler needs to know the column count to compute the memory offset for `grid[r][c]`.
Internally, it calculates `*(grid + r * COLS + c)` — without knowing `COLS`, it cannot generate
the correct address arithmetic.

**Fix — hard-code the column count:**
```c
void process_grid(int grid[][4], int rows) {
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < 4; c++) {
            printf("%d ", grid[r][c]);
        }
        printf("\n");
    }
}
```

**Alternative — use a 1D array with explicit stride** *(requires [Lesson 5: Pointers](LESSON_5_Pointers.md))*:
```c
void process_grid(int *grid, int rows, int cols) {
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            printf("%d ", grid[r * cols + c]);
        }
        printf("\n");
    }
}
```

The 1D approach is more flexible because `rows` and `cols` can be any value — this is why the
tic-tac-toe project uses a flat array.

</details>

---

### Exercise 12: Transpose a 3×3 Grid (Challenge)
You have a 3×3 grid:

```c
int grid[3][3] = {
    {1, 2, 3},
    {4, 5, 6},
    {7, 8, 9}
};
```

Write code to **transpose** it in place — swap rows and columns so the result is:

```
1  4  7
2  5  8
3  6  9
```

*Hint:* You only need to swap elements where `row < col` to avoid swapping twice.

<details>
<summary>Click to reveal answer</summary>

```c
for (int r = 0; r < 3; r++) {
    for (int c = r + 1; c < 3; c++) {   // only upper triangle
        int temp = grid[r][c];
        grid[r][c] = grid[c][r];
        grid[c][r] = temp;
    }
}
```

Why `c = r + 1`?
- When `r = 0`, we swap `[0][1]↔[1][0]` and `[0][2]↔[2][0]`
- When `r = 1`, we swap `[1][2]↔[2][1]`
- When `r = 2`, the inner loop doesn't run (nothing left to swap)

If you used `c = 0` instead, you'd swap each pair twice, returning to the original!

The diagonal (`[0][0]`, `[1][1]`, `[2][2]`) stays in place — transposing doesn't move it.

</details>

---

### Exercise 13: The Null Terminator
Answer these questions without running any code.

Given:
```c
char word[] = "Cat";
```

1. How many bytes does `word` occupy in memory?
2. What is the value of `word[3]`?
3. What happens if you declare `char broken[3] = "Cat"` instead — why is this dangerous?

<details>
<summary>Click to reveal answer</summary>

1. **4 bytes** — `'C'`, `'a'`, `'t'`, `'\0'`. A string of length *n* always needs *n + 1* bytes.
2. **`'\0'`** (value `0`) — the null terminator. It is always at index `length`.
3. `char broken[3] = "Cat"` fits the three characters but leaves **no room for `'\0'`**. The array
   is not a valid C string — functions like `strlen` or `printf("%s")` will read past the end of the
   array looking for a terminator that isn't there, producing garbage output or a crash.

</details>

---

### Exercise 14: Indexing Into a String
Given:
```c
const char *msg = "Hello!";
```

Predict the value of each expression, then explain *why* without running the code:

```c
msg[0]          // (a)
msg[4]          // (b)
msg[5]          // (c)
msg[6]          // (d)
```

<details>
<summary>Click to reveal answer</summary>

```
msg[0]  →  'H'    ← first character
msg[4]  →  'o'    ← fifth character (0-indexed)
msg[5]  →  '!'    ← last visible character
msg[6]  →  '\0'   ← null terminator; the string is 6 chars + 1 terminator = 7 bytes
```

A string is just a `char` array — indexing it is identical to indexing `int scores[]`.
The terminator lives at `msg[strlen(msg)]`, i.e. one past the last visible character.

</details>

---

### Exercise 15: Literals, Arrays, and Implicit Concatenation
Each code snippet below either compiles and runs correctly, has undefined behaviour, or produces a
surprising result. Classify each one and explain why.

```c
// (a)
const char *s = "Hello";
s[0] = 'h';

// (b)
char s[] = "Hello";
s[0] = 'h';

// (c)
const char *msg = "Good" "bye" "!";
printf("%zu\n", strlen(msg));   // what does this print?

// (d)
const char *options[] = {
    "Rock",
    "Paper"
    "Scissors"
};
printf("%s\n", options[1]);     // what does this print?
```

<details>
<summary>Click to reveal answer</summary>

**(a) Undefined behaviour.**
`s` is a pointer to a string *literal*, which lives in read-only memory. Writing to `s[0]` is
undefined behaviour — on most systems it causes a segmentation fault at runtime. The fix is to
use `const char *s` and never write to it, or copy it into a `char` array first.

**(b) Correct.**
`char s[] = "Hello"` creates a writable *copy* of the literal on the stack. `s[0] = 'h'` modifies
that local copy — perfectly legal. After the assignment `s` holds `"hello"`.

**(c) Prints `8`.**
Adjacent string literals are joined at compile time: `"Good" "bye" "!"` → `"Goodbye!"`.
`strlen("Goodbye!")` = 8. There is zero runtime cost.

**(d) Prints `PaperScissors`.**
The comma after `"Paper"` is **missing**, so the compiler silently concatenates `"Paper"` and
`"Scissors"` into a single literal `"PaperScissors"`. The array has only two elements:
`options[0]` = `"Rock"`, `options[1]` = `"PaperScissors"`.
This is the accidental-concatenation bug — no warning, no error.

</details>

---

## Next Steps

Continue to [Lesson 3: Varieties of Named Constants](LESSON_3_Named_Constants.md) →

← Previous: [Lesson 1: Control Flow and Loops](LESSON_1_Control_Flow_And_Loops.md)

← Back to [Learning Guide Index](LEARNING_GUIDE.md)

---

## Bonus: Advanced Topics

> **📚 Come back later!** The topics below build on concepts from later lessons.
> Read them now for a preview, or bookmark this section and return after completing:
> - [Lesson 4: Functions](LESSON_4_Functions.md) — for understanding parameter passing and `const`
> - [Lesson 5: Pointers](LESSON_5_Pointers.md) — for understanding why arrays and strings "decay" to pointers


### What `const char *` Actually Means

Reading a pointer declaration **right-to-left** tells you exactly what it means:

```
const char *message
      ↑      ↑
      │      └── pointer to...
      └───────── ...a char that is const (read-only)
```

The `const` qualifies the `char`, not the pointer itself. It means:
- ✅ You can read `message[0]`, `message[1]`, etc.
- ❌ You cannot write `message[0] = 'x'`
- ✅ You can point `message` at a different string entirely

This is exactly what you want when receiving a string you should only read. The compiler enforces
it for you — any accidental write becomes a compile-time error rather than a runtime crash:

```c
void shout(const char *word) {
    printf("%s!\n", word);   // ✅ reading is fine
    word[0] = 'X';           // ❌ compiler error: assignment of read-only location
}
```

There is a second form — `char * const` — where the pointer itself is constant but the characters
are writable. You will rarely need this; it is covered fully in [Lesson 5: Pointers](LESSON_5_Pointers.md).

**Rule of thumb:**

| Form                   | What's const | Meaning                                     |
|------------------------|--------------|---------------------------------------------|
| `const char *s`        | the chars    | can't modify the string, can re-point `s`   |
| `char * const s`       | the pointer  | can modify the string, can't re-point `s`   |
| `const char * const s` | both         | fully read-only pointer to read-only string |


### Common String Functions (`<string.h>`)
These functions treat a `char` array as a string by scanning for `'\0'`:

```c
#include <string.h>

char a[] = "Hello";
char b[] = "World";
char buf[12];

strlen(a);            // 5  — number of characters, NOT counting '\0'
strcpy(buf, a);       // copies "Hello" into buf  → "Hello"
strcat(buf, " ");     // appends a space           → "Hello "
strcat(buf, b);       // appends "World"           → "Hello World"
strcmp(a, b);         // < 0 (a comes before b alphabetically)
```

> **⚠️ `strcpy`/`strcat` are unsafe** if the destination buffer is too small — they will write past
> the end (a **buffer overflow**). Prefer `strncpy` / `strncat` / `snprintf` when the length is not
> known at compile time.

Understanding *why* these are unsafe requires knowing how pointers work: each function receives only
a pointer to the first character, with no length information attached. It trusts the caller to have
allocated enough space. You'll fully appreciate this after [Lesson 5: Pointers](LESSON_5_Pointers.md).


### Arrays in Memory — Why Contiguous Matters
Because all elements sit next to each other, you can fill or copy an entire array with a single call:

```c
#include <string.h>

CellValue board[BOARD_SIZE];

// Zero-fill every element (safe because CELL_EMPTY = 0, which is all-zero bytes)
memset(board, 0, BOARD_SIZE * sizeof(CellValue));

// Copy one board to another (e.g. saving game state for the AI)
CellValue backup[BOARD_SIZE];
memcpy(backup, board, BOARD_SIZE * sizeof(CellValue));
```

> **`sizeof` note:** Always multiply by `sizeof(element_type)`.
> `BOARD_SIZE` counts *elements*; `memset`/`memcpy` need *bytes*.


### Arrays and Pointers — What Really Happens
When you pass an array to a function, C does **not** copy the whole array. It passes a **pointer** to the first element.
These three signatures are identical to the compiler:

```c
void print_board(CellValue board[BOARD_SIZE]);  // looks like an array…
void print_board(CellValue board[]);            // …size is ignored…
void print_board(CellValue *board);             // …it is just a pointer
```

This has two important consequences:

**1. Functions can modify the caller's array:**

```c
void set_center(CellValue board[BOARD_SIZE]) {
    board[4] = CELL_X;          // modifies the CALLER's array
}

CellValue my_board[BOARD_SIZE] = {0};
set_center(my_board);           // my_board[4] is now CELL_X
```

**2. The function does not know the array's size:**

```c
void wrong(int arr[]) {
    // sizeof(arr) is the size of a POINTER (8 bytes on 64-bit), not the array!
    int n = sizeof(arr) / sizeof(arr[0]);   // ❌ Always gives 2 — wrong!
}
```

**Fix:** Pass the size as a separate parameter, or use a named constant:

```c
void right(CellValue board[], int size) {       // ✅ explicit count
    for (int i = 0; i < size; i++) { ... }
}

void board_init(CellValue board[BOARD_SIZE]) {
    for (int i = 0; i < BOARD_SIZE; i++) { ... }  // ✅ constant is visible
}
```


### `const` — Protecting Array Contents

Adding `const` to a parameter tells the compiler (and the next developer) that this function will only
**read** the array, never write to it:

```c
// board_is_empty promises NOT to change the board
int board_is_empty(const CellValue board[BOARD_SIZE], int position) {
    return board[position] == CELL_EMPTY;
}
```

If you accidentally try to write to a `const` parameter, the compiler will stop you:

```c
int read_only(const int arr[], int size) {
    arr[0] = 99;    // ❌ compiler error: assignment of read-only location
    return arr[0];  // ✅ fine
}
```

**Rule of thumb:** use `const` whenever a function only needs to *read* the array.
It documents intent and turns accidental writes into compile-time errors.
