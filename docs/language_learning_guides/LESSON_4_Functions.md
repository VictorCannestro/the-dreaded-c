# Lesson 4: Functions

Functions are reusable blocks of code that take inputs, do work, and optionally return a result.

## Anatomy of a Function

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

## Declaration vs Definition

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

## Return Type Conventions

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

## Parameters

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

## Static Functions (Private to File)

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

## Zero-Parameter Functions

**⚠️ C Gotcha:** `()` and `(void)` mean different things!

```c
int get_value();      // ❌ "Unknown number of parameters" (deprecated)
int get_value(void);  // ✅ "Exactly zero parameters"
```

Always use `(void)` for functions with no parameters.

See [include/tictactoe.h](../../include/tictactoe.h) for function declarations.

---

## Next Steps

Continue to [Lesson 5: Arrays](LESSON_5_Arrays.md) →

← Previous: [Lesson 3: Pointers](LESSON_3_Pointers.md)

← Back to [Learning Guide Index](LEARNING_GUIDE.md)

