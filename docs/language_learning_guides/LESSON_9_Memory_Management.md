# Lesson 9: Memory Management

> **Learning Objectives**: By the end of this lesson, you will understand how C programs use memory, the difference
> between stack and heap allocation, how to use `malloc`/`free` safely, and how to avoid common memory errors that cause crashes and security vulnerabilities.

Memory management is one of the most critical topics in C programming. Unlike languages with garbage collection (Java, 
Python, Go), C requires you to **manually manage memory**. This gives you power and performance, but also responsibility.

Think of it this way:
- **Garbage-collected languages**: You check into a hotel. Housekeeping cleans your room automatically.
- **C**: You're camping. You set up your tent, and you *must* take it down. Leave trash behind? It stays there forever.

This lesson will teach you to be a responsible camper! 🏕️

---

## Table of Contents
### Core Concepts
1. [The Memory Layout of a C Program](#the-memory-layout-of-a-c-program)
2. [Stack Memory (Automatic Storage)](#stack-memory-automatic-storage)
3. [Heap Memory (Dynamic Storage)](#heap-memory-dynamic-storage)
4. [The malloc Family](#the-malloc-family)
5. [The sizeof Operator](#the-sizeof-operator)

### Patterns & Pitfalls
6. [Basic Allocation Patterns](#basic-allocation-patterns)
7. [Memory Errors: The Deadly Sins](#memory-errors-the-deadly-sins)

### Practical Application
8. [Memory in This Project](#memory-in-this-project)
9. [Best Practices Summary](#best-practices-summary)
10. [Debugging Memory Issues](#debugging-memory-issues)

### Practice
11. [Self-Assessment Checklist](#self-assessment-checklist)
12. [Practical Exercises](#practical-exercises)

### Going Further
13. [Next Steps](#next-steps)
14. [Bonus: Advanced Topics](#bonus-advanced-topics) *(come back after completing Lesson 10)*

---

## The Memory Layout of a C Program

Before diving into allocation, let's understand *where* memory lives. When your program runs, the operating system gives it a chunk of memory divided into distinct regions:

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

### Understanding Each Region

| Region | What Lives Here | Example | Lifetime |
|--------|-----------------|---------|----------|
| **Text** | Your compiled machine code | The `main()` function instructions | Entire program |
| **Initialized Data** | Global/static variables with initial values | `static int count = 10;` | Entire program |
| **BSS** | Global/static variables without initial values | `static int buffer[100];` | Entire program |
| **Heap** | Dynamically allocated memory | `malloc(1024)` | Until `free()` called |
| **Stack** | Local variables, function parameters, return addresses | `int x = 5;` inside a function | Until function returns |

### A Concrete Example

```c
#include <stdio.h>
#include <stdlib.h>

int global_initialized = 42;        // → Initialized Data segment
int global_uninitialized;           // → BSS segment (auto-zeroed)
static int static_var = 100;        // → Initialized Data segment

void example_function(int param) {  // param → Stack
    int local_var = 10;             // → Stack
    static int static_local = 5;    // → Initialized Data (persists!)
    int *heap_ptr = malloc(100);    // heap_ptr → Stack, but points to → Heap
    
    printf("local_var is at:  %p (stack)\n", (void*)&local_var);
    printf("heap memory is at: %p (heap)\n", (void*)heap_ptr);
    
    free(heap_ptr);
}

int main(void) {
    example_function(7);
    return 0;
}
```

> 💡 **Key Insight**: The *pointer variable* `heap_ptr` lives on the stack (8 bytes for the address), but the *memory it points to* lives on the heap (100 bytes). This distinction is crucial!

---

## Stack Memory (Automatic Storage)

Variables declared inside functions live on the **stack**. The stack is managed automatically — memory is allocated when you enter a function and freed when you leave.

### Why "Stack"?

It's called a stack because it works like a stack of plates: you can only add (push) or remove (pop) from the top. Each function call pushes a new "stack frame" containing its local variables, and returning pops that frame off.

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

| Aspect            | Description                         |
|-------------------|-------------------------------------|
| **Speed**         | Very fast (just move stack pointer) |
| **Allocation**    | Automatic when function enters      |
| **Deallocation**  | Automatic when function returns     |
| **Size limit**    | Typically 1-8 MB (OS-dependent)     |
| **Lifetime**      | Limited to function scope           |
| **Thread safety** | Each thread has its own stack       |

**Stack Overflow:**

The stack has a fixed size (typically 1-8 MB). If you allocate too much, you get a **stack overflow**:

```c
void cause_overflow(void) {
    int huge_array[10000000];  // 40 MB! Stack overflow!
}

// Or through infinite recursion:
void infinite(void) {
    infinite();  // Each call adds a stack frame → overflow
}
```

> ⚠️ **Real-World Impact**: Stack overflow vulnerabilities have been exploited in many security breaches. Buffer overflows on the stack can allow attackers to overwrite return addresses and execute malicious code.

### When to Use Stack Memory

✅ **Use the stack when:**
- Data is small (< 1 KB is safe, < 100 KB is usually fine)
- Lifetime matches function scope
- Size is known at compile time
- You want automatic cleanup

❌ **Avoid the stack when:**
- Data is large (arrays of thousands of elements)
- Data needs to outlive the function
- Size is determined at runtime
- You're building recursive algorithms with deep call stacks

---

## Heap Memory (Dynamic Storage)

For larger or longer-lived data, use the **heap** via `malloc()`, `calloc()`, `realloc()`, and `free()`. The heap is a large pool of memory that you manage manually.

### Your First Heap Allocation

```c
#include <stdlib.h>
#include <stdio.h>

int main(void) {
    // Step 1: Request memory from the heap
    GameState *game = malloc(sizeof(GameState));
    
    // Step 2: ALWAYS check if allocation succeeded
    if (game == NULL) {
        fprintf(stderr, "Error: Out of memory!\n");
        exit(1);
    }
    
    // Step 3: Use the memory
    game_init_session(game);
    // ... play the game ...
    
    // Step 4: When done, free the memory
    free(game);
    
    // Step 5: Prevent accidental reuse (dangling pointer)
    game = NULL;
    
    return 0;
}
```

### The Lifecycle of Heap Memory

```
1. REQUEST    →    2. CHECK    →    3. USE    →    4. FREE    →    5. NULL
   malloc()        if NULL?         *ptr = x       free(ptr)       ptr = NULL
```

**Every allocation must eventually be freed.** Forgetting to free causes **memory leaks**. Using memory after freeing causes **undefined behavior** (crashes, security vulnerabilities).

**Characteristics of Heap Memory:**

| Aspect            | Description                                    |
|-------------------|------------------------------------------------|
| **Speed**         | Slower than stack (bookkeeping overhead)       |
| **Allocation**    | Manual via malloc/calloc/realloc               |
| **Deallocation**  | Manual via free                                |
| **Size limit**    | Limited by available RAM (gigabytes)           |
| **Lifetime**      | Until explicitly freed                         |
| **Thread safety** | Shared between threads (needs synchronization) |

---

## The malloc Family

The standard library provides four functions for heap memory management. Each has a specific use case.

### `malloc(size)` — Allocate Uninitialized Memory

```c
#include <stdlib.h>

// Allocate space for 100 integers
int *arr = malloc(100 * sizeof(int));

// WARNING: Memory contains garbage (whatever was there before)!
printf("%d\n", arr[0]);  // Could print anything: 0, -12345, 0xDEADBEEF...
```

**Use `malloc` when** you're going to immediately initialize all the memory yourself.

### `calloc(count, size)` — Allocate and Zero-Initialize

```c
// Allocate 100 integers, all initialized to 0
int *arr = calloc(100, sizeof(int));

printf("%d\n", arr[0]);  // Guaranteed to be 0
```

**Use `calloc` when** you need zero-initialized memory (safer default).

> 💡 **Performance Note**: `calloc` is slightly slower than `malloc` because it must zero the memory. For large allocations where you'll overwrite everything anyway, `malloc` is faster.

### `realloc(ptr, new_size)` — Resize Existing Allocation

```c
int *arr = malloc(10 * sizeof(int));
// arr can hold 10 integers

// Need more space? Resize!
int *new_arr = realloc(arr, 20 * sizeof(int));

if (new_arr == NULL) {
    // Resize failed, but arr is still valid!
    free(arr);
    exit(1);
}
arr = new_arr;  // Update pointer (may have moved!)
// arr can now hold 20 integers
```

**Critical `realloc` behaviors:**
1. **May move your data** to a new location (old pointer becomes invalid)
2. **Returns NULL on failure** but doesn't free the original memory
3. **Preserves existing data** up to the smaller of old/new sizes
4. **`realloc(NULL, size)`** is equivalent to `malloc(size)`
5. **`realloc(ptr, 0)`** behavior is undefined (don't do this!)

### `free(ptr)` — Release Memory

```c
int *arr = malloc(100 * sizeof(int));
// ... use arr ...
free(arr);      // Memory returned to system
arr = NULL;     // Prevent dangling pointer (good practice!)

// Safe: free(NULL) does nothing
free(NULL);     // ✅ No crash, no effect
```

### Quick Reference Table

| Function  | Signature                                 | Initializes? | Use Case                              |
|-----------|-------------------------------------------|--------------|---------------------------------------|
| `malloc`  | `void *malloc(size_t size)`               | No (garbage) | Fast allocation, you'll initialize    |
| `calloc`  | `void *calloc(size_t count, size_t size)` | Yes (zeros)  | Need zero-initialized memory          |
| `realloc` | `void *realloc(void *ptr, size_t size)`   | Preserves    | Growing/shrinking existing allocation |
| `free`    | `void free(void *ptr)`                    | N/A          | Release memory                        |

---

## The sizeof Operator

`sizeof` returns the size in bytes of a type or variable. **Always use it** to make your code portable across different systems.

### Basic Usage

```c
printf("char:   %zu bytes\n", sizeof(char));    // Always 1
printf("int:    %zu bytes\n", sizeof(int));     // Usually 4
printf("long:   %zu bytes\n", sizeof(long));    // 4 or 8
printf("double: %zu bytes\n", sizeof(double));  // Usually 8
printf("pointer: %zu bytes\n", sizeof(void*));  // 4 (32-bit) or 8 (64-bit)
```

### With malloc — Three Styles

```c
// Style 1: ❌ FRAGILE - Assumes int is 4 bytes
int *arr = malloc(40);  // 10 ints... maybe?

// Style 2: ✅ GOOD - Uses sizeof(type)
int *arr = malloc(10 * sizeof(int));

// Style 3: ✅ BEST - Uses sizeof(*variable)
int *arr = malloc(10 * sizeof(*arr));
// If you change `int *arr` to `long *arr`, allocation auto-adjusts!
```

### sizeof with Structs

```c
typedef struct {
    int x;      // 4 bytes
    int y;      // 4 bytes
    char c;     // 1 byte
} Point;

printf("Point size: %zu\n", sizeof(Point));  // Might be 12, not 9!
// Why? Padding for alignment. The compiler adds invisible bytes.
```

> 💡 **Alignment**: CPUs access memory most efficiently at aligned addresses (e.g., 4-byte values at addresses divisible by 4). Compilers add padding to ensure this.

### sizeof with Arrays

```c
int arr[10];
printf("Array size: %zu bytes\n", sizeof(arr));      // 40 (10 × 4)
printf("Element count: %zu\n", sizeof(arr) / sizeof(arr[0]));  // 10

// BUT! This doesn't work with pointers:
int *ptr = arr;
printf("Pointer size: %zu\n", sizeof(ptr));  // 8 (just the pointer!)
```

---

## Basic Allocation Patterns

Understanding these common patterns will help you choose the right approach for your needs. We'll cover the essential patterns here; more complex patterns are in [Advanced Topics](#advanced-topics).

### Pattern 1: Single Object

The simplest case — allocate one struct or value.

```c
GameState *game = malloc(sizeof(GameState));
if (game == NULL) { 
    perror("malloc");
    exit(EXIT_FAILURE);
}

// Initialize and use
game->status = ONGOING;
game->move_count = 0;

// Clean up
free(game);
game = NULL;
```

**Memory layout:**
```
game ──────────►┌─────────────────┐
                │   GameState     │  (one contiguous block)
                │   .status       │
                │   .move_count   │
                │   .board[9]     │
                └─────────────────┘
```

### Pattern 2: Array of Objects (Contiguous)

Allocate multiple structs in one block — best for cache performance.

```c
// Allocate array of 10 games
int num_games = 10;
GameState *games = malloc(num_games * sizeof(GameState));
if (games == NULL) { /* handle error */ }

// Access as array (pointer arithmetic)
games[0].status = ONGOING;
games[5].move_count = 3;

// Or equivalently:
(games + 5)->move_count = 3;

free(games);  // One free for the entire array
```

**Memory layout:**
```
games ─────────►┌─────────────┬─────────────┬─────────────┬─────┐
                │  games[0]   │  games[1]   │  games[2]   │ ... │
                └─────────────┴─────────────┴─────────────┴─────┘
                      ▲ All contiguous in memory
```

> 📚 **More Patterns**: For array-of-pointers and 2D array patterns, see [Advanced Topics](#advanced-topics) after completing the basics.

---

## Memory Errors: The Deadly Sins

These are the bugs that crash programs, corrupt data, and create security vulnerabilities. Learn to recognize and avoid them!

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

> ⚠️ **Why it's dangerous**: An uninitialized pointer contains whatever bits were in that memory location. It might point to valid memory (silent corruption) or invalid memory (crash). Both are bad.

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

---

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

## Debugging Memory Issues

Memory bugs are notoriously hard to track down. Here's your toolkit:

### 1. Compile with All Warnings

```bash
gcc -std=c99 -Wall -Wextra -pedantic -Werror -g -O0 \
    src/*.c -Iinclude -o tictactoe
```

| Flag | Purpose |
|------|---------|
| `-Wall` | Enable all common warnings |
| `-Wextra` | Enable extra warnings |
| `-pedantic` | Reject non-standard features |
| `-Werror` | Treat warnings as errors (forces you to fix them) |
| `-g` | Include debug symbols |
| `-O0` | Disable optimization (easier to debug) |

### 2. Use Valgrind (Linux/macOS)

Valgrind detects memory leaks, use-after-free, and buffer overflows:

```bash
valgrind --leak-check=full --show-leak-kinds=all ./tictactoe
```

**Sample Valgrind output for a leak:**
```
==12345== 40 bytes in 1 blocks are definitely lost in loss record 1 of 1
==12345==    at 0x4C2FB55: malloc (in /usr/lib/valgrind/...)
==12345==    by 0x10915C: create_game (game.c:42)
==12345==    by 0x109189: main (main.c:15)
```

### 3. Use AddressSanitizer (Modern Compilers)

Built into GCC and Clang — catches errors at runtime:

```bash
gcc -fsanitize=address -g -O0 src/*.c -Iinclude -o tictactoe
./tictactoe  # Crashes immediately on memory errors with helpful messages
```

### 4. Add Strategic Assertions

```c
#include <assert.h>

void game_make_move(GameState *state, int position) {
    assert(state != NULL && "state pointer is NULL");
    assert(position >= 0 && position < 9 && "position out of bounds");
    assert(state->board[position] == EMPTY && "position already occupied");
    // ...
}
```

### 5. Use a Debugger (GDB)

```bash
gdb ./tictactoe
(gdb) break game_make_move    # Set breakpoint
(gdb) run                     # Start program
(gdb) print state             # Inspect variable
(gdb) print *state            # Dereference pointer
(gdb) backtrace               # Show call stack
(gdb) watch state->board[0]   # Break when value changes
```

---

## Self-Assessment Checklist

Before moving on, make sure you can:

- [ ] **Explain** the difference between stack and heap memory
- [ ] **Describe** what happens when a function returns (stack frame cleanup)
- [ ] **Use** `malloc`, `calloc`, `realloc`, and `free` correctly
- [ ] **Always** check if `malloc` returned NULL
- [ ] **Calculate** allocation size using `sizeof`
- [ ] **Recognize** the 8 deadly memory errors
- [ ] **Write** code that frees memory in reverse order of allocation
- [ ] **Set** pointers to NULL after freeing them
- [ ] **Run** Valgrind to detect memory leaks
- [ ] **Explain** why this project uses stack allocation

> 💡 **Tip**: If you're unsure about any item, revisit that section or try the related exercise.

---

## Practical Exercises

### Exercise 1: Identify the Bug

**Goal**: Spot the memory error in each code snippet.

**Snippet A:**
```c
int *create_array(int size) {
    int arr[size];
    for (int i = 0; i < size; i++) {
        arr[i] = i * 2;
    }
    return arr;
}
```

<details>
<summary><strong>Click to reveal answer</strong></summary>

**Bug**: Returning address of local (stack) variable.

`arr` is allocated on the stack and destroyed when `create_array` returns. The returned pointer is **dangling** — it points to memory that no longer belongs to the program.

**Fix:**
```c
int *create_array(int size) {
    int *arr = malloc(size * sizeof(int));
    if (arr == NULL) return NULL;
    for (int i = 0; i < size; i++) {
        arr[i] = i * 2;
    }
    return arr;  // Caller must free!
}
```

</details>

---

**Snippet B:**
```c
void process_data(void) {
    char *buffer = malloc(100);
    strcpy(buffer, "Hello, World!");
    
    if (strlen(buffer) > 10) {
        return;  // Early return
    }
    
    free(buffer);
}
```

<details>
<summary><strong>Click to reveal answer</strong></summary>

**Bug**: Memory leak on early return.

When `strlen(buffer) > 10` (which is true for "Hello, World!"), the function returns without freeing `buffer`. This leaks 100 bytes every time `process_data` is called.

**Fix:**
```c
void process_data(void) {
    char *buffer = malloc(100);
    if (buffer == NULL) return;
    
    strcpy(buffer, "Hello, World!");
    
    if (strlen(buffer) > 10) {
        free(buffer);  // Free before early return!
        return;
    }
    
    free(buffer);
}

// Or better — use goto for cleanup:
void process_data(void) {
    char *buffer = malloc(100);
    if (buffer == NULL) return;
    
    strcpy(buffer, "Hello, World!");
    
    if (strlen(buffer) > 10) {
        goto cleanup;
    }
    
    // ... more processing ...
    
cleanup:
    free(buffer);
}
```

</details>

---

**Snippet C:**
```c
int *ptr = malloc(sizeof(int));
*ptr = 42;
free(ptr);
printf("Value: %d\n", *ptr);
```

<details>
<summary><strong>Click to reveal answer</strong></summary>

**Bug**: Use after free (dangling pointer).

After `free(ptr)`, the memory is returned to the system. Accessing `*ptr` is undefined behavior — it might print 42, print garbage, or crash.

**Fix:**
```c
int *ptr = malloc(sizeof(int));
*ptr = 42;
printf("Value: %d\n", *ptr);  // Print BEFORE freeing
free(ptr);
ptr = NULL;  // Prevent accidental reuse
```

</details>

---

### Exercise 2: Fix the Memory Leak

**Goal**: Rewrite this function to properly manage memory.

```c
char **read_lines(FILE *file, int *count) {
    char **lines = malloc(100 * sizeof(char *));
    char buffer[256];
    *count = 0;
    
    while (fgets(buffer, 256, file) && *count < 100) {
        lines[*count] = malloc(strlen(buffer) + 1);
        strcpy(lines[*count], buffer);
        (*count)++;
    }
    
    return lines;
}

// This code leaks memory! How should it be freed?
```

**Task**: Write a `free_lines` function and show how to use it.

<details>
<summary><strong>Click to reveal answer</strong></summary>

```c
void free_lines(char **lines, int count) {
    if (lines == NULL) return;
    
    // Free each string first
    for (int i = 0; i < count; i++) {
        free(lines[i]);
    }
    
    // Then free the array of pointers
    free(lines);
}

// Usage:
int main(void) {
    FILE *file = fopen("data.txt", "r");
    if (file == NULL) return 1;
    
    int count;
    char **lines = read_lines(file, &count);
    fclose(file);
    
    // Process lines...
    for (int i = 0; i < count; i++) {
        printf("%s", lines[i]);
    }
    
    // Clean up
    free_lines(lines, count);
    
    return 0;
}
```

**Key Points:**
- Free in reverse order of allocation (inner before outer)
- Track how many items were allocated (`count`)
- Handle NULL gracefully

</details>

> 📚 **More Exercises**: For more challenging exercises including dynamic arrays, 2D matrices, and memory pools, see [Advanced Topics](#advanced-topics).

---

## Next Steps

Continue to [Lesson 10: The Challenge 🐍](LESSON_10_The_Challenge.md) →

← Previous: [Lesson 8: Function Pointers and Abstraction](LESSON_8_Function_Pointers_And_Abstraction.md)

← Back to [Learning Guide Index](LEARNING_GUIDE.md)

---

## Bonus: Advanced Topics

> 🎯 **Prerequisites**: Complete [Lesson 10: The Challenge](LESSON_10_The_Challenge.md) before tackling this section. The patterns and exercises below combine memory management with more complex data structures and algorithms.

Once you're comfortable with the core concepts and have completed Lesson 10, come back here to deepen your understanding.

---

### Advanced Allocation Patterns
#### Pattern 3: Array of Pointers (Non-contiguous)
Each element is a separate allocation — flexible but more overhead.

```c
int num_games = 10;

// Step 1: Allocate array of pointers
GameState **games = malloc(num_games * sizeof(GameState *));
if (games == NULL) { /* handle error */ }

// Step 2: Allocate each object
for (int i = 0; i < num_games; i++) {
    games[i] = malloc(sizeof(GameState));
    if (games[i] == NULL) {
        // Handle partial failure: free what we allocated
        for (int j = 0; j < i; j++) {
            free(games[j]);
        }
        free(games);
        exit(EXIT_FAILURE);
    }
}

// Use: games[i]->status, games[i]->board, etc.

// Free in reverse order (inner first, then outer)
for (int i = 0; i < num_games; i++) {
    free(games[i]);
}
free(games);
```

**Memory layout:**
```
games ─────────►┌───────┬───────┬───────┬─────┐
                │ ptr 0 │ ptr 1 │ ptr 2 │ ... │
                └───┬───┴───┬───┴───┬───┴─────┘
                    │       │       │
                    ▼       ▼       ▼
                ┌───────┐┌───────┐┌───────┐
                │Game 0 ││Game 1 ││Game 2 │  (scattered in heap)
                └───────┘└───────┘└───────┘
```

**When to use each:**
| Pattern | Pros | Cons |
|---------|------|------|
| Contiguous | Fast access, cache-friendly, one free | Fixed size, may need realloc |
| Pointers | Easy to resize, reorder, NULL some | More allocations, cache misses |

#### Pattern 4: 2D Array (Contiguous)
For matrices, grids, or game boards — one allocation, manual indexing.

```c
int rows = 3, cols = 4;

// Single allocation for entire 2D array
int *matrix = malloc(rows * cols * sizeof(int));
if (matrix == NULL) { /* handle error */ }

// Access with formula: matrix[row * cols + col]
matrix[1 * cols + 2] = 42;  // Row 1, Column 2

// Helper macro for cleaner syntax
#define MATRIX(r, c) matrix[(r) * cols + (c)]
MATRIX(1, 2) = 42;  // Same as above

free(matrix);  // One free
```

**Memory layout:**
```
matrix ────────►┌────┬────┬────┬────┬────┬────┬────┬────┬────┬────┬────┬────┐
                │0,0 │0,1 │0,2 │0,3 │1,0 │1,1 │1,2 │1,3 │2,0 │2,1 │2,2 │2,3 │
                └────┴────┴────┴────┴────┴────┴────┴────┴────┴────┴────┴────┘
                       Row 0        │       Row 1        │       Row 2
```

#### Pattern 5: 2D Array (Array of Row Pointers)
Classic "array of arrays" — natural `[row][col]` syntax but more allocations.

```c
int rows = 3, cols = 4;

// Allocate array of row pointers
int **matrix = malloc(rows * sizeof(int *));
if (matrix == NULL) { /* handle error */ }

// Allocate each row
for (int r = 0; r < rows; r++) {
    matrix[r] = malloc(cols * sizeof(int));
    if (matrix[r] == NULL) { /* handle partial failure */ }
}

// Natural 2D access
matrix[1][2] = 42;  // Row 1, Column 2

// Free each row, then the pointer array
for (int r = 0; r < rows; r++) {
    free(matrix[r]);
}
free(matrix);
```

**Memory layout:**
```
matrix ────────►┌──────────┐
                │ row_ptr 0├──────►┌────┬────┬────┬────┐
                ├──────────┤       │0,0 │0,1 │0,2 │0,3 │
                │ row_ptr 1├───►   └────┴────┴────┴────┘
                ├──────────┤   └──►┌────┬────┬────┬────┐
                │ row_ptr 2├───    │1,0 │1,1 │1,2 │1,3 │
                └──────────┘   └──►└────┴────┴────┴────┘
                               ┌────┬────┬────┬────┐
                               │2,0 │2,1 │2,2 │2,3 │
                               └────┴────┴────┴────┘
```

---

### Advanced Exercises
#### Exercise 1: Dynamic Array
**Goal**: Implement a growable array that resizes automatically.

```c
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *data;
    int size;      // Number of elements currently stored
    int capacity;  // Total space allocated
} DynamicArray;

// TODO: Implement these functions
DynamicArray *array_create(int initial_capacity);
void array_push(DynamicArray *arr, int value);
int array_get(DynamicArray *arr, int index);
void array_free(DynamicArray *arr);

int main(void) {
    DynamicArray *arr = array_create(4);
    
    for (int i = 0; i < 20; i++) {
        array_push(arr, i * 10);
        printf("Pushed %d, size=%d, capacity=%d\n", 
               i * 10, arr->size, arr->capacity);
    }
    
    printf("Element at index 15: %d\n", array_get(arr, 15));
    
    array_free(arr);
    return 0;
}
```

<details>
<summary><strong>Click to reveal answer</strong></summary>

```c
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *data;
    int size;
    int capacity;
} DynamicArray;

DynamicArray *array_create(int initial_capacity) {
    DynamicArray *arr = malloc(sizeof(DynamicArray));
    if (arr == NULL) return NULL;
    
    arr->data = malloc(initial_capacity * sizeof(int));
    if (arr->data == NULL) {
        free(arr);
        return NULL;
    }
    
    arr->size = 0;
    arr->capacity = initial_capacity;
    return arr;
}

void array_push(DynamicArray *arr, int value) {
    if (arr == NULL) return;
    
    // Need to grow?
    if (arr->size >= arr->capacity) {
        int new_capacity = arr->capacity * 2;  // Double the size
        int *new_data = realloc(arr->data, new_capacity * sizeof(int));
        
        if (new_data == NULL) {
            fprintf(stderr, "Failed to grow array\n");
            return;
        }
        
        arr->data = new_data;
        arr->capacity = new_capacity;
    }
    
    arr->data[arr->size] = value;
    arr->size++;
}

int array_get(DynamicArray *arr, int index) {
    if (arr == NULL || index < 0 || index >= arr->size) {
        fprintf(stderr, "Index out of bounds: %d\n", index);
        return -1;  // Error value
    }
    return arr->data[index];
}

void array_free(DynamicArray *arr) {
    if (arr == NULL) return;
    
    free(arr->data);  // Free the data array first
    free(arr);        // Then free the struct
}

/* Output:
Pushed 0, size=1, capacity=4
Pushed 10, size=2, capacity=4
Pushed 20, size=3, capacity=4
Pushed 30, size=4, capacity=4
Pushed 40, size=5, capacity=8    <-- Grew!
...
Pushed 150, size=16, capacity=16
Pushed 160, size=17, capacity=32 <-- Grew again!
...
Element at index 15: 150
*/
```

**Key Points:**
- Use `realloc` to grow the array (doubles capacity each time)
- Check `realloc` result before updating the pointer
- Free inner data before freeing the struct

</details>

---

#### Exercise 2: 2D Matrix
**Goal**: Implement a dynamically allocated 2D matrix.

```c
#include <stdio.h>
#include <stdlib.h>

// TODO: Implement these functions
int **matrix_create(int rows, int cols);
void matrix_set(int **matrix, int row, int col, int value);
int matrix_get(int **matrix, int row, int col);
void matrix_print(int **matrix, int rows, int cols);
void matrix_free(int **matrix, int rows);

int main(void) {
    int rows = 3, cols = 4;
    int **matrix = matrix_create(rows, cols);
    
    // Fill with values
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            matrix_set(matrix, r, c, r * cols + c);
        }
    }
    
    matrix_print(matrix, rows, cols);
    
    printf("Value at [1][2]: %d\n", matrix_get(matrix, 1, 2));
    
    matrix_free(matrix, rows);
    return 0;
}
```

<details>
<summary><strong>Click to reveal answer</strong></summary>

```c
#include <stdio.h>
#include <stdlib.h>

int **matrix_create(int rows, int cols) {
    // Allocate array of row pointers
    int **matrix = malloc(rows * sizeof(int *));
    if (matrix == NULL) return NULL;
    
    // Allocate each row
    for (int r = 0; r < rows; r++) {
        matrix[r] = calloc(cols, sizeof(int));  // Zero-initialized
        if (matrix[r] == NULL) {
            // Cleanup on failure
            for (int j = 0; j < r; j++) {
                free(matrix[j]);
            }
            free(matrix);
            return NULL;
        }
    }
    
    return matrix;
}

void matrix_set(int **matrix, int row, int col, int value) {
    if (matrix != NULL && matrix[row] != NULL) {
        matrix[row][col] = value;
    }
}

int matrix_get(int **matrix, int row, int col) {
    if (matrix != NULL && matrix[row] != NULL) {
        return matrix[row][col];
    }
    return 0;  // Error value
}

void matrix_print(int **matrix, int rows, int cols) {
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            printf("%3d ", matrix[r][c]);
        }
        printf("\n");
    }
}

void matrix_free(int **matrix, int rows) {
    if (matrix == NULL) return;
    
    // Free each row first
    for (int r = 0; r < rows; r++) {
        free(matrix[r]);
    }
    
    // Then free the row pointer array
    free(matrix);
}

/* Output:
  0   1   2   3 
  4   5   6   7 
  8   9  10  11 
Value at [1][2]: 6
*/
```

**Key Points:**
- Allocate row pointers first, then each row
- Handle partial allocation failure (cleanup what was allocated)
- Free rows before freeing the row pointer array
- Using `calloc` gives zero-initialized memory

</details>

---

#### Exercise 3: Memory-Safe String Functions
**Goal**: Write safe string functions that manage their own memory.

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// TODO: Implement these functions

// Create a copy of the string (caller must free)
char *string_duplicate(const char *str);

// Concatenate two strings into a new string (caller must free)
char *string_concat(const char *str1, const char *str2);

// Join an array of strings with a separator (caller must free)
char *string_join(const char **strings, int count, const char *separator);

int main(void) {
    char *dup = string_duplicate("Hello");
    printf("Duplicate: %s\n", dup);
    free(dup);
    
    char *cat = string_concat("Hello, ", "World!");
    printf("Concat: %s\n", cat);
    free(cat);
    
    const char *words[] = {"one", "two", "three"};
    char *joined = string_join(words, 3, "-");
    printf("Joined: %s\n", joined);
    free(joined);
    
    return 0;
}
```

<details>
<summary><strong>Click to reveal answer</strong></summary>

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *string_duplicate(const char *str) {
    if (str == NULL) return NULL;
    
    size_t len = strlen(str);
    char *copy = malloc(len + 1);  // +1 for null terminator
    if (copy == NULL) return NULL;
    
    strcpy(copy, str);
    return copy;
}

char *string_concat(const char *str1, const char *str2) {
    if (str1 == NULL || str2 == NULL) return NULL;
    
    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);
    
    char *result = malloc(len1 + len2 + 1);
    if (result == NULL) return NULL;
    
    strcpy(result, str1);
    strcat(result, str2);
    
    return result;
}

char *string_join(const char **strings, int count, const char *separator) {
    if (strings == NULL || count <= 0 || separator == NULL) {
        return NULL;
    }
    
    // Calculate total length needed
    size_t total_len = 0;
    size_t sep_len = strlen(separator);
    
    for (int i = 0; i < count; i++) {
        total_len += strlen(strings[i]);
        if (i < count - 1) {
            total_len += sep_len;  // Add separator length between strings
        }
    }
    
    // Allocate result buffer
    char *result = malloc(total_len + 1);
    if (result == NULL) return NULL;
    
    // Build the result
    result[0] = '\0';  // Start with empty string
    for (int i = 0; i < count; i++) {
        strcat(result, strings[i]);
        if (i < count - 1) {
            strcat(result, separator);
        }
    }
    
    return result;
}

/* Output:
Duplicate: Hello
Concat: Hello, World!
Joined: one-two-three
*/
```

**Key Points:**
- Always allocate `strlen + 1` to include the null terminator
- Calculate total size before allocating
- Handle NULL inputs gracefully
- Document that caller must free the returned memory

</details>

---

#### Challenge Exercise: Memory Pool
**Goal**: Implement a simple memory pool for fixed-size allocations.

A memory pool pre-allocates a large block and hands out fixed-size chunks. This is faster than `malloc` for many small allocations and avoids fragmentation.

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define POOL_SIZE 10

typedef struct {
    void *memory;           // The pre-allocated block
    int item_size;          // Size of each item
    int capacity;           // Total number of items
    int *free_list;         // Indices of free slots
    int free_count;         // Number of free slots
} MemoryPool;

// TODO: Implement these functions
MemoryPool *pool_create(int item_size, int capacity);
void *pool_alloc(MemoryPool *pool);
void pool_free(MemoryPool *pool, void *ptr);
void pool_destroy(MemoryPool *pool);
```

<details>
<summary><strong>Click to reveal answer</strong></summary>

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    void *memory;
    int item_size;
    int capacity;
    int *free_list;
    int free_count;
} MemoryPool;

MemoryPool *pool_create(int item_size, int capacity) {
    MemoryPool *pool = malloc(sizeof(MemoryPool));
    if (pool == NULL) return NULL;
    
    pool->memory = malloc(item_size * capacity);
    if (pool->memory == NULL) {
        free(pool);
        return NULL;
    }
    
    pool->free_list = malloc(capacity * sizeof(int));
    if (pool->free_list == NULL) {
        free(pool->memory);
        free(pool);
        return NULL;
    }
    
    pool->item_size = item_size;
    pool->capacity = capacity;
    pool->free_count = capacity;
    
    // Initialize free list with all indices
    for (int i = 0; i < capacity; i++) {
        pool->free_list[i] = i;
    }
    
    return pool;
}

void *pool_alloc(MemoryPool *pool) {
    if (pool == NULL || pool->free_count == 0) {
        return NULL;  // Pool exhausted
    }
    
    // Pop an index from the free list
    int index = pool->free_list[--pool->free_count];
    
    // Calculate the address of this slot
    return (char *)pool->memory + (index * pool->item_size);
}

void pool_free(MemoryPool *pool, void *ptr) {
    if (pool == NULL || ptr == NULL) return;
    
    // Calculate which index this pointer corresponds to
    int offset = (char *)ptr - (char *)pool->memory;
    int index = offset / pool->item_size;
    
    // Validate the pointer
    if (index < 0 || index >= pool->capacity) {
        fprintf(stderr, "Invalid pointer returned to pool\n");
        return;
    }
    
    // Push index back onto free list
    pool->free_list[pool->free_count++] = index;
}

void pool_destroy(MemoryPool *pool) {
    if (pool == NULL) return;
    
    free(pool->free_list);
    free(pool->memory);
    free(pool);
}

// Demo
typedef struct {
    int id;
    char name[32];
} Player;

int main(void) {
    MemoryPool *pool = pool_create(sizeof(Player), 5);
    
    // Allocate some players
    Player *p1 = pool_alloc(pool);
    Player *p2 = pool_alloc(pool);
    Player *p3 = pool_alloc(pool);
    
    p1->id = 1; strcpy(p1->name, "Alice");
    p2->id = 2; strcpy(p2->name, "Bob");
    p3->id = 3; strcpy(p3->name, "Charlie");
    
    printf("Player 1: %s (id=%d)\n", p1->name, p1->id);
    printf("Player 2: %s (id=%d)\n", p2->name, p2->id);
    
    // Return one to the pool
    pool_free(pool, p2);
    
    // Allocate again (reuses p2's slot!)
    Player *p4 = pool_alloc(pool);
    p4->id = 4; strcpy(p4->name, "Diana");
    printf("Player 4: %s (id=%d)\n", p4->name, p4->id);
    
    pool_destroy(pool);
    return 0;
}

/* Output:
Player 1: Alice (id=1)
Player 2: Bob (id=2)
Player 4: Diana (id=4)
*/
```

**Key Points:**
- Pre-allocate one large block instead of many small ones
- Use a free list (stack) to track available slots
- Pointer arithmetic to find slot addresses
- Much faster than `malloc`/`free` for many small allocations
- Used in game engines, embedded systems, and high-performance servers

</details>

---

### Further Reading
Once you've mastered these concepts, explore:

- **Standard Library**: Study how `realloc` works internally
- **Memory Allocators**: jemalloc, tcmalloc, and other high-performance allocators
- **Garbage Collection**: How other languages manage memory automatically
- **Embedded Systems**: Memory management without an OS
- **Valgrind Internals**: How memory checking tools work


