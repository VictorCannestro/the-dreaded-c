# Pitfalls: Memory & Pointers

> Part of the [C Pitfalls](README.md) series.

Errors in this category involve incorrect memory access, pointer misuse, array boundary violations, and misuse of memory functions. They are among the most dangerous C bugs because they produce **undefined behaviour** with no guaranteed runtime error.

## Pitfalls in This Section

- [1. Missing Include for NULL](#1-missing-include-for-null)
- [4. Array Out of Bounds](#4-array-out-of-bounds)
- [5. Off-by-One Errors](#5-off-by-one-errors)
- [6. Pointer Dereferencing NULL](#6-pointer-dereferencing-null)
- [14. `memset` with Non-Zero Values](#14-memset-with-non-zero-values)

---

### 1. Missing Include for NULL

**Severity:** 🔴 Compile Error  
**Frequency:** Very Common for Beginners

#### The Problem

```c
// ai.c
if (state == NULL) {
    return -1;
}
// Compiler error: use of undeclared identifier 'NULL'
```

#### Why This Happens

`NULL` is **not a built-in keyword** in C. It's actually a macro that must be defined by including the appropriate header file. Many beginners assume `NULL` is part of the language itself, but it's actually defined as:

```c
#define NULL ((void*)0)  // Or sometimes just 0
```

#### The Solution

```c
#include <stdlib.h>  // Defines NULL, malloc, free, etc.

if (state == NULL) {
    return -1;  // Now compiles correctly
}
```

#### Where NULL is Defined

`NULL` is defined in multiple standard headers:
- `<stdlib.h>` - General utilities (malloc, free, exit)
- `<stdio.h>` - Input/output functions (printf, scanf, FILE*)
- `<string.h>` - String operations (strcpy, strlen)
- `<stddef.h>` - Standard definitions (size_t, ptrdiff_t)

#### Best Practices

1. **Include the header for the functions you use:**
   ```c
   #include <stdlib.h>  // If using malloc/free
   #include <stdio.h>   // If using FILE* or I/O functions
   #include <string.h>  // If using string functions
   ```

2. **Use `stddef.h` for NULL-only needs:**
   ```c
   #include <stddef.h>  // Minimal header just for NULL
   ```

3. **Modern alternative - use 0 or nullptr (C23):**
   ```c
   if (state == 0) { }  // Always works, no include needed
   ```

#### Debugging Tips

If you see this error, check:
- Missing `#include` at the top of your file
- Typo in header name (`<stdlib.h>` not `<stdlib>`)
- Using NULL in header files without their own proper includes

---

### 4. Array Out of Bounds

**Severity:** 🔴 Critical - Undefined Behavior  
**Frequency:** Very Common

#### The Problem

```c
CellValue board[9];
board[9] = CELL_X;  // OUT OF BOUNDS! Array indices are 0-8
```

#### Why This is Extremely Dangerous

Unlike Java, Python, or modern languages, C does **not** perform runtime bounds checking:

```c
int array[5] = {1, 2, 3, 4, 5};
array[10] = 42;  // NO RUNTIME ERROR!
                 // Corrupts memory silently
```

**What actually happens:**

1. **Memory Corruption:** You overwrite whatever happens to be next to the array in memory:
   ```
   Memory Layout:
   [array[0]][array[1]][array[2]][array[3]][array[4]][other_variable][...]
                                             ↑
                                      array[5] writes here!
   ```

2. **Unpredictable Behavior:**
   - Might corrupt other variables
   - Might corrupt function return addresses (security exploit!)
   - Might corrupt heap metadata (malloc/free will crash)
   - Might hit read-only memory (segmentation fault)
   - Might seem to "work" (worst case - bug hiding)

3. **Heisenbug:** The bug may disappear when you add debugging code because it changes the memory layout!

#### Real-World Example

```c
void process_input(void) {
    int scores[3];
    int important_flag = 1;
    
    // User enters 4 scores instead of 3
    for (int i = 0; i <= 3; i++) {  // BUG: should be i < 3
        scanf("%d", &scores[i]);
    }
    
    // scores[3] overwrites important_flag!
    // important_flag is now unpredictable
    
    if (important_flag) {
        save_data();  // Might not run!
    }
}
```

#### The Solution

**1. Always Validate Indices:**
```c
int board_place(CellValue board[BOARD_SIZE], int position, CellValue symbol) {
    // Bounds check BEFORE access
    if (position < 0 || position >= BOARD_SIZE) {
        return -1;  // Error code
    }
    board[position] = symbol;
    return 0;
}
```

**2. Use Constants Instead of Magic Numbers:**
```c
#define BOARD_SIZE 9

CellValue board[BOARD_SIZE];

// Now obvious that this is wrong:
for (int i = 0; i <= BOARD_SIZE; i++) { ... }  // i <= is suspicious
```

**3. Use sizeof for Dynamic Sizing:**
```c
int values[] = {1, 2, 3, 4, 5};
size_t count = sizeof(values) / sizeof(values[0]);

for (size_t i = 0; i < count; i++) {
    printf("%d\n", values[i]);  // Safe
}
```

#### Debugging Techniques

**1. Enable Address Sanitizer (ASan):**
```bash
gcc -fsanitize=address -g -o program program.c
./program

# Output will show:
# ERROR: AddressSanitizer: heap-buffer-overflow
# Write of size 4 at 0x60300000 ...
```

**2. Use Valgrind:**
```bash
valgrind --leak-check=full ./program

# Output will show:
# Invalid write of size 4
# at 0x...: function_name (file.c:line)
```

**3. Debug Build with Bounds Checking (GCC):**
```bash
gcc -D_FORTIFY_SOURCE=2 -O2 -fstack-protector-strong ...
```

**4. Static Analysis:**
```bash
clang --analyze program.c
# or
cppcheck --enable=all program.c
```

#### Best Practices

1. **Use Functions for Array Access:**
   ```c
   CellValue board_get(const CellValue board[BOARD_SIZE], int pos) {
       if (pos < 0 || pos >= BOARD_SIZE) {
           return CELL_INVALID;  // or assert()
       }
       return board[pos];
   }
   ```

2. **Pass Size Information:**
   ```c
   void process_array(int *array, size_t size) {
       for (size_t i = 0; i < size; i++) {  // Use passed size
           array[i] = 0;
       }
   }
   ```

3. **Use Assertions in Debug Builds:**
   ```c
   #include <assert.h>
   
   void set_score(int scores[3], int index, int value) {
       assert(index >= 0 && index < 3);
       scores[index] = value;
   }
   ```

#### Common Mistakes

```c
// ❌ WRONG: Off-by-one with <=
for (int i = 0; i <= 9; i++) {
    board[i] = CELL_EMPTY;  // board[9] is out of bounds!
}

// ✅ CORRECT: Use <
for (int i = 0; i < 9; i++) {
    board[i] = CELL_EMPTY;
}

// ❌ WRONG: Mixing up size and index
int size = 10;
int array[size];
array[size] = 42;  // Out of bounds! Should be size-1

// ✅ CORRECT:
array[size - 1] = 42;
```

---

### 5. Off-by-One Errors

**Severity:** 🔴 Critical - Common Source of Bugs  
**Frequency:** Extremely Common (even for experienced developers)

#### The Problem

```c
for (int i = 0; i <= 9; i++) {  // BUG: Should be i < 9
    board[i] = CELL_EMPTY;      // board[9] is out of bounds!
}
```

#### Why This Happens

Off-by-one errors (also called "fencepost errors") occur due to:

1. **Confusion about array indexing:** Arrays are 0-indexed in C
2. **Mixing up size vs last valid index:** Size is 9, last index is 8
3. **Loop boundary confusion:** Using `<=` instead of `<`
4. **String null terminator:** Forgetting to account for `\0`

#### Common Patterns of Off-by-One Errors

**Pattern 1: Using <= instead of <**
```c
// ❌ WRONG
int array[10];
for (int i = 0; i <= 10; i++) {  // Iterates 11 times! (0-10)
    array[i] = 0;                // array[10] is out of bounds
}

// ✅ CORRECT
for (int i = 0; i < 10; i++) {   // Iterates 10 times (0-9)
    array[i] = 0;
}
```

**Pattern 2: Confusing Size with Last Index**
```c
// ❌ WRONG
#define SIZE 5
int numbers[SIZE];
int last = numbers[SIZE];  // Out of bounds! SIZE is 5, valid indices are 0-4

// ✅ CORRECT
int last = numbers[SIZE - 1];  // numbers[4] is the last element
```

**Pattern 3: String Buffer Allocation**
```c
// ❌ WRONG
char name[5];
strcpy(name, "Alice");  // Needs 6 chars: A-l-i-c-e-\0
                        // Buffer overflow!

// ✅ CORRECT
char name[6];           // 5 chars + 1 null terminator
strcpy(name, "Alice");
```

**Pattern 4: Range Checks**
```c
// ❌ WRONG
int position;
scanf("%d", &position);
if (position >= 0 && position <= BOARD_SIZE) {  // BUG: allows 9
    board[position] = CELL_X;
}

// ✅ CORRECT
if (position >= 0 && position < BOARD_SIZE) {   // Only allows 0-8
    board[position] = CELL_X;
}
```

**Pattern 5: Loop Starting from 1**
```c
// ❌ WRONG (if you intended to fill entire array)
for (int i = 1; i <= 10; i++) {
    array[i] = 0;  // Skips array[0], crashes at array[10]
}

// ✅ CORRECT
for (int i = 0; i < 10; i++) {
    array[i] = 0;
}
```

#### Real-World Example from Tic-Tac-Toe

```c
// Common off-by-one bug in board iteration:
// ❌ WRONG
for (int i = 0; i <= BOARD_SIZE; i++) {  // BUG: <= instead of <
    if (board[i] == CELL_EMPTY) {        // board[9] is out of bounds!
        return i;
    }
}

// ✅ CORRECT
for (int i = 0; i < BOARD_SIZE; i++) {   // Correct: i < 9
    if (board[i] == CELL_EMPTY) {
        return i;  // Returns 0-8, all valid indices
    }
}
```

#### The Mnemonic: "Less Than is Your Friend"

```c
// For arrays of size N:
// - Valid indices: 0 to N-1
// - Loop condition: i < N

int array[SIZE];

for (int i = 0; i < SIZE; i++) {  // i goes from 0 to SIZE-1 ✓
    // access array[i]
}
```

#### Prevention Strategies

**1. Use Symbolic Constants:**
```c
#define BOARD_SIZE 9

for (int i = 0; i < BOARD_SIZE; i++) { }  // Clear
for (int i = 0; i <= BOARD_SIZE; i++) { }  // Obviously wrong
```

**2. Calculate Size Automatically:**
```c
int values[] = {10, 20, 30, 40, 50};
size_t count = sizeof(values) / sizeof(values[0]);

for (size_t i = 0; i < count; i++) {
    printf("%d\n", values[i]);
}
```

**3. Use Assertions:**
```c
#include <assert.h>

void set_cell(CellValue board[9], int index, CellValue value) {
    assert(index >= 0 && index < 9);
    board[index] = value;
}
```

**4. Prefer Standard Library Functions:**
```c
// Instead of manual loops:
memset(board, CELL_EMPTY, sizeof(board));  // No off-by-one possible

// Or use memcpy:
memcpy(dest, src, BOARD_SIZE * sizeof(CellValue));
```

**5. Write Tests for Boundary Conditions:**
```c
void test_board_boundaries(void) {
    CellValue board[9];
    
    assert(board_place(board, 0, CELL_X) == 0);   // First position
    assert(board_place(board, 8, CELL_O) == 0);   // Last valid position
    assert(board_place(board, 9, CELL_X) == -1);  // Out of bounds
    assert(board_place(board, -1, CELL_X) == -1); // Negative index
}
```

#### Common Loop Patterns - Quick Reference

```c
int array[SIZE];

// ✅ Standard forward iteration
for (int i = 0; i < SIZE; i++) { }

// ✅ Backward iteration
for (int i = SIZE - 1; i >= 0; i--) { }

// ❌ WRONG - off by one
for (int i = 0; i <= SIZE; i++) { }      // Accesses array[SIZE]!
for (int i = 1; i <= SIZE; i++) { }      // Skips array[0]!
for (int i = SIZE - 1; i > 0; i--) { }   // Skips array[0]!
```

#### Remember: Arrays are 0-Indexed

```
Array size 9:  [0] [1] [2] [3] [4] [5] [6] [7] [8]
               ↑                               ↑
           First (0)                      Last (SIZE-1)

NOT:          [1] [2] [3] [4] [5] [6] [7] [8] [9]  ← WRONG!
```

---

### 6. Pointer Dereferencing NULL

**Severity:** 🔴 Critical - Immediate Crash  
**Frequency:** Very Common

#### The Problem

```c
void game_init(GameState *state) {
    state->status = ONGOING;  // CRASH if state is NULL!
}

int main(void) {
    GameState *state = NULL;
    game_init(state);  // Segmentation fault!
    return 0;
}
```

#### Why This Crashes

Dereferencing a NULL pointer causes a **segmentation fault** (SIGSEGV) because:

1. **NULL is address 0x0:** This address is protected by the operating system
2. **Memory protection:** The OS prevents access to address 0 to catch bugs
3. **Immediate termination:** Your program crashes with "Segmentation fault (core dumped)"

```c
GameState *state = NULL;  // state points to address 0x0

state->status = ONGOING;  // Try to write to address 0x0
                          // OS says: "No! SIGSEGV!"
```

#### Common Scenarios

**Scenario 1: Uninitialized Pointer**
```c
GameState *state;  // Uninitialized! Contains garbage address
state->status = ONGOING;  // Crash (or worse - corrupts random memory)
```

**Scenario 2: malloc Failure**
```c
GameState *state = malloc(sizeof(GameState));
// malloc returns NULL if memory allocation fails!
state->status = ONGOING;  // Crash if malloc failed
```

**Scenario 3: Function Returning NULL**
```c
GameState *find_game(int id) {
    // ... search ...
    return NULL;  // Not found
}

GameState *state = find_game(42);
state->status = ONGOING;  // Crash if game not found
```

**Scenario 4: Use After Free**
```c
GameState *state = malloc(sizeof(GameState));
free(state);
state = NULL;  // Good practice
// ... later ...
if (state != NULL) {  // Check prevents crash
    state->status = ONGOING;
}
```

#### The Solutions

**Approach 1: Defensive NULL Checks (Public API)**
```c
void game_init(GameState *state) {
    if (state == NULL) {
        return;  // Or return error code
    }
    state->status = ONGOING;  // Now safe
}
```

**Approach 2: Assert (Internal Functions)**
```c
#include <assert.h>

void game_init(GameState *state) {
    assert(state != NULL);  // Crashes in debug, removed in release
    state->status = ONGOING;
}
```

**Approach 3: Check malloc Returns**
```c
GameState *state = malloc(sizeof(GameState));
if (state == NULL) {
    fprintf(stderr, "Error: Out of memory\n");
    exit(EXIT_FAILURE);
}
state->status = ONGOING;  // Now safe
```

#### When to Use Which Approach

| Situation | Approach | Reason |
|-----------|----------|--------|
| Public API function | Defensive NULL checks | Callers might pass invalid data |
| Internal helper function | `assert()` | NULL here indicates a programmer error |
| After `malloc` | Check return value | System might be out of memory |
| After function that might fail | Check for NULL | Handle failure gracefully |

#### Complete Example

```c
// game.c
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

GameState *game_create(void) {
    GameState *state = malloc(sizeof(GameState));
    if (state == NULL) {
        fprintf(stderr, "Error: Failed to allocate game state\n");
        return NULL;
    }
    initialize_board(state);
    return state;
}

void game_reset(GameState *state) {
    // Public API - defensive check
    if (state == NULL) {
        fprintf(stderr, "Warning: game_reset called with NULL pointer\n");
        return;
    }
    initialize_board(state);
}

static void initialize_board(GameState *state) {
    // Internal function - assert
    assert(state != NULL);
    state->status = ONGOING;
    for (int i = 0; i < 9; i++) {
        state->board[i] = CELL_EMPTY;
    }
}

int main(void) {
    GameState *state = game_create();
    if (state == NULL) {
        return EXIT_FAILURE;
    }
    
    game_reset(state);
    
    free(state);
    state = NULL;  // Prevent use-after-free
    
    return EXIT_SUCCESS;
}
```

#### Debugging NULL Pointer Crashes

**1. Using GDB:**
```bash
gcc -g program.c -o program
gdb ./program

(gdb) run
(gdb) backtrace     # Shows call stack
(gdb) print state   # Shows pointer value
```

**2. Enable Core Dumps:**
```bash
ulimit -c unlimited
./program
gdb ./program core
(gdb) backtrace
```

**3. Use Address Sanitizer:**
```bash
gcc -fsanitize=address -g program.c -o program
./program

# Will show:
# ERROR: AddressSanitizer: SEGV on unknown address 0x000000000000
```

#### Best Practices

1. **Always initialize pointers:**
   ```c
   GameState *state = NULL;  // Not: GameState *state;
   ```

2. **Check malloc returns:**
   ```c
   void *ptr = malloc(size);
   if (ptr == NULL) { /* handle error */ }
   ```

3. **Set to NULL after free:**
   ```c
   free(state);
   state = NULL;  // Prevents double-free and use-after-free
   ```

4. **Use static analysis:**
   ```bash
   clang --analyze program.c  // Finds potential NULL dereferences
   ```

#### The "Billion Dollar Mistake"

Tony Hoare, who invented NULL references, called it his "billion-dollar mistake":

> "I call it my billion-dollar mistake. It was the invention of the null reference in 1965... This has led to innumerable errors, vulnerabilities, and system crashes, which have probably caused a billion dollars of pain and damage in the last forty years."

In C, NULL pointers are especially dangerous because there's no built-in protection. Always check before dereferencing!

---

### 14. `memset` with Non-Zero Values

**Severity:** 🟡 Medium - Incorrect Behavior  
**Frequency:** Occasional mistake

#### The Problem

```c
int arr[10];
memset(arr, 5, sizeof(arr));  // Does NOT set each int to 5!

printf("%d\n", arr[0]);  // Prints 84215045 (0x05050505), not 5!
```

#### Why This Happens

`memset` operates on **bytes**, not on array elements:

```c
void *memset(void *ptr, int value, size_t num);
//                      ^^^^^^^^^^
//           Sets each of 'num' BYTES to the low 8 bits of 'value'
```

**What actually happens with `memset(arr, 5, sizeof(arr))`:**
```
Each int is 4 bytes wide.
memset sets every byte to 0x05:

Byte view:  [05][05][05][05] [05][05][05][05] ...
Int view:   [ 0x05050505  ] [ 0x05050505  ] ...
            = 84215045        = 84215045
```

#### When `memset` Works Correctly

**Setting to `0` — always safe:**
```c
int arr[10];
memset(arr, 0, sizeof(arr));  // All ints are 0 ✅
// Works because every byte being 0x00 means every int is 0
```

**Setting to `-1` — safe on two's complement systems:**
```c
int arr[10];
memset(arr, -1, sizeof(arr));  // All ints are -1 ✅
// memset fills each byte with 0xFF
// 0xFFFFFFFF == -1 in two's complement (all modern hardware)
```

**Setting individual bytes in a `char`/`uint8_t` array:**
```c
unsigned char buffer[100];
memset(buffer, 0xAB, sizeof(buffer));  // Each byte is 0xAB ✅
```

**What does NOT work:**
```c
int arr[10];
memset(arr, 1, sizeof(arr));   // Each int is 0x01010101 (16843009), not 1 ❌
memset(arr, 42, sizeof(arr));  // Each int is 0x2A2A2A2A (707406378), not 42 ❌

double values[10];
memset(values, 1, sizeof(values));  // Not 1.0 — corrupted floating point ❌

void *ptrs[10];
memset(ptrs, 1, sizeof(ptrs));  // Not valid pointers ❌
```

#### The Solution

**For non-zero values, use a loop:**

```c
// ✅ Initialize ints to 5
int arr[10];
for (int i = 0; i < 10; i++) {
    arr[i] = 5;
}

// ✅ Use an initializer (when size is known at compile time)
int arr[10] = {5, 5, 5, 5, 5, 5, 5, 5, 5, 5};

// ✅ Use a helper function for reusable code
void fill_int_array(int *array, size_t count, int value) {
    for (size_t i = 0; i < count; i++) {
        array[i] = value;
    }
}

fill_int_array(arr, 10, 5);
```

#### Real-World Example from Tic-Tac-Toe

```c
// ❌ WRONG if CELL_EMPTY is not 0
CellValue board[9];
memset(board, CELL_EMPTY, sizeof(board));  // Only safe if CELL_EMPTY == 0

// ✅ CORRECT — document the assumption
typedef enum {
    CELL_EMPTY = 0,  // MUST stay 0 for memset to work
    CELL_X = 1,
    CELL_O = 2
} CellValue;

memset(board, CELL_EMPTY, sizeof(board));  // OK — CELL_EMPTY is 0

// ✅ SAFEST — works for any value of CELL_EMPTY
void board_init(CellValue board[9]) {
    for (int i = 0; i < 9; i++) {
        board[i] = CELL_EMPTY;
    }
}
```

#### Good Uses of `memset`

```c
// Zero-initialize a struct before use
GameState state;
memset(&state, 0, sizeof(state));

// Clear a buffer
char buffer[1024];
memset(buffer, 0, sizeof(buffer));

// Fill a byte array with a pattern
unsigned char data[256];
memset(data, 0xFF, sizeof(data));

// Securely wipe sensitive data from memory
char password[100];
// ... use password ...
memset(password, 0, sizeof(password));  // Clear before buffer goes out of scope
```

#### Summary

| Use case | Safe? | Preferred alternative |
|----------|-------|-----------------------|
| Zero all bytes | ✅ Yes | `= {0}` initializer |
| Set all bytes to -1 | ✅ Yes (two's complement) | Loop |
| Set ints to any other value | ❌ No | Loop or initializer |
| Set floats/doubles | ❌ No | Loop |
| Set pointers to NULL | ❌ No (use `= {0}` or loop) | Loop or `= {0}` |
| Fill `char`/`uint8_t` array | ✅ Yes | `memset` |

#### Best Practices

1. **Reserve `memset` for zeroing memory:**
   ```c
   memset(ptr, 0, size);  // Safe and idiomatic
   ```

2. **Prefer initializers for zero-initialization:**
   ```c
   int arr[5] = {0};       // Cleaner than memset for declarations
   GameState s = {0};      // Zero-initialize struct at declaration
   ```

3. **Use loops for non-zero values:**
   ```c
   for (size_t i = 0; i < count; i++) {
       array[i] = value;
   }
   ```

4. **Document assumptions when using `memset` with enums:**
   ```c
   // Requires CELL_EMPTY == 0 — do not change the enum ordering
   memset(board, CELL_EMPTY, sizeof(board));
   ```


