# Pitfalls: Variables & Types

> Part of the [C Pitfalls](README.md) series.

Errors in this category arise from variables that carry garbage values or from silent numeric conversions that truncate or reinterpret data unexpectedly.

## Pitfalls in This Section

- [7. Uninitialized Variables](#7-uninitialized-variables)
- [8. Silent Integer Promotion](#8-silent-integer-promotion)

---

### 7. Uninitialized Variables

**Severity:** 🔴 Critical - Undefined Behavior  
**Frequency:** Common for beginners

#### The Problem

```c
int best_move;  // Contains garbage value!
// ... some conditional logic ...
if (game_over) {
    best_move = 4;
}
// If game is not over, best_move is never set
return best_move;  // Could be anything: 0, 42, -999999, etc.
```

#### Why This is Dangerous

In C, local variables are **not automatically initialized**. They contain whatever bytes happened to be in that memory location:

```c
void function(void) {
    int x;  // Might contain: 0, 42, -1234567, or any value
    printf("%d\n", x);  // UNDEFINED BEHAVIOR!
}
```

**What happens:**
1. **Stack memory is reused:** Variables get memory from the stack
2. **No automatic zeroing:** Unlike Java/Python, C doesn't initialize to 0
3. **Previous function data:** You get leftover data from previous function calls
4. **Nondeterministic bugs:** May work on your machine, fail on others

#### Real-World Example

```c
int ai_get_move(GameState *state) {
    int best_move;  // Uninitialized!
    int best_score;
    
    for (int i = 0; i < 9; i++) {
        if (board_is_empty(state, i)) {
            int score = evaluate_move(state, i);
            if (score > best_score) {  // best_score is garbage!
                best_score = score;
                best_move = i;
            }
        }
    }
    
    return best_move;  // May never be set if no moves evaluated correctly
}
```

#### The Solution

**Always initialize variables when declared:**

```c
// ✅ Initialize with meaningful value
int best_move = -1;      // Sentinel: "no move found"
int best_score = INT_MIN; // Worst possible score

// ✅ Initialize to zero
int counter = 0;
float average = 0.0f;
char *name = NULL;

// ✅ Initialize from function
int position = get_user_input();
```

#### Different Initialization Strategies

**1. Sentinel Values:**
```c
int position = -1;     // -1 means "not set" or "invalid"
char *filename = NULL; // NULL means "no file"
```

**2. Zero Initialization:**
```c
int scores[10] = {0};   // All elements set to 0
GameState state = {0};  // All struct members set to 0
```

**3. Meaningful Defaults:**
```c
int player = PLAYER_X;    // Default player
int difficulty = MEDIUM;  // Default difficulty
bool game_over = false;   // Game starts not over
```

**4. Computed Values:**
```c
int center_x = window_width / 2;
time_t start_time = time(NULL);
```

#### Special Cases

**Arrays:**
```c
// ❌ WRONG: Partially initialized - remaining elements are 0, but intent is unclear
int scores[5] = {1, 2};

// ✅ CLEAR: Explicitly initialize all
int scores[5] = {1, 2, 0, 0, 0};
// or
int scores[5] = {0};  // All zeros
```

**Structs:**
```c
// ❌ Uninitialized struct
GameState state;
state.status = ONGOING;  // Other fields are garbage!

// ✅ Zero-initialize entire struct
GameState state = {0};
state.status = ONGOING;

// ✅ Or use designated initializers (C99+)
GameState state = {
    .status = ONGOING,
    .turn = PLAYER_X,
    .board = {0}
};
```

**Static and Global Variables (auto-initialized):**
```c
// These ARE automatically initialized to 0
static int call_count;  // = 0
int global_value;       // = 0

void function(void) {
    static int counter;  // = 0 (initialized once, retains value between calls)
    counter++;
}
```

#### Detection Tools

**1. Compiler Warnings:**
```bash
gcc -Wall -Wextra -Wuninitialized program.c

# Warning: 'best_move' may be used uninitialized
```

**2. Static Analysis:**
```bash
clang --analyze program.c
cppcheck --enable=all program.c
```

**3. Valgrind:**
```bash
valgrind --track-origins=yes ./program

# Conditional jump or move depends on uninitialized value(s)
```

**4. Memory Sanitizer:**
```bash
gcc -fsanitize=memory -g program.c
./program

# WARNING: MemorySanitizer: use-of-uninitialized-value
```

#### Best Practices

1. **Initialize at declaration:**
   ```c
   int count = 0;  // Not: int count; count = 0;
   ```

2. **Treat warnings as errors:**
   ```bash
   gcc -Wall -Wextra -Werror=uninitialized ...
   ```

3. **Document initialization requirements:**
   ```c
   /**
    * Note: Caller must initialize state before calling
    */
   void process_state(GameState *state);
   ```

4. **Consider zero-initialization for complex structs:**
   ```c
   GameState state = {0};  // Safe baseline before setting specific fields
   ```

---

### 8. Silent Integer Promotion

**Severity:** 🟡 Medium - Subtle Bugs  
**Frequency:** Less common but confusing

#### The Problem

```c
unsigned char a = 200;
unsigned char b = 100;
unsigned char result = a + b;  // What is result?

printf("%d\n", result);  // Prints 44, not 300!
```

#### Why This Happens

C performs **integer promotion** on small types before arithmetic:

1. **Operands promoted to `int`:** `char`, `short`, `_Bool` become `int`
2. **Arithmetic performed:** Operation done at `int` size
3. **Result converted back:** Truncated to destination type
4. **Overflow in conversion:** Data loss occurs

```c
unsigned char a = 200;
unsigned char b = 100;

// Addition:
// 1. Promote: (int)a + (int)b = 200 + 100 = 300
// 2. Assign back to unsigned char: 300 % 256 = 44
unsigned char result = a + b;  // result = 44
```

> **Note:** The behavior of assigning 200 to a `signed char` (range -128 to 127) is implementation-defined. On most systems using two's complement, 200 becomes -56. Use `unsigned char` or larger types to avoid ambiguity.

#### Detailed Example

```c
#include <stdio.h>
#include <limits.h>

int main(void) {
    printf("CHAR_MIN: %d, CHAR_MAX: %d\n", CHAR_MIN, CHAR_MAX);
    // Typical output: CHAR_MIN: -128, CHAR_MAX: 127
    
    unsigned char a = 200;
    unsigned char b = 100;
    
    int sum = a + b;              // 300 - OK, fits in int
    printf("sum (int): %d\n", sum);  // 300
    
    unsigned char result = a + b; // 300 -> 44 (300 % 256)
    printf("result (unsigned char): %d\n", result);  // 44
    
    return 0;
}
```

#### Common Scenarios

**Scenario 1: Signed vs Unsigned Comparison**
```c
signed char x = -1;
unsigned char y = 1;

if (x < y) {  // -1 < 1 is true (both promoted to int)
    printf("Correct\n");
}

// But:
unsigned char z = (unsigned char)x;  // z = 255 (wrap-around)
```

**Scenario 2: Bitwise Operations**
```c
unsigned char flags = 0xFF;
unsigned char mask = 0x0F;

// Promoted to int before ~
unsigned char result = ~flags & mask;  // May not work as expected!

// Better: explicitly cast
unsigned char result = (unsigned char)(~flags) & mask;
```

**Scenario 3: Negative Index**
```c
char index = -1;  // Might be used as a sentinel

int array[10];
array[index];  // Undefined behavior! Negative index after promotion
```

#### The Solution

**1. Use appropriate types for the value range:**
```c
int count = 0;       // Not: char count = 0;
size_t length = 0;   // For array sizes and counts

uint8_t pixel_value = 255;   // Explicit unsigned 8-bit
int16_t temperature = -40;   // Explicit signed 16-bit
```

**2. Be explicit with conversions:**
```c
unsigned char a = 200;
unsigned char b = 100;

int result = (int)a + (int)b;  // Explicit promotion
if (result > 255) {
    // Handle overflow
}
```

**3. Use fixed-width types (C99+):**
```c
#include <stdint.h>

uint8_t byte1 = 200;
uint8_t byte2 = 100;
uint16_t sum = byte1 + byte2;  // Guaranteed to fit
```

#### Best Practices

1. **Prefer `int` for arithmetic variables:**
   ```c
   int x = 0;  // Not: char x = 0;
   ```

2. **Use sized types for protocol/data storage:**
   ```c
   uint8_t pixel_data[1024];  // Network protocol, image data
   int32_t coordinate;         // Graphics, large values
   ```

3. **Check limits before narrowing:**
   ```c
   #include <limits.h>
   
   if (value > UCHAR_MAX) {
       // Handle overflow before truncating
   }
   ```

4. **Document expected ranges:**
   ```c
   /**
    * @param brightness Value from 0-255
    */
   void set_brightness(uint8_t brightness);
   ```

