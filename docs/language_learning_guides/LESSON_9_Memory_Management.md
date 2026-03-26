# Lesson 9: Memory Management
Memory management is one of the most critical topics in C programming. Unlike languages with garbage collection 
(Java, Python, Go), C requires you to **manually manage memory**. This gives you power and performance, but also 
responsibility.

## The Memory Layout of a C Program
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

## Stack Memory (Automatic Storage)
Variables declared inside functions live on the **stack**. The stack is managed automatically — memory is allocated 
when you enter a function and freed when you leave.

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
│   frame     │            │   frame     │            └─────────────┘
├─────────────┤            ├─────────────┤
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

## Heap Memory (Dynamic Storage)
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

| Aspect            | Description                                    |
|-------------------|------------------------------------------------|
| **Speed**         | Slower than stack (bookkeeping overhead)       |
| **Allocation**    | Manual via malloc/calloc/realloc               |
| **Deallocation**  | Manual via free                                |
| **Size limit**    | Limited by available RAM (gigabytes)           |
| **Lifetime**      | Until explicitly freed                         |
| **Thread safety** | Shared between threads (needs synchronization) |

## The malloc Family
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

## Allocation Patterns

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
board[1][2] = 5;  // Row 1, Col 2

// Free each row, then the array
for (int i = 0; i < 3; i++) {
    free(board[i]);
}
free(board);
```

## Memory Errors: The Deadly Sins
### 1. Uninitialized Pointer
```c
int *ptr;           // ❌ Contains garbage address!
*ptr = 5;           // ❌ Writing to random memory → crash or corruption

// ✅ Fix: Initialize to NULL or valid address
int *ptr = NULL;
// or
int value = 42;
int *ptr = &value;
```

### 2. Null Pointer Dereference
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

### 3. Memory Leak
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

### 4. Use After Free (Dangling Pointer)
```c
int *ptr = malloc(sizeof(int));
*ptr = 42;
free(ptr);
// ptr still contains the old address!
printf("%d\n", *ptr);  // ❌ x no longer exists!

// ✅ Fix: Set to NULL after free
free(ptr);
ptr = NULL;
// Now any accidental use will reliably crash (easier to debug)
```

### 5. Double Free
```c
int *ptr = malloc(sizeof(int));
free(ptr);
free(ptr);          // ❌ Crash! Memory already freed

// ✅ Fix: Set to NULL after free
free(ptr);
ptr = NULL;
free(ptr);          // ✅ free(NULL) is safe (does nothing)
```

### 6. Buffer Overflow (Heap)
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

### 7. Returning Stack Address
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

### 8. Mismatched Allocation/Deallocation
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

## The sizeof Operator

`sizeof` returns the size in bytes. Use it to make allocations portable:

```c
// ❌ Fragile: assumes int is 4 bytes
int *arr = malloc(40);  // 10 ints... maybe?

// ✅ Robust: works regardless of type size
int *arr = malloc(10 * sizeof(int));

// ✅ Even better: size from variable (type changes automatically)
int *arr = malloc(10 * sizeof(*arr));
```

## Memory in This Project
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

## Best Practices Summary
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

## Next Steps

Continue to [Lesson 10: The Challenge 🐍](LESSON_10_The_Challenge.md) →

← Previous: [Lesson 8: Function Pointers and Abstraction](LESSON_8_Function_Pointers_And_Abstraction.md)

← Back to [Learning Guide Index](LEARNING_GUIDE.md)

