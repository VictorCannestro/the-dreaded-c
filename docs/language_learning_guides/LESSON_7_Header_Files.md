# Lesson 7: Header Files
**Learning Objective:** Understand how to organize C code across multiple files using headers and source files, implement proper include guards, and follow best practices for modular code design.

C code is split into two types of files:
- **Header files (`.h`)** - Declarations ("what exists")
- **Source files (`.c`)** - Definitions ("how it works")

---

## Table of Contents
### Core Concepts
**Foundations**
1. [Why Two File Types?](#why-two-file-types) — interface vs implementation; compilation speed
2. [The Compilation Model](#the-compilation-model) — preprocessor, compiler, linker stages
3. [Header File Example (`.h`)](#header-file-example-h) — building a module progressively
4. [Source File Example (`.c`)](#source-file-example-c) — implementing declarations; `static` helpers

**Essential Concepts**
5. [Include Guards: Preventing Double Inclusion](#include-guards-preventing-double-inclusion) — `#ifndef`/`#define`/`#endif` pattern
6. [Include Order Convention](#include-order-convention) — own header first; catching missing dependencies
7. [What Goes Where?](#what-goes-where) — declarations in headers, definitions in sources

**Avoiding Mistakes**
8. [Common Pitfalls](#common-pitfalls) — wrong include style; missing updates; including `.c` files

**Putting It Together**
9. [Real-World Example: Building a Math Library](#real-world-example-building-a-math-library) — complete module from scratch

### Practice
10. [Self-Check Questions](#self-check-questions) — quick knowledge verification
11. [Practical Exercises](#practical-exercises) — spot errors, create modules, design headers
12. [Bonus Challenge: Apply to the TicTacToe Project](#bonus-challenge-apply-to-the-tictactoe-project) — explore the real codebase

### Going Further
13. [Summary](#summary) — key takeaways and mental models
14. [Next Steps](#next-steps) — continue to Lesson 8
15. [Bonus: Advanced Topics](#bonus-advanced-topics) — *(return after Lessons 8–9)*
    - `#pragma once` — modern include guard alternative
    - `static inline` Functions — when implementations belong in headers
    - Forward Declarations — solving circular dependencies
    - Conditional Compilation — `#ifdef`, `#if`, platform-specific code

---

## Why Two File Types?
Imagine you're building a car. You need:
- **A manual** (header file) - tells you what the car can do: "Press pedal to accelerate"
- **The engine** (source file) - the actual implementation of how acceleration works

In C, this separation serves multiple purposes:

### 1. **Interface vs. Implementation**
```c
// You need to know this to USE a function:
int calculate_winner(CellValue board[9]);  // Declaration

// You DON'T need to know this to use it:
int calculate_winner(CellValue board[9]) {   // Definition
    // 50 lines of complex logic...
}
```

### 2. **Compilation Speed**
When you change the implementation (`.c` file), only that file needs to be recompiled. Files that include the header don't need recompilation if the interface hasn't changed.

### 3. **Information Hiding**
Headers expose only what users need to know. Internal helper functions stay private in the `.c` file.

### 4. **Code Reuse**
Multiple source files can include the same header to use its functionality.

---

## The Compilation Model
Understanding how C compiles multi-file projects is crucial:

```
┌─────────────┐     ┌─────────────┐     ┌─────────────┐
│  main.c     │     │  board.c    │     │   ai.c      │
│             │     │             │     │             │
│ #include    │     │ #include    │     │ #include    │
│ "board.h"   │     │ "board.h"   │     │ "ai.h"      │
│ #include    │     │ #include    │     │ #include    │
│ "ai.h"      │     │ <stdio.h>   │     │ "board.h"   │
└──────┬──────┘     └──────┬──────┘     └──────┬──────┘
       │                   │                   │
       ▼                   ▼                   ▼
  Preprocessor        Preprocessor        Preprocessor
  (text expansion)    (text expansion)    (text expansion)
       │                   │                   │
       ▼                   ▼                   ▼
    Compiler            Compiler            Compiler
       │                   │                   │
       ▼                   ▼                   ▼
   main.o              board.o              ai.o
   (object file)       (object file)        (object file)
       │                   │                   │
       └───────────┬───────┴───────────────────┘
                   ▼
               Linker
                   │
                   ▼
            Executable
            (program)
```

**Key Insight:** Each `.c` file is compiled independently. The preprocessor copies header contents into each `.c` file before compilation.

---

## Header File Example (`.h`)
Let's build a simple calculator module progressively.

### Version 1: Minimal Header
```c
// calculator.h - Basic version

#ifndef CALCULATOR_H
#define CALCULATOR_H

int add(int a, int b);
int subtract(int a, int b);

#endif
```

**What's happening:**
- `#ifndef` / `#define` / `#endif` is an **include guard** (explained later)
- We only **declare** functions (no implementation)
- Semicolons end declarations, no curly braces

### Version 2: Adding Types
```c
// calculator.h - With custom types

#ifndef CALCULATOR_H
#define CALCULATOR_H

// Type definitions belong in headers
typedef enum {
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_DIVIDE
} Operation;

typedef struct {
    int result;
    int error_code;  // 0 = success, 1 = division by zero
} CalcResult;

// Function declarations using our types
CalcResult calculate(Operation op, int a, int b);
const char* operation_name(Operation op);

#endif
```

### Version 3: Documentation and Constants
```c
// calculator.h - Production quality

#ifndef CALCULATOR_H
#define CALCULATOR_H

// ============================================================================
// Constants
// ============================================================================

#define CALC_SUCCESS 0
#define CALC_ERROR_DIV_ZERO 1
#define CALC_ERROR_OVERFLOW 2

// ============================================================================
// Types
// ============================================================================

/**
 * Supported arithmetic operations
 */
typedef enum {
    OP_ADD,       // Addition: a + b
    OP_SUBTRACT,  // Subtraction: a - b
    OP_MULTIPLY,  // Multiplication: a * b
    OP_DIVIDE     // Integer division: a / b
} Operation;

/**
 * Result of a calculation with error handling
 */
typedef struct {
    int result;       // The calculated value (valid if error_code == 0)
    int error_code;   // CALC_SUCCESS or CALC_ERROR_*
} CalcResult;

// ============================================================================
// Public Functions
// ============================================================================

/**
 * Perform an arithmetic operation
 * 
 * @param op The operation to perform
 * @param a First operand
 * @param b Second operand
 * @return CalcResult with result and error_code
 */
CalcResult calculate(Operation op, int a, int b);

/**
 * Get human-readable name of an operation
 * 
 * @param op The operation
 * @return String like "addition" or "division"
 */
const char* operation_name(Operation op);

/**
 * Check if a result indicates success
 * 
 * @param result The CalcResult to check
 * @return 1 if successful, 0 if error
 */
int calc_is_success(CalcResult result);

#endif // CALCULATOR_H
```

**Best Practices Shown:**
- ✅ Documentation comments for public API
- ✅ Logical grouping with comment sections
- ✅ Named constants instead of magic numbers
- ✅ Consistent naming conventions

---

## Source File Example (`.c`)
Now let's implement what we declared:

```c
// calculator.c - Implementation

#include "calculator.h"  // Our own header FIRST

#include <limits.h>      // Standard headers after (for INT_MAX, INT_MIN)

// ============================================================================
// Private Helper Functions (not in header!)
// ============================================================================

/**
 * Check if multiplication would overflow
 * This is a "static" function - only visible in this file
 */
static int would_overflow_multiply(int a, int b) {
    if (a == 0 || b == 0) return 0;
    if (a > 0) {
        if (b > 0 && a > INT_MAX / b) return 1;
        if (b < 0 && b < INT_MIN / a) return 1;
    } else {
        if (b > 0 && a < INT_MIN / b) return 1;
        if (b < 0 && a < INT_MAX / b) return 1;  // Both negative
    }
    return 0;
}

// ============================================================================
// Public Function Implementations
// ============================================================================

CalcResult calculate(Operation op, int a, int b) {
    CalcResult result = {0, CALC_SUCCESS};
    
    switch (op) {
        case OP_ADD:
            result.result = a + b;
            break;
            
        case OP_SUBTRACT:
            result.result = a - b;
            break;
            
        case OP_MULTIPLY:
            if (would_overflow_multiply(a, b)) {
                result.error_code = CALC_ERROR_OVERFLOW;
            } else {
                result.result = a * b;
            }
            break;
            
        case OP_DIVIDE:
            if (b == 0) {
                result.error_code = CALC_ERROR_DIV_ZERO;
            } else {
                result.result = a / b;
            }
            break;
    }
    
    return result;
}

const char* operation_name(Operation op) {
    // Static array lives for entire program duration
    static const char* names[] = {
        "addition",
        "subtraction",
        "multiplication",
        "division"
    };
    
    if (op >= OP_ADD && op <= OP_DIVIDE) {
        return names[op];
    }
    return "unknown";
}

int calc_is_success(CalcResult result) {
    return result.error_code == CALC_SUCCESS;
}
```

**Key Points:**
- `static` functions are **private** to this file
- We can have helper functions that users never see
- The header is our "contract" - we implement exactly what we declared

---

## Include Guards: Preventing Double Inclusion

### The Problem

Without guards, this happens:

```c
// shapes.h (NO GUARD - WRONG!)
typedef struct {
    int x, y;
} Point;

// graphics.h
#include "shapes.h"

// main.c
#include "shapes.h"
#include "graphics.h"

// After preprocessing, main.c effectively contains:
typedef struct { int x, y; } Point;  // From shapes.h
typedef struct { int x, y; } Point;  // From shapes.h via graphics.h
// ERROR: Point redefined!
```

### The Solution: Include Guards

```c
// shapes.h (CORRECT)
#ifndef SHAPES_H       // "If SHAPES_H is not defined..."
#define SHAPES_H       // "...define it now (first inclusion)"

typedef struct {
    int x, y;
} Point;

#endif // SHAPES_H     // "End of guarded section"

// On second inclusion:
// - SHAPES_H is already defined
// - #ifndef fails, entire content skipped
// - No redefinition error!
```

### Include Guard Naming Convention

```c
// board.h
#ifndef BOARD_H
#define BOARD_H
// ...
#endif

// win_condition_calculator.h
#ifndef WIN_CONDITION_CALCULATOR_H
#define WIN_CONDITION_CALCULATOR_H
// ...
#endif

// For headers in subdirectories:
// ai/minimax.h
#ifndef AI_MINIMAX_H
#define AI_MINIMAX_H
// ...
#endif
```

**Rules:**
- Use the filename in UPPERCASE
- Replace `.` and `/` with `_`
- End with `_H`
- Must be unique across entire project

> 💡 **Want to learn about `#pragma once`?** See [More Advanced Topics](#more-advanced-topics) after completing this lesson.

---

## Include Order Convention

### The Rule

```c
// In any .c file:

#include "own_header.h"        // 1. Own header first (if exists)

#include "project_header_1.h"  // 2. Project headers
#include "project_header_2.h"
#include "project_header_3.h"

#include <stdlib.h>            // 3. Standard library headers
#include <stdio.h>
#include <string.h>
```

### Why Own Header First?

This catches missing dependencies in your header:

```c
// board.h - BAD (missing includes)
typedef struct {
    CellValue cells[9];  // Uses CellValue but doesn't include constants.h!
} Board;

// board.c
#include "board.h"       // ERROR! CellValue undefined
```

If you put standard headers first, they might accidentally define something that makes your header work:

```c
// board.c - WRONG ORDER
#include <stdlib.h>      // Happens to define size_t
#include "board.h"       // Works by accident!

// But in main.c:
#include "board.h"       // ERROR! size_t undefined
```

**Correct approach:**

```c
// board.h - GOOD
#include "constants.h"   // Declare dependencies

typedef struct {
    CellValue cells[9];
} Board;

// board.c - GOOD
#include "board.h"       // Own header first
#include <stdlib.h>      // Other includes after
```

### Alphabetical Ordering

Within each group, sort alphabetically for consistency:

```c
#include "tictactoe.h"

#include "ai.h"
#include "board.h"
#include "ui_interface.h"
#include "win_condition_calculator.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
```

---

## What Goes Where?

### Header Files (`.h`) - The Public Interface

**DO Include:**
```c
// ✅ Type definitions
typedef struct { /* ... */ } GameState;
typedef enum { X, O, EMPTY } CellValue;

// ✅ Function declarations
void game_init(GameState* state);
int game_make_move(GameState* state, int pos);

// ✅ Macro definitions
#define BOARD_SIZE 9
#define MAX_PLAYERS 2

// ✅ Inline functions (small, performance-critical)
static inline int is_valid_position(int pos) {
    return pos >= 0 && pos < 9;
}

// ✅ External variable declarations (rarely needed)
extern int global_game_count;
```

**DON'T Include:**
```c
// ❌ Function implementations (except inline)
void game_init(GameState* state) {  // NO!
    // ...
}

// ❌ Global variable definitions
int global_game_count = 0;  // NO! (declaration with 'extern' only)

// ❌ Static variables (internal state)
static int internal_counter = 0;  // NO!
```

### Source Files (`.c`) - The Implementation

**DO Include:**
```c
// ✅ Function implementations
void game_init(GameState* state) {
    // Full implementation
}

// ✅ Static helper functions (private to this file)
static int calculate_score(int wins, int losses) {
    return wins - losses;
}

// ✅ Static variables (file-scope state)
static int games_played = 0;

// ✅ Global variable definitions
int global_game_count = 0;  // Defined here, declared in .h with 'extern'
```

### Quick Reference Table

| Item | Header (`.h`) | Source (`.c`) |
|------|---------------|---------------|
| `typedef struct` | ✅ | ❌ |
| `typedef enum` | ✅ | ❌ |
| `#define` constants | ✅ | ❌ (unless file-private) |
| Function declarations | ✅ | ❌ |
| Function implementations | ❌ | ✅ |
| `static` helper functions | ❌ | ✅ |
| `inline` functions | ✅ | ❌ |
| Global variables | `extern` declaration | Definition |
| `static` variables | ❌ | ✅ |

---

## Common Pitfalls

### Pitfall 1: Implementing Functions in Headers

```c
// calculator.h - WRONG!
#ifndef CALCULATOR_H
#define CALCULATOR_H

int add(int a, int b) {
    return a + b;
}

#endif
```

**Why it's wrong:** If multiple `.c` files include this header, you'll get "multiple definition" errors during linking.

> 💡 **Exception:** `static inline` functions *can* be defined in headers. See [More Advanced Topics](#more-advanced-topics) for when this is appropriate.

### Pitfall 2: Circular Dependencies

```c
// game.h
#include "player.h"
typedef struct Game {
    Player* players;
} Game;

// player.h  
#include "game.h"
typedef struct Player {
    Game* current_game;
} Player;
```

**Problem:** Each header needs the other, creating an infinite loop during preprocessing!

**Solution:** Use *forward declarations* — see [More Advanced Topics](#more-advanced-topics) for the full technique.

### Pitfall 3: Forgetting to Update Header

```c
// board.h - Declaration
void board_clear(int board[9]);

// board.c - Changed implementation
void board_clear(CellValue board[9]) {  // Type changed!
    // ...
}

// main.c
#include "board.h"
int my_board[9];
board_clear(my_board);  // Compiles, but WRONG type passed!
```

**Rule:** Always update header when you change function signature.

### Pitfall 4: Including `.c` Files

```c
// main.c - WRONG!
#include "board.c"  // Never do this!
```

**Why:** You'll get multiple definitions. `.c` files should only be compiled, not included.

### Pitfall 5: Using Wrong Include Style

```c
#include <my_project_header.h>  // WRONG - angle brackets for project files
#include "stdio.h"              // WRONG - quotes for standard library
```

**Correct:**
```c
#include "my_project_header.h"  // Quotes for project files
#include <stdio.h>              // Angle brackets for system/standard library
```

**Why:** 
- `<>` searches system include paths
- `""` searches current directory first, then system paths

---

## Real-World Example: Building a Math Library

Let's build a complete, small module from scratch.

### Step 1: Design the Interface

```c
// math_utils.h
#ifndef MATH_UTILS_H
#define MATH_UTILS_H

/**
 * Calculate factorial of n
 * @param n Non-negative integer
 * @return n! or -1 if n is negative
 */
int factorial(int n);

/**
 * Calculate greatest common divisor
 * @param a First number
 * @param b Second number
 * @return GCD of a and b
 */
int gcd(int a, int b);

/**
 * Check if a number is prime
 * @param n Number to check
 * @return 1 if prime, 0 otherwise
 */
int is_prime(int n);

#endif
```

### Step 2: Implement the Functions

```c
// math_utils.c
#include "math_utils.h"

// Private helper (not in header!)
static int abs_value(int n) {
    return n < 0 ? -n : n;
}

int factorial(int n) {
    if (n < 0) return -1;
    if (n == 0 || n == 1) return 1;
    
    int result = 1;
    for (int i = 2; i <= n; i++) {
        result *= i;
    }
    return result;
}

int gcd(int a, int b) {
    a = abs_value(a);
    b = abs_value(b);
    
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int is_prime(int n) {
    if (n < 2) return 0;
    if (n == 2) return 1;
    if (n % 2 == 0) return 0;
    
    for (int i = 3; i * i <= n; i += 2) {
        if (n % i == 0) return 0;
    }
    return 1;
}
```

### Step 3: Use the Library

```c
// main.c
#include "math_utils.h"
#include <stdio.h>

int main(void) {
    printf("Factorial of 5: %d\n", factorial(5));
    printf("GCD of 48 and 18: %d\n", gcd(48, 18));
    printf("Is 17 prime? %s\n", is_prime(17) ? "Yes" : "No");
    
    return 0;
}
```

### Step 4: Compile

```bash
gcc -c math_utils.c -o math_utils.o
gcc -c main.c -o main.o
gcc math_utils.o main.o -o program
./program
```

---

## Self-Check Questions

Before moving to the exercises, test your understanding with these quick questions:

**Q1: What is the primary purpose of a header file?**

<details>
<summary>Click to reveal answer</summary>

To declare the **public interface** of a module - the types, constants, and function signatures that other files need to use it. Headers tell other code *what exists* without revealing *how it works*.

</details>

**Q2: Why do we use include guards?**

<details>
<summary>Click to reveal answer</summary>

To prevent the same header from being included multiple times in a single compilation unit, which would cause "redefinition" errors. The guard ensures the header content is only processed once.

</details>

**Q3: What's wrong with this code?**
```c
// math.h
int square(int x) {
    return x * x;
}
```

<details>
<summary>Click to reveal answer</summary>

Function **implementations** should not be in header files! If multiple `.c` files include this header, the linker will report "multiple definition of `square`" errors. 

**Fix:** Either move the implementation to a `.c` file, or mark it `static inline`:
```c
static inline int square(int x) {
    return x * x;
}
```

</details>

**Q4: What's the difference between `#include <stdio.h>` and `#include "myheader.h"`?**

<details>
<summary>Click to reveal answer</summary>

- `<angle brackets>` - Search system/standard library include paths first
- `"quotes"` - Search the current directory first, then system paths

Use `<>` for standard library headers, `""` for your project's headers.

</details>

**Q5: Why should you include your own header first in a `.c` file?**

<details>
<summary>Click to reveal answer</summary>

To catch missing dependencies in your header. If your header uses a type but forgets to include its definition, you'll get an error immediately. If you include other headers first, they might accidentally provide the missing definition, hiding the bug.

</details>

---

## Practical Exercises

### Exercise 1: Spot the Errors

The following header file has **3 errors**. Can you find them all?

```c
// string_utils.h
#ifndef STRING_UTIL_H
#define STRING_UTIL_H

typedef enum {
    LOWERCASE,
    UPPERCASE,
    TITLECASE
} TextCase;

// Convert string to specified case
void convert_case(char* str, TextCase target_case) {
    // Implementation here
}

// Count words in a string
int count_words(char* str);

#endif
```

<details>
<summary><strong>Click to reveal answer</strong></summary>

**Errors found:**

1. **Inconsistent guard name:** Guard is `STRING_UTIL_H` but filename is `string_utils.h` - should be `STRING_UTILS_H` (matching filename exactly)

2. **Function implementation in header:** `convert_case()` has a body - implementations should only be in `.c` files (unless it's `static inline`)

3. **Missing `const` qualifier:** `count_words` takes `char* str` but doesn't modify the string, so it should be `const char* str` for const-correctness

**Corrected version:**
```c
// string_utils.h
#ifndef STRING_UTILS_H
#define STRING_UTILS_H

typedef enum {
    LOWERCASE,
    UPPERCASE,
    TITLECASE
} TextCase;

// Convert string to specified case (modifies in place)
void convert_case(char* str, TextCase target_case);

// Count words in a string
int count_words(const char* str);

#endif

```

</details>

---

### Exercise 2: Create a Stack Module

Create header and source files for a simple integer stack with these operations:
- `stack_push(int value)` - Add value to stack
- `stack_pop()` - Remove and return top value
- `stack_peek()` - Return top value without removing
- `stack_is_empty()` - Check if stack is empty
- `stack_clear()` - Clear all values

Requirements:
- Fixed size stack (maximum 100 elements)
- Handle error conditions appropriately
- Include proper documentation

<details>
<summary><strong>Click to reveal answer</strong></summary>

```c
// stack.h
#ifndef STACK_H
#define STACK_H

#define STACK_MAX_SIZE 100
#define STACK_ERROR -999999  // Sentinel value for errors

/**
 * Push a value onto the stack
 * @param value The integer to push
 * @return 1 on success, 0 if stack is full
 */
int stack_push(int value);

/**
 * Pop a value from the stack
 * @return The popped value, or STACK_ERROR if empty
 */
int stack_pop(void);

/**
 * Peek at the top value without removing it
 * @return The top value, or STACK_ERROR if empty
 */
int stack_peek(void);

/**
 * Check if stack is empty
 * @return 1 if empty, 0 otherwise
 */
int stack_is_empty(void);

/**
 * Check if stack is full
 * @return 1 if full, 0 otherwise
 */
int stack_is_full(void);

/**
 * Clear all values from the stack
 */
void stack_clear(void);

/**
 * Get current number of elements in stack
 * @return Number of elements
 */
int stack_size(void);

#endif
```

```c
// stack.c
#include "stack.h"

// Private state (not visible outside this file)
static int stack_data[STACK_MAX_SIZE];
static int stack_top = 0;  // Index of next free slot

int stack_push(int value) {
    if (stack_top >= STACK_MAX_SIZE) {
        return 0;  // Stack full
    }
    stack_data[stack_top++] = value;
    return 1;
}

int stack_pop(void) {
    if (stack_top == 0) {
        return STACK_ERROR;  // Stack empty
    }
    return stack_data[--stack_top];
}

int stack_peek(void) {
    if (stack_top == 0) {
        return STACK_ERROR;
    }
    return stack_data[stack_top - 1];
}

int stack_is_empty(void) {
    return stack_top == 0;
}

int stack_is_full(void) {
    return stack_top >= STACK_MAX_SIZE;
}

void stack_clear(void) {
    stack_top = 0;
}

int stack_size(void) {
    return stack_top;
}
```

**Usage example:**
```c
// main.c
#include "stack.h"
#include <stdio.h>

int main(void) {
    stack_push(10);
    stack_push(20);
    stack_push(30);
    
    printf("Top: %d\n", stack_peek());      // 30
    printf("Size: %d\n", stack_size());     // 3
    printf("Pop: %d\n", stack_pop());       // 30
    printf("Pop: %d\n", stack_pop());       // 20
    printf("Size: %d\n", stack_size());     // 1
    
    return 0;
}
```

</details>

---

### Exercise 3: Fix Circular Dependency *(Preview of Advanced Topic)*

> 📚 This exercise previews *forward declarations*, covered in detail in [More Advanced Topics](#more-advanced-topics). Try it now for a challenge, or skip and return later.

These two headers have a circular dependency. Refactor them using forward declarations:

```c
// employee.h
#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include "department.h"

typedef struct Employee {
    char name[50];
    int id;
    Department* dept;
} Employee;

void employee_transfer(Employee* emp, Department* new_dept);

#endif
```

```c
// department.h
#ifndef DEPARTMENT_H
#define DEPARTMENT_H

#include "employee.h"

typedef struct Department {
    char name[50];
    Employee* employees[100];
    int num_employees;
} Department;

void department_add_employee(Department* dept, Employee* emp);

#endif
```

<details>
<summary><strong>Click to reveal answer</strong></summary>

**Solution using forward declarations:**

```c
// employee.h
#ifndef EMPLOYEE_H
#define EMPLOYEE_H

// Forward declaration - tells compiler Department exists
typedef struct Department Department;

typedef struct Employee {
    char name[50];
    int id;
    Department* dept;  // Pointer OK with just forward declaration
} Employee;

void employee_transfer(Employee* emp, Department* new_dept);

#endif
```

```c
// department.h
#ifndef DEPARTMENT_H
#define DEPARTMENT_H

// Forward declaration - tells compiler Employee exists
typedef struct Employee Employee;

typedef struct Department {
    char name[50];
    Employee* employees[100];  // Pointer OK with just forward declaration
    int num_employees;
} Department;

void department_add_employee(Department* dept, Employee* emp);

#endif
```

**Key principle:** Forward declarations work for pointers because the compiler doesn't need to know the size or internal structure of the pointed-to type. It only needs to know the type exists.

**When forward declarations DON'T work:**
```c
typedef struct Employee Employee;

typedef struct {
    Employee direct_employee;  // ERROR! Compiler needs to know size
} Department;
```

In this case, you'd need the full definition, which brings back the circular dependency. The solution is to use pointers or restructure your design.

</details>

---

### Exercise 4: Complete Implementation

Here's a header for a simple temperature converter. Implement the source file:

```c
// temperature.h
#ifndef TEMPERATURE_H
#define TEMPERATURE_H

typedef enum {
    CELSIUS,
    FAHRENHEIT,
    KELVIN
} TempScale;

/**
 * Convert temperature from one scale to another
 * @param value Temperature value
 * @param from Source scale
 * @param to Target scale
 * @return Converted temperature
 */
double temp_convert(double value, TempScale from, TempScale to);

/**
 * Get the name of a temperature scale
 * @param scale The scale
 * @return String like "Celsius" or "Fahrenheit"
 */
const char* temp_scale_name(TempScale scale);

/**
 * Get the symbol for a temperature scale
 * @param scale The scale
 * @return String like "°C" or "°F"
 */
const char* temp_scale_symbol(TempScale scale);

#endif
```

<details>
<summary><strong>Click to reveal answer</strong></summary>

```c
// temperature.c
#include "temperature.h"

// ============================================================================
// Private Helper Functions
// ============================================================================

static double celsius_to_fahrenheit(double c) {
    return (c * 9.0 / 5.0) + 32.0;
}

static double celsius_to_kelvin(double c) {
    return c + 273.15;
}

static double fahrenheit_to_celsius(double f) {
    return (f - 32.0) * 5.0 / 9.0;
}

static double kelvin_to_celsius(double k) {
    return k - 273.15;
}

// ============================================================================
// Public Function Implementations
// ============================================================================

double temp_convert(double value, TempScale from, TempScale to) {
    // If same scale, no conversion needed
    if (from == to) {
        return value;
    }
    
    // Strategy: Convert everything to Celsius first, then to target
    double celsius;
    
    switch (from) {
        case CELSIUS:
            celsius = value;
            break;
        case FAHRENHEIT:
            celsius = fahrenheit_to_celsius(value);
            break;
        case KELVIN:
            celsius = kelvin_to_celsius(value);
            break;
        default:
            return 0.0;  // Invalid input
    }
    
    // Now convert from Celsius to target
    switch (to) {
        case CELSIUS:
            return celsius;
        case FAHRENHEIT:
            return celsius_to_fahrenheit(celsius);
        case KELVIN:
            return celsius_to_kelvin(celsius);
        default:
            return 0.0;  // Invalid input
    }
}

const char* temp_scale_name(TempScale scale) {
    switch (scale) {
        case CELSIUS:    return "Celsius";
        case FAHRENHEIT: return "Fahrenheit";
        case KELVIN:     return "Kelvin";
        default:         return "Unknown";
    }
}

const char* temp_scale_symbol(TempScale scale) {
    switch (scale) {
        case CELSIUS:    return "°C";
        case FAHRENHEIT: return "°F";
        case KELVIN:     return "K";
        default:         return "?";
    }
}
```

**Usage example:**
```c
// main.c
#include "temperature.h"
#include <stdio.h>

int main(void) {
    double temp = 100.0;
    
    printf("%.1f%s = %.1f%s\n",
           temp,
           temp_scale_symbol(CELSIUS),
           temp_convert(temp, CELSIUS, FAHRENHEIT),
           temp_scale_symbol(FAHRENHEIT));
    
    printf("%.1f%s = %.1f%s\n",
           temp,
           temp_scale_symbol(CELSIUS),
           temp_convert(temp, CELSIUS, KELVIN),
           temp_scale_symbol(KELVIN));
    
    return 0;
}

// Output:
// 100.0°C = 212.0°F
// 100.0°C = 373.2K
```

</details>

---

### Exercise 5: Design Challenge

Design a header file for a simple library system with these requirements:

1. Books have: title, author, ISBN, availability status
2. Functions needed:
   - Add a book to the library
   - Remove a book by ISBN
   - Search for books by author
   - Check out a book
   - Return a book
   - List all available books

Don't implement it - just design the header with proper types, constants, and function declarations.

<details>
<summary><strong>Click to reveal answer</strong></summary>

```c
// library.h
#ifndef LIBRARY_H
#define LIBRARY_H

// ============================================================================
// Constants
// ============================================================================

#define LIBRARY_MAX_BOOKS 1000
#define LIBRARY_TITLE_MAX_LEN 100
#define LIBRARY_AUTHOR_MAX_LEN 50
#define LIBRARY_ISBN_LEN 18  // ISBN-13 with hyphens + null terminator

#define LIBRARY_SUCCESS 0
#define LIBRARY_ERROR_FULL -1
#define LIBRARY_ERROR_NOT_FOUND -2
#define LIBRARY_ERROR_ALREADY_EXISTS -3
#define LIBRARY_ERROR_UNAVAILABLE -4

// ============================================================================
// Types
// ============================================================================

/**
 * Book availability status
 */
typedef enum {
    BOOK_AVAILABLE,
    BOOK_CHECKED_OUT,
    BOOK_RESERVED,
    BOOK_MAINTENANCE
} BookStatus;

/**
 * Represents a single book in the library
 */
typedef struct {
    char title[LIBRARY_TITLE_MAX_LEN];
    char author[LIBRARY_AUTHOR_MAX_LEN];
    char isbn[LIBRARY_ISBN_LEN];
    BookStatus status;
    int checkout_count;  // How many times it's been checked out
} Book;

/**
 * Search results container
 */
typedef struct {
    Book* books[LIBRARY_MAX_BOOKS];
    int count;
} SearchResults;

// ============================================================================
// Library Management Functions
// ============================================================================

/**
 * Initialize the library system
 * Must be called before any other library functions
 */
void library_init(void);

/**
 * Add a new book to the library
 * @param title Book title
 * @param author Book author
 * @param isbn Book ISBN (must be unique)
 * @return LIBRARY_SUCCESS or error code
 */
int library_add_book(const char* title, const char* author, const char* isbn);

/**
 * Remove a book from the library by ISBN
 * @param isbn ISBN of book to remove
 * @return LIBRARY_SUCCESS or LIBRARY_ERROR_NOT_FOUND
 */
int library_remove_book(const char* isbn);

/**
 * Get the total number of books in the library
 * @return Number of books
 */
int library_get_book_count(void);

// ============================================================================
// Search Functions
// ============================================================================

/**
 * Search for books by author
 * @param author Author name to search for (case-insensitive partial match)
 * @param results Pointer to SearchResults to populate
 */
void library_search_by_author(const char* author, SearchResults* results);

/**
 * Search for books by title
 * @param title Title to search for (case-insensitive partial match)
 * @param results Pointer to SearchResults to populate
 */
void library_search_by_title(const char* title, SearchResults* results);

/**
 * Find a specific book by ISBN
 * @param isbn ISBN to search for
 * @return Pointer to book if found, NULL otherwise
 */
Book* library_find_by_isbn(const char* isbn);

/**
 * Get all available books
 * @param results Pointer to SearchResults to populate
 */
void library_get_available_books(SearchResults* results);

// ============================================================================
// Checkout/Return Functions
// ============================================================================

/**
 * Check out a book
 * @param isbn ISBN of book to check out
 * @return LIBRARY_SUCCESS or error code
 */
int library_checkout_book(const char* isbn);

/**
 * Return a previously checked-out book
 * @param isbn ISBN of book to return
 * @return LIBRARY_SUCCESS or error code
 */
int library_return_book(const char* isbn);

/**
 * Check if a book is available for checkout
 * @param isbn ISBN of book to check
 * @return 1 if available, 0 otherwise
 */
int library_is_available(const char* isbn);

// ============================================================================
// Utility Functions
// ============================================================================

/**
 * Get human-readable status string
 * @param status The BookStatus value
 * @return String like "Available" or "Checked Out"
 */
const char* library_status_string(BookStatus status);

/**
 * Print all books in the library (for debugging)
 */
void library_print_all_books(void);

/**
 * Clear search results (frees internal resources if needed)
 * @param results SearchResults to clear
 */
void library_clear_search_results(SearchResults* results);

#endif
```

**Design notes:**

1. **Constants defined clearly** - Magic numbers avoided
2. **Error codes** - Functions return status codes for error handling
3. **Opaque state** - Implementation details hidden (how books are stored)
4. **Const correctness** - Input strings marked `const` where not modified
5. **Documentation** - Every function documented with purpose and parameters
6. **Initialization function** - Explicit setup required
7. **Helper types** - `SearchResults` encapsulates search functionality
8. **Utility functions** - Status to string, debugging output

</details>

---

## Bonus Challenge: Apply to the TicTacToe Project

Now that you understand header files, explore the actual project structure:

### Challenge Tasks:

1. **Examine `include/board.h`** - Identify:
   - What include guard is used?
   - What types are declared?
   - What functions are available?

2. **Compare with `src/board.c`** - Find:
   - Which functions are `static` (private)?
   - How does the implementation differ from declarations?

3. **Trace Dependencies** - Starting from `main.c`:
   - Which headers does it include?
   - Draw a simple dependency graph

4. **Create Your Own Module** - Add a new module to the project:
   - `include/stats.h` - Track game statistics (wins, losses, draws)
   - `src/stats.c` - Implement the tracking
   - Integrate it with the game loop

<details>
<summary><strong>Click to reveal hints for Task 4</strong></summary>

**stats.h skeleton:**
```c
#ifndef STATS_H
#define STATS_H

typedef struct {
    int x_wins;
    int o_wins;
    int draws;
    int total_games;
} GameStats;

void stats_init(GameStats* stats);
void stats_record_win(GameStats* stats, char winner);  // 'X', 'O', or 'D' for draw
void stats_print(const GameStats* stats);
double stats_win_percentage(const GameStats* stats, char player);

#endif
```

**Integration:** Call `stats_record_win()` when a game ends in `tictactoe.c`.

</details>

---

## Summary

**Key Takeaways:**

1. **Headers declare, sources define** - This separation is fundamental to C
2. **Include guards are mandatory** - Prevent double-inclusion errors
3. **Own header first** - Catches missing dependencies early
4. **Forward declarations solve circular dependencies** - Use for pointer types
5. **Keep headers minimal** - Only expose what users need to know
6. **Document your public API** - Headers are your user manual

**Mental Model:**

Think of headers as a restaurant menu:
- **Menu (header):** Shows what dishes are available, ingredients, price
- **Kitchen (source):** The actual recipes and cooking process
- **You don't need to know how to cook it**, just what you can order

**Common Pattern:**

Every module in C typically has:
```
module_name.h   → Interface (what)
module_name.c   → Implementation (how)
```

---

## Next Steps

Continue to [Lesson 8: Function Pointers and Abstraction](LESSON_8_Function_Pointers_And_Abstraction.md) →

← Previous: [Lesson 6: Structs](LESSON_6_Structs.md)

← Back to [Learning Guide Index](LEARNING_GUIDE.md)

---

## Bonus: Advanced Topics
> 🎓 **Prerequisites:** Complete this lesson first. These topics build on header file fundamentals and connect with concepts from [Lesson 8: Function Pointers](LESSON_8_Function_Pointers_And_Abstraction.md) and [Lesson 9: Memory Management](LESSON_9_Memory_Management.md). Return here after those lessons for the full picture.

### `#pragma once` — Modern Include Guard Alternative

Instead of the traditional `#ifndef`/`#define`/`#endif` pattern, many compilers support a simpler approach:

```c
// calculator.h (Modern style)
#pragma once

typedef enum { OP_ADD, OP_SUBTRACT } Operation;
int calculate(Operation op, int a, int b);
```

**Comparison:**

| Feature        | `#ifndef` Guards | `#pragma once`            |
|----------------|------------------|---------------------------|
| Standard C     | ✅ Yes            | ❌ No (compiler extension) |
| Portability    | Universal        | Most modern compilers     |
| Name conflicts | Possible         | Impossible                |
| Symbolic links | Works            | May fail on some systems  |
| Verbosity      | 3 lines          | 1 line                    |

**When to use which:**
- **Use `#ifndef` guards** for maximum portability, open-source projects, or when you need to support older compilers
- **Use `#pragma once`** for personal projects or when your team agrees on modern compilers (GCC, Clang, MSVC all support it)

**This project uses:** Traditional `#ifndef` guards for educational clarity and maximum compatibility.

---

### `static inline` Functions — When Implementations Belong in Headers

Earlier, we said "don't put function implementations in headers." Here's the exception:

```c
// math_utils.h
#ifndef MATH_UTILS_H
#define MATH_UTILS_H

// Small, frequently-called function - OK in header!
static inline int max(int a, int b) {
    return (a > b) ? a : b;
}

// Complex function - declaration only
int factorial(int n);

#endif
```

**Why this works:**
- `static` means each `.c` file gets its own copy (no linker conflict)
- `inline` suggests the compiler insert the code directly at call sites (avoiding function call overhead)

**When to use `static inline`:**
- ✅ Very small functions (1-3 lines)
- ✅ Performance-critical code called in tight loops
- ✅ Simple accessors or predicates

**When NOT to use:**
- ❌ Functions longer than ~5 lines (code bloat)
- ❌ Functions with complex logic
- ❌ Functions that might change frequently (forces recompilation of all including files)

**Real-world example from this project:**

```c
// In a header - perfect candidate for static inline
static inline int is_valid_position(int pos) {
    return pos >= 0 && pos < 9;
}

// NOT a good candidate - too complex, keep in .c file
int calculate_winner(CellValue board[9]);  // Declaration only
```

---

### Forward Declarations — Solving Circular Dependencies
When two headers need each other, you get an infinite include loop:

```c
// game.h - PROBLEM: includes player.h, which includes game.h, which...
#include "player.h"

typedef struct {
    Player* current_player;
} Game;
```

**The Solution: Forward Declarations**

Tell the compiler a type *exists* without providing its full definition:

```c
// game.h - SOLUTION
#ifndef GAME_H
#define GAME_H

// Forward declaration: "Player is a struct type that exists somewhere"
typedef struct Player Player;

typedef struct Game {
    Player* current_player;  // ✅ Pointer to forward-declared type is OK
    int turn_count;
} Game;

void game_start(Game* game);

#endif
```

```c
// player.h
#ifndef PLAYER_H
#define PLAYER_H

// Forward declaration for Game
typedef struct Game Game;

typedef struct Player {
    char name[50];
    Game* current_game;  // ✅ Pointer to forward-declared type is OK
} Player;

void player_make_move(Player* player, int position);

#endif
```

**Key Rules:**

1. **Forward declarations work for pointers only:**
   ```c
   typedef struct Player Player;
   
   Player* ptr;      // ✅ OK - compiler just needs to know it's a pointer
   Player instance;  // ❌ ERROR - compiler needs full size/layout
   ```

2. **Use the struct tag in forward declarations:**
   ```c
   typedef struct Player Player;  // ✅ Correct
   typedef Player Player;         // ❌ Wrong - Player not defined yet
   ```

3. **Include the full header in the `.c` file:**
   ```c
   // game.c
   #include "game.h"
   #include "player.h"  // Now we need the full Player definition
   
   void game_start(Game* game) {
       // Can access player->name here because player.h is included
   }
   ```

**When forward declarations aren't enough:**

If you need to access struct members or know the size, you must include the full header:

```c
// In player.c - need full Game definition to access game->turn_count
#include "player.h"
#include "game.h"  // Full include needed here

void player_make_move(Player* player, int position) {
    player->current_game->turn_count++;  // Accessing Game members
}
```

---

### Conditional Compilation — Platform-Specific Code
The preprocessor can include or exclude code based on conditions:

```c
// platform.h
#ifndef PLATFORM_H
#define PLATFORM_H

// Detect operating system
#if defined(_WIN32) || defined(_WIN64)
    #define PLATFORM_WINDOWS 1
    #define PATH_SEPARATOR '\\'
#elif defined(__APPLE__)
    #define PLATFORM_MACOS 1
    #define PATH_SEPARATOR '/'
#elif defined(__linux__)
    #define PLATFORM_LINUX 1
    #define PATH_SEPARATOR '/'
#else
    #error "Unsupported platform"
#endif

// Platform-specific function declarations
void platform_clear_screen(void);
const char* platform_get_home_dir(void);

#endif
```

**Common Patterns:**

```c
// Debug-only code
#ifdef DEBUG
    #define LOG(msg) printf("[DEBUG] %s\n", msg)
#else
    #define LOG(msg)  // Compiles to nothing in release
#endif

// Feature flags
#ifdef ENABLE_AI_HARD_MODE
    int ai_minimax(GameState* state, int depth);
#endif

// Header version guards
#if __STDC_VERSION__ >= 199901L
    // C99 features available
    #include <stdbool.h>
#else
    // Provide fallback
    typedef int bool;
    #define true 1
    #define false 0
#endif
```

**Useful Predefined Macros:**

| Macro | Meaning |
|-------|---------|
| `__FILE__` | Current source filename |
| `__LINE__` | Current line number |
| `__func__` | Current function name (C99+) |
| `__DATE__` | Compilation date |
| `__TIME__` | Compilation time |
| `__STDC_VERSION__` | C standard version |

**Example: Debug assertion macro:**

```c
#ifdef DEBUG
    #define ASSERT(cond, msg) \
        do { \
            if (!(cond)) { \
                fprintf(stderr, "ASSERT FAILED: %s\n", msg); \
                fprintf(stderr, "  at %s:%d in %s()\n", __FILE__, __LINE__, __func__); \
                abort(); \
            } \
        } while(0)
#else
    #define ASSERT(cond, msg)  // No-op in release
#endif
```

