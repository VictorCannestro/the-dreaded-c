# C Learning Guide - Tic-Tac-Toe Project

This guide explains the key C concepts used in this project, with explanations for people new to C.

## Table of Contents
1. [Basic Types](#basic-types)
2. [Enums](#enums)
3. [Structs](#structs)
4. [Arrays](#arrays)
5. [Pointers](#pointers)
6. [Functions](#functions)
7. [Header Files](#header-files)
8. [Memory Management](#memory-management)

---

## Basic Types

C has several built-in types for storing data:

```c
int count = 5;           // Whole numbers (32-bit typically)
float price = 9.99;      // Decimal numbers (less precise)
double value = 3.14159;  // Decimal numbers (more precise)
char letter = 'A';       // Single character
void func(void);         // No return value or parameters
```

In our project:
```c
// In tictactoe.h
typedef int GameStatus;  // We define our own type aliases
```

---

## Enums

An `enum` (enumeration) is a way to give meaningful names to integer values:

```c
typedef enum {
    CELL_EMPTY = 0,
    CELL_X = 1,
    CELL_O = 2
} CellValue;
```

**Why use enums?**
- Instead of: `if (board[0] == 1)` // What does 1 mean?
- We write: `if (board[0] == CELL_X)` // Clear!

**In our project** (see [include/tictactoe.h](include/tictactoe.h)):
```c
typedef enum {
    GAME_ONGOING = 0,
    GAME_X_WINS = 1,
    GAME_O_WINS = 2,
    GAME_DRAW = 3
} GameStatus;
```

---

## Structs

A `struct` (structure) groups related data together:

```c
typedef struct {
    int age;
    float height;
    char name[50];
} Person;

// Usage:
Person john;
john.age = 30;
john.height = 5.9;
```

**In our project** (see [include/tictactoe.h](include/tictactoe.h)):
```c
typedef struct {
    CellValue board[9];  // The 3x3 game board
    GameStatus status;   // Current game state
    int move_count;      // How many moves have been made
} GameState;
```

**Why is this useful?**
- Keeps related data together
- Easier to pass to functions
- More organized than separate variables

---

## Arrays

Arrays store multiple values of the same type in order:

```c
int numbers[5];           // Array of 5 integers
numbers[0] = 10;          // First element (0-indexed!)
numbers[4] = 50;          // Last element

// Multi-dimensional arrays
int matrix[3][3];         // 3x3 grid
matrix[0][0] = 1;         // Row 0, Column 0
matrix[2][1] = 5;         // Row 2, Column 1
```

**Important:** Array indexing starts at 0!
- `array[0]` is the FIRST element
- `array[8]` is the 9th element

**In our project** (see [src/tictactoe.c](src/tictactoe.c)):
```c
CellValue board[9];  // Linear array for 3x3 board
// Position mapping:
//   0 | 1 | 2
//  -----------
//   3 | 4 | 5
//  -----------
//   6 | 7 | 8

// To convert row,col to position:
int position = row * 3 + col;  // Example: row=1, col=1 → position=4 (center)
```

---

## Pointers

A pointer is a variable that stores the memory address of another variable:

```c
int x = 42;
int *ptr = &x;     // ptr points to x (&x means "address of x")

printf("%d\n", *ptr);   // Prints: 42 (*ptr means "value at that address")
printf("%p\n", ptr);    // Prints memory address (like: 0x7ffc...)
```

### Why use pointers?

1. **Pass by reference** - Functions can modify the original variable:
```c
void increment(int *num) {
    *num = *num + 1;    // Modify the original
}

int value = 5;
increment(&value);      // value is now 6
```

2. **Return multiple values**:
```c
void divide_with_remainder(int a, int b, int *quotient, int *remainder) {
    *quotient = a / b;
    *remainder = a % b;
}
```

**In our project** (see [src/tictactoe.c](src/tictactoe.c)):
```c
void game_init(GameState *state) {
    if (state == NULL) return;  // Safety check
    
    memset(state->board, CELL_EMPTY, 9 * sizeof(CellValue));
    // state-> means "access member through pointer"
    // Equivalent to: (*state).board
}
```

### Pointer Syntax Cheat Sheet
```c
int *ptr;           // Declare a pointer to int
int *ptr = &var;    // Initialize pointer to address of var
*ptr = 10;          // Dereference: set the value at that address
int val = *ptr;     // Dereference: read the value at that address
ptr->member;        // Access struct member through pointer
(*ptr).member;      // Same as above (less common)
```

---

## Functions

Functions are reusable blocks of code:

```c
// Function declaration (in header file)
int add(int a, int b);

// Function definition (in source file)
int add(int a, int b) {
    return a + b;
}

// Usage
int result = add(3, 4);  // result is 7
```

### Return Values

```c
void print_message(void) {
    printf("Hello\n");
    // No return statement means void return
}

int get_number(void) {
    return 42;
}
```

### Parameters vs Arguments
```c
void function(int parameter) {  // parameter = variable in function
    // ...
}

function(5);  // 5 = the argument passed to the function
```

**In our project** (see [include/tictactoe.h](include/tictactoe.h)):
```c
int game_make_move(GameState *state, int position);
//  ↑ return type    ↑ parameter 1    ↑ parameter 2

// Returns 0 on success, -1 on failure
```

---

## Header Files

Header files (`.h`) contain declarations; source files (`.c`) contain implementations:

### Header File (`.h`)
```c
// tictactoe.h - What functions are available
#ifndef TICTACTOE_H
#define TICTACTOE_H

void game_init(GameState *state);
int game_make_move(GameState *state, int position);

#endif
```

### Source File (`.c`)
```c
// tictactoe.c - How functions work
#include "tictactoe.h"

void game_init(GameState *state) {
    // Implementation here
}

int game_make_move(GameState *state, int position) {
    // Implementation here
}
```

**Why separate files?**
- Users only need to see what functions exist (header)
- Implementation details are hidden (source)
- Prevents accidental circular dependencies
- Easier to maintain large projects

### Include Guards
```c
#ifndef TICTACTOE_H
#define TICTACTOE_H
// ... content ...
#endif
```
Prevents the same header from being included twice (which would cause errors).

---

## Memory Management

### Stack vs Heap

**Stack:** Automatic memory (fast, limited size)
```c
GameState game;  // Automatically cleaned up when variable goes out of scope
game_init(&game);
```

**Heap:** Manual memory (slow, larger size)
```c
GameState *game = malloc(sizeof(GameState));  // Allocate
game_init(game);
free(game);  // Must manually free!
```

**In our project:** We use stack allocation for simplicity. The interactive game in [src/main.c](src/main.c):
```c
GameState game;       // Stack allocation
game_init(&game);     // Initialize
game_make_move(&game, position);
```

### Common Errors

```c
int *ptr;           // Uninitialized pointer - DANGEROUS!
*ptr = 5;           // Undefined behavior - likely a crash

int x = 5;
int *ptr = &x;
// x goes out of scope
*ptr;               // Now ptr points to invalid memory!

free(ptr);          // Freeing unallocated memory - CRASH!
ptr = NULL;         // Good practice after freeing
```

---

## Common C Patterns in This Project

### 1. NULL Pointer Checks
```c
void game_init(GameState *state) {
    if (state == NULL) return;  // Defensive programming
    // ...
}
```
Always check pointers for NULL before using them!

### 2. Error Return Values
```c
int game_make_move(GameState *state, int position) {
    if (state == NULL || position < 0 || position > 8) {
        return -1;  // Error code
    }
    // Success code: return 0
    return 0;
}
```

### 3. Static Functions (File-Local Functions)
```c
// In tictactoe.c
static CellValue check_winner(const CellValue board[9]) {
    // This function is only visible in this file
    // Not accessible from other files
}
```
Helps with organization and prevents accidental usage.

### 4. Const Correctness
```c
// Function doesn't modify the board
void display_board(const GameState *state) {
    // Compiler error if we try: state->board[0] = CELL_X;
}
```

---

## Testing Concepts (Unity Framework)

Tests verify that functions work correctly:

```c
void test_game_init_creates_empty_board(void) {
    GameState game;
    game_init(&game);
    
    // Assert that board is empty
    for (int i = 0; i < 9; i++) {
        TEST_ASSERT_EQUAL(CELL_EMPTY, game.board[i]);
    }
}
```

**Why write tests?**
- Catch bugs early
- Document how code should behave
- Safe to refactor (tests guarantee nothing broke)
- Cheaper than finding bugs in production

See [tests/test_tictactoe.c](tests/test_tictactoe.c) for 15+ examples.

---

## Debugging Tips

### 1. Print Everything
```c
printf("DEBUG: position = %d\n", position);
printf("DEBUG: board[0] = %d\n", board[0]);
printf("DEBUG: state = %p\n", (void*)state);  // Print pointer address
```

### 2. Use a Debugger (GDB on Linux/macOS, Visual Studio on Windows)
```bash
# Compile with debug symbols
cmake .. -DCMAKE_BUILD_TYPE=Debug
make
gdb ./tictactoe
(gdb) break main
(gdb) run
(gdb) print variable_name
```

### 3. Add Assertions
```c
#include <assert.h>

void game_make_move(GameState *state, int position) {
    assert(state != NULL);
    assert(position >= 0 && position < 9);
    // ...
}
```

### 4. Static Analysis
```bash
# Check for common mistakes
cppcheck src/
```

---

## Next Steps

1. **Modify the game:**
   - Change board size to 4x4
   - Add scoring
   - Implement AI player

2. **Improve the code:**
   - Add more comments
   - Add input validation
   - Add logging

3. **Learn more:**
   - Read the source files with these concepts in mind
   - Try creating a new function (follow existing patterns)
   - Write tests for your new function

---

## Additional Resources

- **Official C Standard**: ISO/IEC 9899:2018 (complex, formal)
- **Beej's Guide to C**: https://beej.us/guide/bgc/ (beginner-friendly)
- **K&R C**: "The C Programming Language" (classic textbook)
- **C Reference**: https://en.cppreference.com/w/c (quick lookup)

Happy learning! 🚀
