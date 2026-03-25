# C Learning Guide - Tic-Tac-Toe Project

This guide explains the key C concepts and software engineering practices used in this project, with explanations for people new to C.

## Table of Contents

### Core C Concepts
1. [Basic Types](#basic-types)
2. [Enums](#enums)
3. [Structs](#structs)
4. [Arrays](#arrays)
5. [Pointers](#pointers)
6. [Functions](#functions)
7. [Header Files](#header-files)
8. [Function Pointers and Abstraction](#function-pointers-and-abstraction)
9. [Memory Management](#memory-management)

### Software Engineering Practices
10. [Separation of Concerns](#separation-of-concerns)
11. [API Design](#api-design)
12. [Defensive Programming](#defensive-programming)
13. [Testing](#testing)
14. [Code Smells and Refactoring](#code-smells-and-refactoring)

### Common Pitfalls
15. [C Pitfalls and How to Avoid Them](#c-pitfalls-and-how-to-avoid-them)

---

## Basic Types

C provides fundamental data types for storing different kinds of values:

| Type     | Size (typical) | Range       | Use Case                          |
|----------|----------------|-------------|-----------------------------------|
| `char`   | 1 byte         | -128 to 127 | Characters, small numbers         |
| `int`    | 4 bytes        | ±2 billion  | General integers                  |
| `float`  | 4 bytes        | ±3.4×10³⁸   | Decimals (6-7 digits precision)   |
| `double` | 8 bytes        | ±1.7×10³⁰⁸  | Decimals (15-16 digits precision) |

```c
int move_count = 0;      // Counting moves in the game
char marker = 'X';       // Single character for display
double score = 95.5;     // If we added scoring
```

### Special Types

```c
void game_init(void);    // void = "nothing" (no return / no parameters)
```

### Type Sizes Can Vary!

C doesn't guarantee exact sizes. Use `<stdint.h>` when you need specific sizes:

```c
#include <stdint.h>

int8_t   small = 127;      // Exactly 8 bits, signed
uint32_t large = 4000000;  // Exactly 32 bits, unsigned
```

**In this project:** We use `int` for simplicity since exact sizes don't matter for a game.

---

## Enums

An `enum` (enumeration) creates a set of named integer constants. Instead of remembering that `1` means X and `2` means O, you use meaningful names.

### Syntax

```c
typedef enum {
    CELL_EMPTY = 0,   // Explicit values (optional but recommended)
    CELL_X = 1,
    CELL_O = 2
} CellValue;          // Type name

CellValue cell = CELL_X;  // Usage
```

### Why Enums Are Essential

**❌ Without enums (confusing):**
```c
if (board[0] == 1) { ... }   // What is 1? Who knows!
board[4] = 2;                 // Setting to... something?
```

**✅ With enums (self-documenting):**
```c
if (board[0] == CELL_X) { ... }   // Checking for X
board[4] = CELL_O;                 // Placing O at center
```

### All Enums in This Project

**Cell values** (what's in each board position):
```c
typedef enum {
    CELL_EMPTY = 0,
    CELL_X = 1,
    CELL_O = 2
} CellValue;
```

**Game status** (is the game over?):
```c
typedef enum {
    ONGOING = 0,
    X_WINS = 1,
    O_WINS = 2,
    DRAW = 3
} GameStatus;
```

**Player type** (human or computer?):
```c
typedef enum {
    PLAYER_HUMAN = 0,
    PLAYER_COMPUTER = 1
} PlayerType;
```

**Difficulty level**:
```c
typedef enum {
    DIFFICULTY_EASY = 0,
    DIFFICULTY_HARD = 1
} Difficulty;
```

### Clever Enum Trick: Using Values as Indices

```c
// CELL_X = 1, CELL_O = 2
// players[0] = X's info, players[1] = O's info

PlayerType game_get_player_type(GameState *state, CellValue symbol) {
    return state->players[symbol - 1].type;  // CELL_X-1=0, CELL_O-1=1
}
```

See [include/constants.h](include/constants.h) for all enum definitions.

---

## Structs

A `struct` (structure) groups related data together into a single unit. Think of it as a custom type that bundles multiple variables.

### Syntax

```c
typedef struct {
    int x;
    int y;
} Point;

Point p;       // Declare a Point variable
p.x = 10;      // Access members with dot notation
p.y = 20;
```

### Structs in This Project

**Player** - Who is playing (human or computer)?
```c
typedef struct {
    PlayerType type;    // PLAYER_HUMAN or PLAYER_COMPUTER
    CellValue symbol;   // CELL_X or CELL_O
} Player;
```

**GameState** - Everything about the current game:
```c
typedef struct {
    CellValue board[BOARD_SIZE]; // The NxN board (configurable size)
    GameStatus status;           // ONGOING, X_WINS, O_WINS, or DRAW
    int move_count;              // How many moves made
    Player players[2];           // players[0]=X, players[1]=O
    int game_count;              // Which game number in session
    CellValue last_winner;       // Who won the previous game
    Difficulty difficulty;       // DIFFICULTY_EASY or DIFFICULTY_HARD
} GameState;
```

### Accessing Struct Members

**Direct access** (when you have the struct itself):
```c
GameState game;
game.move_count = 0;
game.board[4] = CELL_X;
```

**Pointer access** (when you have a pointer to the struct):
```c
GameState *state = &game;
state->move_count = 0;      // Arrow notation for pointers
state->board[4] = CELL_X;

// Equivalent but more verbose:
(*state).move_count = 0;
```

### Nested Structs

Structs can contain other structs:
```c
// GameState contains an array of Player structs
state->players[0].type = PLAYER_HUMAN;
state->players[0].symbol = CELL_X;
state->players[1].type = PLAYER_COMPUTER;
state->players[1].symbol = CELL_O;
```

### Why Use Structs?

1. **Organization** - Related data stays together
2. **Pass to functions easily** - One parameter instead of many
3. **Return multiple values** - Return a struct with all results
4. **Self-documenting** - Field names explain what data means

See [include/tictactoe.h](include/tictactoe.h) for struct definitions.

---

## Arrays

Arrays store multiple values of the same type in contiguous memory.

### Declaring and Using Arrays

```c
int numbers[5];          // Array of 5 integers (indices 0-4)
numbers[0] = 10;         // First element
numbers[4] = 50;         // Last element (NOT numbers[5]!)

// Initialize at declaration
int primes[] = {2, 3, 5, 7, 11};  // Size inferred: 5 elements
```

### ⚠️ Critical: Arrays Are 0-Indexed!

```
Index:    [0]  [1]  [2]  [3]  [4]
Values:    10   20   30   40   50
```

- First element: `array[0]`
- Last element of size-N array: `array[N-1]`
- `array[N]` is **OUT OF BOUNDS** (undefined behavior!)

### The Board: 1D Array as 2D Grid

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

### Arrays and Pointers

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

### Common Array Operations

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

See [src/board.c](src/board.c) for array operations.

---

## Pointers

A **pointer** stores the memory address of another variable. This is one of C's most powerful and most confusing features.

### Visual Model

```
Variable x:     ┌───────┐
                │  42   │  ← value
                └───────┘
                0x7ffc   ← address (memory location)

Pointer ptr:    ┌───────┐
                │0x7ffc │  ← stores the ADDRESS of x
                └───────┘
```

### Pointer Operators

| Operator | Name | Meaning |
|----------|------|---------|
| `&` | Address-of | Get the address of a variable |
| `*` | Dereference | Get the value at an address |
| `->` | Arrow | Access struct member through pointer |

### Basic Example

```c
int x = 42;
int *ptr = &x;      // ptr holds the address of x

printf("%d\n", x);      // 42 (the value)
printf("%p\n", &x);     // 0x7ffc... (address of x)
printf("%p\n", ptr);    // 0x7ffc... (same address)
printf("%d\n", *ptr);   // 42 (value at that address)

*ptr = 100;             // Modify x through the pointer
printf("%d\n", x);      // 100 (x changed!)
```

### Why Pointers Matter

**1. Functions Can Modify Variables (Pass by Reference)**

```c
// Without pointer - value is copied, original unchanged
void broken_increment(int num) {
    num = num + 1;  // Only modifies the copy
}

// With pointer - function modifies the original
void increment(int *num) {
    *num = *num + 1;  // Modifies the actual variable
}

int value = 5;
broken_increment(value);   // value still 5
increment(&value);         // value now 6
```

**2. Avoid Copying Large Data**

```c
// BAD: Copies entire GameState (expensive if large)
void display_board_bad(GameState state) { ... }

// GOOD: Only passes 8-byte address
void display_board(GameState *state) { ... }
```

**3. Return Multiple Values**

```c
void get_min_max(int arr[], int size, int *min, int *max) {
    *min = arr[0];
    *max = arr[0];
    for (int i = 1; i < size; i++) {
        if (arr[i] < *min) *min = arr[i];
        if (arr[i] > *max) *max = arr[i];
    }
}

int minimum, maximum;
get_min_max(numbers, 5, &minimum, &maximum);
```

### Pointers in This Project

Almost every function takes a `GameState *` pointer:

```c
void game_init_session(GameState *state) {
    if (state == NULL) return;  // Always check for NULL!
    
    state->status = ONGOING;        // Arrow: access through pointer
    state->move_count = 0;
    board_init(state->board);       // Pass board array to another function
}
```

### Struct Pointer Syntax

```c
GameState game;
GameState *ptr = &game;

// These are equivalent:
(*ptr).status = ONGOING;   // Dereference, then access member
ptr->status = ONGOING;     // Arrow notation (preferred)
```

### Common Pointer Mistakes

```c
int *ptr;                // ❌ Uninitialized - points to garbage!
*ptr = 5;                // ❌ CRASH: writing to random memory

int *ptr = NULL;         // ✅ Initialize to NULL if no value yet
if (ptr != NULL) {       // ✅ Check before using
    *ptr = 5;
}
```

See [src/tictactoe.c](src/tictactoe.c) for pointer usage throughout.

---

## Functions

Functions are reusable blocks of code that take inputs, do work, and optionally return a result.

### Anatomy of a Function

```c
int add(int a, int b) {
//↑     ↑         ↑
//│     │         └── Parameters (inputs)
//│     └── Function name
//└── Return type

    int sum = a + b;   // Body: the work
    return sum;        // Return value (output)
}

// Usage:
int result = add(3, 4);  // result = 7
```

### Declaration vs Definition

**Declaration** (in `.h` file) - "This function exists"
```c
// tictactoe.h
int game_make_move(GameState *state, int position);
```

**Definition** (in `.c` file) - "This is how it works"
```c
// tictactoe.c
int game_make_move(GameState *state, int position) {
    if (state == NULL) return -1;
    // ... implementation ...
    return 0;
}
```

**Why separate?** Other files only need to know *what* functions exist (header), not *how* they work (source).

### Return Type Conventions

**`void`** - No return value
```c
void display_board(const GameState *state) {
    printf("...");
    // No return statement needed
}
```

**`int` for success/failure** - 0 = success, -1 = error
```c
int game_make_move(GameState *state, int position) {
    if (position < 0 || position >= BOARD_SIZE) {
        return -1;  // Error: invalid position
    }
    // ... do the move ...
    return 0;  // Success
}

// Usage:
if (game_make_move(&game, pos) == -1) {
    printf("Invalid move!\n");
}
```

**`int` for boolean** - 0 = false, non-zero = true
```c
int game_is_over(GameState *state) {
    return state->status != ONGOING;  // Returns 1 or 0
}

// Usage:
if (game_is_over(&game)) {
    printf("Game ended!\n");
}
```

**Return the actual value** - For queries
```c
CellValue game_get_current_player(GameState *state) {
    return (state->move_count % 2 == 0) ? CELL_X : CELL_O;
}
```

### Parameters

**Value parameters** - Copy of the value (original unchanged)
```c
void print_number(int n) {
    n = n + 1;  // Only modifies the copy
    printf("%d\n", n);
}
```

**Pointer parameters** - Address of the value (can modify original)
```c
void increment(int *n) {
    *n = *n + 1;  // Modifies the original variable
}
```

**`const` parameters** - Promise not to modify
```c
void display_board(const GameState *state) {
    // state->status = X_WINS;  // ❌ Compiler error!
    printf("%d", state->status);  // ✅ Reading is OK
}
```

### Static Functions (Private to File)

```c
// Only visible within this .c file
static void update_status(GameState *state) {
    // Helper function - not part of public API
}

// Visible to other files (via header)
int game_make_move(GameState *state, int position) {
    // ... 
    update_status(state);  // Can call static function
    return 0;
}
```

### Zero-Parameter Functions

**⚠️ C Gotcha:** `()` and `(void)` mean different things!

```c
int get_value();      // ❌ "Unknown number of parameters" (deprecated)
int get_value(void);  // ✅ "Exactly zero parameters"
```

Always use `(void)` for functions with no parameters.

See [include/tictactoe.h](include/tictactoe.h) for function declarations.

---

## Header Files

C code is split into two types of files:
- **Header files (`.h`)** - Declarations ("what exists")
- **Source files (`.c`)** - Definitions ("how it works")

### Why Two File Types?

Imagine you want to use `game_make_move()` from another file. You need to know:
- What parameters does it take?
- What does it return?

You **don't** need to know how it's implemented. That's what headers provide.

### Header File Example (`.h`)

```c
// tictactoe.h - Public interface

#ifndef TICTACTOE_H      // Include guard (prevents double-inclusion)
#define TICTACTOE_H

#include "constants.h"   // Types we depend on

// Type definitions
typedef struct {
    CellValue board[9];
    GameStatus status;
    // ...
} GameState;

// Function declarations (no bodies!)
void game_init_session(GameState *state);
int game_make_move(GameState *state, int position);
int game_is_over(GameState *state);

#endif // TICTACTOE_H
```

### Source File Example (`.c`)

```c
// tictactoe.c - Implementation

#include "tictactoe.h"    // Our own header first
#include "board.h"        // Other project headers
#include "ai.h"

#include <stdlib.h>       // Standard library headers
#include <time.h>

// Function definitions (with bodies!)
void game_init_session(GameState *state) {
    if (state == NULL) return;
    board_init(state->board);
    state->status = ONGOING;
    // ...
}

int game_make_move(GameState *state, int position) {
    // Full implementation here
}
```

### Include Guards

```c
#ifndef TICTACTOE_H   // "If TICTACTOE_H is NOT defined..."
#define TICTACTOE_H   // "...define it now"

// Header content here

#endif                // "End of the #ifndef block"
```

**Purpose:** Prevent the same header from being included twice in one compilation, which would cause "redefinition" errors.

**Common mistake:** Copy-pasting a header and forgetting to change the guard name:
```c
// board.h - WRONG!
#ifndef TICTACTOE_H   // ❌ Should be BOARD_H
#define TICTACTOE_H
```

### Include Order Convention

```c
// In tictactoe.c:
#include "tictactoe.h"        // 1. Own header first
#include "board.h"            // 2. Other project headers
#include "win_condition_calculator.h"

#include <assert.h>           // 3. Standard library headers
#include <stdlib.h>
#include <time.h>
```

**Why own header first?** Catches missing includes in the header itself.

### What Goes Where?

| In Header (`.h`) | In Source (`.c`) |
|------------------|------------------|
| Type definitions (`typedef`, `struct`) | Function implementations |
| Function declarations | `static` helper functions |
| `#define` constants | Local variables |
| `extern` variable declarations | `#include` for implementation needs |

See [include/](include/) for all header files.

---

## Function Pointers and Abstraction
Function pointers allow you to store and call functions dynamically. This enables powerful patterns like callbacks, plugin systems, and **interface abstraction**.

### Basic Function Pointer Syntax
```c
// A regular function
int add(int a, int b) {
    return a + b;
}

// A function pointer variable
int (*operation)(int, int);  // Points to a function taking 2 ints, returning int

// Assign and call
operation = add;             // Point to the add function
int result = operation(3, 4); // Call through pointer: result = 7
```

### Why Function Pointers Matter

They let you **swap behavior without changing code**. This is crucial for:
- Swapping CLI for GUI
- Testing with mock functions
- Plugin architectures
- Strategy patterns

### The UserInterface Pattern in This Project

We use function pointers to abstract the UI layer:

**Step 1: Define the interface (what functions must exist)**
```c
// ui_interface.h
typedef struct UserInterface {
    void (*display_board)(const GameState *state);
    void (*display_status)(GameStatus status);
    void (*display_message)(const char *message);
    int  (*get_move)(GameState *game);
    int  (*get_yes_no)(const char *prompt);
    CellValue (*get_symbol_choice)(const char *prompt);
    Difficulty (*get_difficulty)(void);
} UserInterface;

// Factory function declaration
UserInterface *ui_get_cli_interface(void);
```

**Step 2: Implement the interface (CLI version)**
```c
// ui_cli.c
static void cli_display_board(const GameState *state) {
    // printf the board to terminal...
}

static int cli_get_move(GameState *game) {
    // scanf from user...
}

// Create the interface struct with function pointers
static UserInterface cli_interface = {
    .display_board = cli_display_board,
    .display_status = cli_display_status,
    .display_message = cli_display_message,
    .get_move = cli_get_move,
    .get_yes_no = cli_get_yes_no,
    .get_symbol_choice = cli_get_symbol_choice,
    .get_difficulty = cli_get_difficulty,
};

UserInterface *ui_get_cli_interface(void) {
    return &cli_interface;
}
```

**Step 3: Use the interface (game logic doesn't know about CLI)**
```c
// main.c
int main(void) {
    GameState game;
    UserInterface *ui = ui_get_cli_interface();  // Could be GUI!
    
    ui->display_message("Welcome to Tic-Tac-Toe!");
    
    while (!game_is_over(&game)) {
        ui->display_board(&game);
        int move = ui->get_move(&game);  // Works for CLI or GUI
        game_make_move(&game, move);
    }
    
    ui->display_status(game.status);
    return 0;
}
```

### Adding a GUI Implementation
To add a GUI, you just implement the same interface:

```c
// ui_gui.c (example with SDL2)
static void gui_display_board(const GameState *state) {
    // SDL_RenderDrawLine, SDL_RenderCopy, etc.
}

static int gui_get_move(GameState *game) {
    // SDL_PollEvent for mouse clicks
}

static UserInterface gui_interface = {
    .display_board = gui_display_board,
    .get_move = gui_get_move,
    // ... same fields, different implementations
};

UserInterface *ui_get_gui_interface(void) {
    return &gui_interface;
}
```

**To switch from CLI to GUI:** Change ONE line in main.c:
```c
// UserInterface *ui = ui_get_cli_interface();
UserInterface *ui = ui_get_gui_interface();  // That's it!
```

### Benefits of This Pattern

| Without Abstraction              | With UserInterface                      |
|----------------------------------|-----------------------------------------|
| `printf` scattered throughout    | All I/O in one place                    |
| Changing UI = rewrite game logic | Changing UI = new implementation file   |
| Can't test without terminal      | Can mock the interface for tests        |
| Hard to add web/mobile UI        | Each platform implements same interface |

### Algorithm Deep Dive: `cli_display_board`

For a detailed breakdown of how `cli_display_board` renders the game board dynamically for any board size, see:

📄 **[CLI Display Board Algorithm](algorithms/CLI_DISPLAY_BOARD.md)**

Topics covered:
- Visual output examples (3x3 and 4x4 boards)
- Step-by-step code breakdown
- 2D to 1D index conversion formula
- Key design decisions and rationale
- Complexity analysis
- Extension ideas (colors, Unicode, labels)

See [include/ui_interface.h](include/ui_interface.h) and [src/ui_cli.c](src/ui_cli.c).

---

## Memory Management

Memory management is one of the most critical topics in C programming. Unlike languages with garbage collection (Java, Python, Go), C requires you to **manually manage memory**. This gives you power and performance, but also responsibility.

### The Memory Layout of a C Program

When your program runs, memory is divided into distinct regions:

```
┌─────────────────────────────────┐  High addresses
│           Stack                 │  ← Local variables, function calls
│             ↓                   │    (grows downward)
│                                 │
│          (free)                 │
│                                 │
│             ↑                   │
│           Heap                  │  ← malloc/calloc/realloc
├─────────────────────────────────┤
│     Uninitialized Data (BSS)    │  ← Global/static vars (zeroed)
├─────────────────────────────────┤
│     Initialized Data            │  ← Global/static vars with values
├─────────────────────────────────┤
│           Text                  │  ← Your compiled code
└─────────────────────────────────┘  Low addresses
```

### Stack Memory (Automatic Storage)

Variables declared inside functions live on the **stack**. The stack is managed automatically—memory is allocated when you enter a function and freed when you leave.

```c
void play_game(void) {
    GameState game;        // Allocated on stack when function enters
    int scores[100];       // Also on stack (400 bytes)
    
    game_init_session(&game);
    // ... use game and scores ...
    
}   // Both 'game' and 'scores' are automatically destroyed here
```

**How the Stack Works:**

```
Before play_game():        During play_game():         After play_game():
┌─────────────┐            ┌─────────────┐            ┌─────────────┐
│   main()    │            │   main()    │            │   main()    │
│   frame     │            │   frame     │            │   frame     │
├─────────────┤            ├─────────────┤            └─────────────┘
│             │            │ play_game() │                  ↑
│             │            │   frame     │            Stack pointer
│             │            │ ┌─────────┐ │            returned here
│             │            │ │  game   │ │
│             │            │ │scores[100]│
│             │            │ └─────────┘ │
└─────────────┘            └─────────────┘
```

**Characteristics of Stack Memory:**

| Aspect | Description |
|--------|-------------|
| **Speed** | Very fast (just move stack pointer) |
| **Allocation** | Automatic when function enters |
| **Deallocation** | Automatic when function returns |
| **Size limit** | Typically 1-8 MB (OS-dependent) |
| **Lifetime** | Limited to function scope |
| **Thread safety** | Each thread has its own stack |

**Stack Overflow:**

The stack has a fixed size. If you allocate too much, you get a **stack overflow**:

```c
void cause_overflow(void) {
    int huge_array[10000000];  // 40 MB! Stack overflow!
}

// Or through infinite recursion:
void infinite(void) {
    infinite();  // Each call adds a stack frame → overflow
}
```

### Heap Memory (Dynamic Storage)

For larger or longer-lived data, use the **heap** via `malloc()`, `calloc()`, `realloc()`, and `free()`:

```c
#include <stdlib.h>

// Allocate memory for one GameState
GameState *game = malloc(sizeof(GameState));

// Always check if allocation succeeded
if (game == NULL) {
    fprintf(stderr, "Error: Out of memory!\n");
    exit(1);
}

// Use the memory
game_init_session(game);

// When done, free the memory
free(game);
game = NULL;  // Prevent dangling pointer
```

**Characteristics of Heap Memory:**

| Aspect | Description |
|--------|-------------|
| **Speed** | Slower than stack (bookkeeping overhead) |
| **Allocation** | Manual via malloc/calloc/realloc |
| **Deallocation** | Manual via free |
| **Size limit** | Limited by available RAM (gigabytes) |
| **Lifetime** | Until explicitly freed |
| **Thread safety** | Shared between threads (needs synchronization) |

### The malloc Family

**`malloc(size)`** - Allocate uninitialized memory:
```c
// Allocate 100 integers (uninitialized - contains garbage!)
int *arr = malloc(100 * sizeof(int));
```

**`calloc(count, size)`** - Allocate and zero-initialize:
```c
// Allocate 100 integers (all initialized to 0)
int *arr = calloc(100, sizeof(int));
```

**`realloc(ptr, new_size)`** - Resize existing allocation:
```c
// Grow array from 100 to 200 integers
arr = realloc(arr, 200 * sizeof(int));
// Note: realloc may move the data to a new location!
```

**`free(ptr)`** - Release memory back to the system:
```c
free(arr);
arr = NULL;  // Good practice
```

### Allocation Patterns

**Pattern 1: Single Object**
```c
GameState *game = malloc(sizeof(GameState));
if (game == NULL) { /* handle error */ }
// ... use game->board, game->status, etc.
free(game);
```

**Pattern 2: Array of Objects**
```c
// Allocate array of 10 games
GameState *games = malloc(10 * sizeof(GameState));
if (games == NULL) { /* handle error */ }

// Access as array
games[0].status = ONGOING;
games[5].move_count = 3;

free(games);  // Frees entire array
```

**Pattern 3: Array of Pointers**
```c
// Allocate array of 10 pointers
GameState **games = malloc(10 * sizeof(GameState *));
if (games == NULL) { /* handle error */ }

// Allocate each game individually
for (int i = 0; i < 10; i++) {
    games[i] = malloc(sizeof(GameState));
    if (games[i] == NULL) { /* handle error */ }
}

// Free in reverse order
for (int i = 0; i < 10; i++) {
    free(games[i]);
}
free(games);
```

**Pattern 4: 2D Array (Contiguous)**
```c
// Allocate 3x3 board as contiguous memory
int *board = malloc(3 * 3 * sizeof(int));
// Access: board[row * 3 + col]
board[1 * 3 + 2] = 5;  // Row 1, Col 2
free(board);
```

**Pattern 5: 2D Array (Array of Rows)**
```c
// Allocate array of row pointers
int **board = malloc(3 * sizeof(int *));
for (int i = 0; i < 3; i++) {
    board[i] = malloc(3 * sizeof(int));
}
// Access: board[row][col]
board[1][2] = 5;

// Free each row, then the array
for (int i = 0; i < 3; i++) {
    free(board[i]);
}
free(board);
```

### Memory Errors: The Deadly Sins

#### 1. Uninitialized Pointer
```c
int *ptr;           // ❌ Contains garbage address!
*ptr = 5;           // ❌ Writing to random memory → crash or corruption

// ✅ Fix: Initialize to NULL or valid address
int *ptr = NULL;
// or
int value = 42;
int *ptr = &value;
```

#### 2. Null Pointer Dereference
```c
int *ptr = malloc(sizeof(int));
// What if malloc fails?
*ptr = 5;           // ❌ If ptr is NULL, this crashes!

// ✅ Fix: Always check malloc result
int *ptr = malloc(sizeof(int));
if (ptr == NULL) {
    fprintf(stderr, "Out of memory\n");
    exit(1);
}
*ptr = 5;           // ✅ Safe
```

#### 3. Memory Leak
```c
void create_leak(void) {
    int *ptr = malloc(sizeof(int));
    *ptr = 42;
    return;         // ❌ ptr goes out of scope, memory never freed!
}

// Call this 1000 times → 4000 bytes leaked

// ✅ Fix: Free before returning
void no_leak(void) {
    int *ptr = malloc(sizeof(int));
    *ptr = 42;
    // ... use ptr ...
    free(ptr);      // ✅ Clean up
    return;
}
```

#### 4. Use After Free (Dangling Pointer)
```c
int *ptr = malloc(sizeof(int));
*ptr = 42;
free(ptr);
// ptr still contains the old address!
printf("%d\n", *ptr);  // ❌ Undefined behavior! May print 42, 0, or crash

// ✅ Fix: Set to NULL after free
free(ptr);
ptr = NULL;
// Now any accidental use will reliably crash (easier to debug)
```

#### 5. Double Free
```c
int *ptr = malloc(sizeof(int));
free(ptr);
free(ptr);          // ❌ Crash! Memory already freed

// ✅ Fix: Set to NULL after free
free(ptr);
ptr = NULL;
free(ptr);          // ✅ free(NULL) is safe (does nothing)
```

#### 6. Buffer Overflow (Heap)
```c
int *arr = malloc(5 * sizeof(int));  // Space for 5 ints
arr[10] = 42;       // ❌ Writing beyond allocated memory!
                    // Corrupts heap metadata → crash later

// ✅ Fix: Track array size, check bounds
size_t arr_size = 5;
if (index < arr_size) {
    arr[index] = 42;  // ✅ Safe
}
```

#### 7. Returning Stack Address
```c
int *get_value(void) {
    int x = 42;      // x lives on stack
    return &x;       // ❌ Returning address of local variable!
}   // x is destroyed here

int *ptr = get_value();
printf("%d\n", *ptr);  // ❌ x no longer exists!

// ✅ Fix: Allocate on heap
int *get_value(void) {
    int *x = malloc(sizeof(int));
    *x = 42;
    return x;        // ✅ Heap memory persists
}
// Caller must free!
```

#### 8. Mismatched Allocation/Deallocation
```c
// ❌ Wrong: mixing C++ new with C free
int *ptr = new int;  // C++ allocation
free(ptr);           // C deallocation → undefined!

// ❌ Wrong: freeing stack memory
int x = 42;
free(&x);            // Crash! &x is not from malloc

// ✅ Always match:
// malloc/calloc/realloc → free
// new → delete (C++)
// new[] → delete[] (C++)
```

### The sizeof Operator

`sizeof` returns the size in bytes. Use it to make allocations portable:

```c
// ❌ Fragile: assumes int is 4 bytes
int *arr = malloc(40);  // 10 ints... maybe?

// ✅ Robust: works regardless of type size
int *arr = malloc(10 * sizeof(int));

// ✅ Even better: size from variable (type changes automatically)
int *arr = malloc(10 * sizeof(*arr));
```

**sizeof with different types:**
```c
printf("char:   %zu bytes\n", sizeof(char));    // 1
printf("int:    %zu bytes\n", sizeof(int));     // 4 (usually)
printf("long:   %zu bytes\n", sizeof(long));    // 4 or 8
printf("float:  %zu bytes\n", sizeof(float));   // 4
printf("double: %zu bytes\n", sizeof(double));  // 8
printf("pointer:%zu bytes\n", sizeof(int*));    // 4 or 8

// Struct size includes padding!
typedef struct {
    char c;    // 1 byte
    int i;     // 4 bytes
} Example;
printf("Example: %zu bytes\n", sizeof(Example));  // Often 8, not 5!
```

### Memory in This Project

This project uses **stack allocation only** for simplicity:

```c
// main.c
int main(void) {
    GameState game;           // ~50 bytes on stack
    UserInterface *ui = ui_get_cli_interface();
    
    game_init_session(&game);
    // ... play games ...
    
    return 0;  // 'game' automatically cleaned up
}
```

**Why stack is sufficient here:**
- `GameState` is small (~50 bytes)
- Only one game instance needed
- Lifetime matches the program
- No need for dynamic sizing

**When we would use heap:**
- Storing multiple game histories
- Dynamically sized board (runtime-configurable)
- Network multiplayer (games outlive connections)
- Undo/redo stack of arbitrary depth

### Best Practices Summary

```c
// 1. Always initialize pointers
int *ptr = NULL;

// 2. Always check malloc result
ptr = malloc(sizeof(int));
if (ptr == NULL) {
    perror("malloc failed");
    exit(EXIT_FAILURE);
}

// 3. Use sizeof with the variable, not the type
Thing *things = malloc(n * sizeof(*things));

// 4. Free in reverse order of allocation
free(inner);
free(outer);

// 5. Set pointers to NULL after freeing
free(ptr);
ptr = NULL;

// 6. Use calloc for zero-initialized memory
int *zeroes = calloc(100, sizeof(int));

// 7. Check realloc carefully (may return NULL)
int *new_ptr = realloc(ptr, new_size);
if (new_ptr == NULL) {
    // ptr is still valid! Don't lose it.
    free(ptr);
    exit(EXIT_FAILURE);
}
ptr = new_ptr;

// 8. Document ownership in comments
// Caller is responsible for freeing returned pointer
GameState *create_game(void);
```

### Memory Debugging Tools

**Valgrind (Linux/macOS):**
```bash
# Compile with debug symbols
gcc -g -O0 program.c -o program

# Run with memory checking
valgrind --leak-check=full --show-leak-kinds=all ./program
```

**Valgrind output example:**
```
==12345== HEAP SUMMARY:
==12345==   in use at exit: 72 bytes in 3 blocks
==12345==   total heap usage: 10 allocs, 7 frees, 1,024 bytes allocated
==12345== 
==12345== 24 bytes in 1 blocks are definitely lost in record 1 of 3
==12345==    at 0x4C2FB0F: malloc (in /usr/lib/valgrind/...)
==12345==    by 0x108721: create_game (game.c:42)
==12345==    by 0x108800: main (main.c:15)
```

**AddressSanitizer (Clang/GCC):**
```bash
# Compile with sanitizer
gcc -fsanitize=address -g program.c -o program

# Run normally - crashes with detailed report on memory errors
./program
```

**Static Analysis:**
```bash
# Clang static analyzer
scan-build gcc -c *.c

# Cppcheck
cppcheck --enable=all src/
```

### Quick Reference Table

| Operation | Function | Notes |
|-----------|----------|-------|
| Allocate | `malloc(size)` | Uninitialized memory |
| Allocate zeroed | `calloc(count, size)` | All bytes set to 0 |
| Resize | `realloc(ptr, new_size)` | May move data |
| Free | `free(ptr)` | Sets memory as available |
| Get size | `sizeof(type)` | Compile-time constant |
| Copy memory | `memcpy(dst, src, n)` | Non-overlapping only |
| Move memory | `memmove(dst, src, n)` | Handles overlap |
| Fill memory | `memset(ptr, value, n)` | Sets bytes to value |
| Compare | `memcmp(a, b, n)` | Returns 0 if equal |

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

### Smell 2: God Function
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

### Smell 3: Leaking Abstraction
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

### Smell 4: Duplicated Logic
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

### Smell 5: Silent Failure
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

## C Pitfalls and How to Avoid Them
### Pitfall 1: Missing Include for NULL
**❌ Error:**
```c
// ai.c
if (state == NULL) { ... }
// Compiler: error: use of undeclared identifier 'NULL'
```

**✅ Fix:**
```c
#include <stdlib.h>  // Defines NULL

if (state == NULL) { ... }
```

**Why:** `NULL` is not a keyword in C - it's defined in several headers (`<stdlib.h>`, `<stdio.h>`, `<string.h>`, etc.). Always include the appropriate header.

### Pitfall 2: Empty Parameter List `()`
**❌ Wrong:**
```c
int newline();  // Means "unknown number of arguments" in C!
```

**✅ Correct:**
```c
int newline(void);  // Means "exactly zero arguments"
```

**Why:** In C (unlike C++), `()` declares a function with an **unspecified** number of parameters. This is deprecated and generates warnings under `-Wstrict-prototypes`. Always use `(void)` for no-parameter functions.

**Real project error:**
```
warning: a function declaration without a prototype is deprecated in all versions of C [-Wstrict-prototypes]
    int newline();
                ^
                 void
```

### Pitfall 3: Missing Newline at End of File
**❌ Error:**
```c
// ai.c - last line
}
// ^ No newline after closing brace
```

**Compiler warning:**
```
warning: no newline at end of file [-Wnewline-eof]
```

**✅ Fix:** Ensure every source file ends with a newline character.

**Why:** The C standard requires all source files to end with a newline. Some compilers/tools break without it.

### Pitfall 4: Array Out of Bounds
**❌ Dangerous:**
```c
CellValue board[9];
board[9] = CELL_X;  // OUT OF BOUNDS! Undefined behavior
```

**Why it's bad:**
- No runtime error
- Corrupts adjacent memory
- Hard to debug crashes later

**✅ Prevention:**
```c
int board_place(CellValue board[BOARD_SIZE], int position, CellValue symbol) {
    if (position < 0 || position >= BOARD_SIZE) {
        return -1;  // Bounds check BEFORE access
    }
    board[position] = symbol;
    return 0;
}
```

### Pitfall 5: Off-by-One Errors
**❌ Common mistake:**
```c
for (int i = 0; i <= 9; i++) {  // Should be i < 9
    board[i] = CELL_EMPTY;      // board[9] is out of bounds!
}
```

**✅ Correct:**
```c
for (int i = 0; i < BOARD_SIZE; i++) {
    board[i] = CELL_EMPTY;
}
```

**Remember:** Arrays are **0-indexed**. An array of size 9 has indices 0-8.

### Pitfall 6: Pointer Dereferencing NULL
**❌ Crash:**
```c
void game_init(GameState *state) {
    state->status = ONGOING;  // CRASH if state is NULL!
}
```

**✅ Defensive:**
```c
void game_init(GameState *state) {
    if (state == NULL) return;  // Guard
    state->status = ONGOING;     // Now safe
}
```

### Pitfall 7: Uninitialized Variables
**❌ Undefined Behavior:**
```c
int best_move;  // Garbage value!
// ... maybe assign, maybe not ...
return best_move;  // Could be anything!
```

**✅ Always Initialize:**
```c
int best_move = -1;  // Sentinel value
```

### Pitfall 8: Silent Integer Promotion
**❌ Subtle bug:**
```c
char a = 200;
char b = 100;
char result = a + b;  // Overflow! Result is implementation-defined
```

**Why:** Arithmetic operations promote small types to `int`. The sum (300) overflows when cast back to `char`.

**✅ Awareness:**
```c
// Use appropriate types for the value range
int result = a + b;  // No overflow if result fits in int
```

### Pitfall 9: `=` vs `==`
**❌ Assignment instead of comparison:**
```c
if (state->status = ONGOING) {  // ALWAYS true! Assigns ONGOING to status
    // ...
}
```

**✅ Comparison:**
```c
if (state->status == ONGOING) {  // Correct comparison
    // ...
}
```

**Prevention:** Enable warnings: `-Wall -Wextra`

### Pitfall 10: String Literals Are Immutable
**❌ Crash:**
```c
char *str = "Hello";
str[0] = 'h';  // SEGFAULT! String literals are in read-only memory
```

**✅ Mutable String:**
```c
char str[] = "Hello";  // Array copy - mutable
str[0] = 'h';          // OK
```

### Pitfall 11: `scanf` Buffer Overflow
**❌ Dangerous:**
```c
char name[10];
scanf("%s", name);  // User can type 100 characters - BUFFER OVERFLOW!
```

**✅ Safe:**
```c
char name[10];
scanf("%9s", name);  // Max 9 chars + null terminator
```

**Better:** Use `fgets()` for string input.

### Pitfall 12: Forgetting `break` in `switch`
**❌ Fall-through:**
```c
switch (cell) {
    case CELL_X:
        printf("X");
        // FALLS THROUGH to next case!
    case CELL_O:
        printf("O");
        break;
}
```

**✅ Explicit:**
```c
switch (cell) {
    case CELL_X:
        printf("X");
        break;  // Explicit break
    case CELL_O:
        printf("O");
        break;
}
```

### Pitfall 13: Ignoring Return Values
**❌ Silent Failure:**
```c
scanf("%d", &position);  // Returns number of items read
// What if scanf fails?
```

**✅ Check Returns:**
```c
if (scanf("%d", &position) != 1) {
    clear_input_buffer();
    printf("Invalid input.\n");
    continue;
}
```

### Pitfall 14: `memset` with Non-Zero Values
**❌ Only works for 0 and -1:**
```c
int arr[10];
memset(arr, 5, sizeof(arr));  // Does NOT set each int to 5!
// Sets each BYTE to 5, resulting in 0x05050505 per int
```

**✅ Loop for non-zero:**
```c
for (int i = 0; i < 10; i++) {
    arr[i] = 5;
}
```

**Or use with enums:**
```c
CellValue board[9];
memset(board, CELL_EMPTY, sizeof(board));  // OK if CELL_EMPTY is 0
```

### Pitfall 15: Header Guard Typos
**❌ Copy-paste error:**
```c
// board.h
#ifndef TICTACTOE_H  // Wrong guard!
#define TICTACTOE_H
// ...
#endif
```

**✅ Match filename:**
```c
// board.h
#ifndef BOARD_H
#define BOARD_H
// ...
#endif
```

**Why:** Duplicate guards mean the header can be included even when it shouldn't be, or excluded when it should be.

---

## Compiler Flags for Catching Errors
**Always compile with warnings enabled:**

```bash
gcc -std=c99 -Wall -Wextra -pedantic -Werror -O2 \
    src/*.c -Iinclude -o tictactoe
```

- `-Wall` - Enable all common warnings
- `-Wextra` - Enable extra warnings
- `-pedantic` - Reject non-standard features
- `-Werror` - Treat warnings as errors (forces you to fix them)
- `-O2` - Optimization (can reveal uninitialized variable bugs)

**This project compiles cleanly with these strict flags!**

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
gcc -g -O0 src/*.c -Iinclude -o tictactoe
gdb ./tictactoe
(gdb) break main
(gdb) run
(gdb) print variable_name
(gdb) next  # Step over
(gdb) step  # Step into
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
clang --analyze src/*.c -Iinclude
```

### 5. Valgrind (Memory Errors)
```bash
valgrind --leak-check=full ./tictactoe
```

---

## Best Practices Summary
✅ **DO:**
- Check all pointers for NULL before dereferencing
- Validate all array indices before access
- Check return values from functions
- Use `const` for read-only parameters
- Use named constants instead of magic numbers
- Write tests for every function
- Enable all compiler warnings
- Initialize all variables
- Use `(void)` for zero-parameter functions

❌ **DON'T:**
- Access arrays out of bounds
- Use uninitialized variables
- Ignore compiler warnings
- Mix `=` and `==`
- Forget `break` in `switch` statements
- Use `()` instead of `(void)` in function declarations
- Forget to free dynamically allocated memory (if you use `malloc`)
- Trust user input without validation

---

## Next Steps
1. **Explore the codebase:**
   - Read `src/board.c` - Pure data structure operations
   - Read `src/win_condition_calculator.c` - Algorithm with testable helpers
   - Read `src/tictactoe.c` - Module coordination
   - Read `include/ui_interface.h` - Function pointer abstraction
   - Read `src/ui_cli.c` - CLI implementation of the interface
   - Read `tests/` - See how functions are tested

2. **Modify the game:**
   - Change board size to 4x4 (change `BOARD_DIM` in `constants.h`)
   - Implement a GUI using `ui_gui_example.c` as a template
   - Add a third player (extend `CellValue` enum)
   - Implement a medium difficulty AI
   - Add undo/redo functionality

3. **Improve the code:**
   - Add more edge case tests
   - Add logging to track game events
   - Implement save/load game state
   - Create a web interface using the `UserInterface` pattern

4. **Build and test:**
   ```bash
   # Build the game
   ceedling release
   
   # Run the game
   ./build/release/tictactoe.exe
   
   # Run tests (106 tests)
   ceedling test:all
   ```

---

## Project Structure Overview
```
the-dreaded-c/
├── include/           # Header files (.h)
│   ├── ai.h
│   ├── ai_easy.h
│   ├── board.h
│   ├── constants.h        # BOARD_DIM, BOARD_SIZE, WIN_LENGTH, enums
│   ├── tictactoe.h
│   ├── ui_interface.h     # UserInterface abstraction (function pointers)
│   ├── utils.h
│   └── win_condition_calculator.h
├── src/               # Implementation files (.c)
│   ├── ai.c               # Hard AI (minimax algorithm)
│   ├── ai_easy.c          # Easy AI (random moves)
│   ├── board.c            # Board operations
│   ├── constants.c        # Global constants
│   ├── main.c             # Program entry point
│   ├── tictactoe.c        # Game coordinator
│   ├── ui_cli.c           # CLI implementation of UserInterface
│   ├── ui_gui_example.c   # Example GUI implementation (SDL2)
│   ├── utils.c            # Helper utilities
│   └── win_condition_calculator.c  # Win/draw detection (testable helpers)
├── tests/             # Unit tests
│   ├── test_board.c                    
│   ├── test_tictactoe.c                
│   └── test_win_condition_calculator.c
└── docs/
    ├── LEARNING_GUIDE.md  # This file
    └── REQUIREMENTS.md
```

---

## Real Bugs We Fixed in This Project
### Bug 1: Implicit Function Definition
**Symptom:** `warning: implicit declaration of function 'NULL'`  
**Cause:** Missing `#include <stdlib.h>` in `ai.c`  
**Fix:** Added the include to define `NULL`

### Bug 2: Empty Parameter List
**Symptom:** `warning: a function declaration without a prototype is deprecated`  
**Cause:** `int newline()` instead of `int newline(void)`  
**Fix:** Changed to `int newline(void)` in both header and source

### Bug 3: Missing Newline at EOF
**Symptom:** `warning: no newline at end of file`  
**Cause:** Files ended without a final newline character  
**Fix:** Added newline to end of all source files

### Bug 4: API Leakage
**Symptom:** `main.c` directly manipulating `game.game_count`  
**Cause:** Missing API function for winner symbol choice  
**Fix:** Added `game_set_winner_symbol_choice()` function

### Bug 5: Silent Fallback
**Symptom:** AI returning center position when passed NULL  
**Cause:** Defensive programming taken too far - hiding bugs  
**Fix:** Replaced `if (state == NULL) return 4;` with `assert(state != NULL);`

**Lesson:** Each bug taught us something about C best practices!

---

## Additional Resources
- **Official C Standard**: ISO/IEC 9899:2018 (complex, formal)
- **Beej's Guide to C**: https://beej.us/guide/bgc/ (beginner-friendly)
- **K&R C**: "The C Programming Language" (classic textbook)
- **C Reference**: https://en.cppreference.com/w/c (quick lookup)

Happy learning! 🚀
