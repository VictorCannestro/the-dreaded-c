# Pitfalls: Function Declarations & Definitions

> Part of the [C Pitfalls](README.md) series.

Errors in this category involve how functions and headers are declared. These bugs are often invisible at first glance but lead to broken type checking, subtle runtime crashes, or silent include failures.

## Pitfalls in This Section

- [2. Empty Parameter List `()`](#2-empty-parameter-list-)
- [15. Header Guard Typos](#15-header-guard-typos)

---

### 2. Empty Parameter List `()`

**Severity:** 🟡 Warning (but dangerous!)  
**Frequency:** Common when coming from C++

#### The Problem

```c
// utils.h
int newline();  // Looks innocent, but means "unknown parameters"!

// Later in code...
newline(42, "hello", 3.14);  // COMPILES! No type checking!
```

#### Why This is Dangerous

In C (unlike C++), an empty parameter list `()` means:
- **"This function takes an unspecified number of arguments"**
- The compiler **cannot** type-check calls to this function
- Any arguments can be passed without warning
- This is a relic from K&R C (pre-ANSI C)

Compare with C++:
```cpp
// C++
int newline();  // Means "no parameters" (same as void)

// C
int newline();  // Means "unknown parameters" (dangerous!)
```

#### The Solution

```c
// utils.h
int newline(void);  // Explicitly means "exactly zero parameters"

// Now this won't compile:
newline(42);  // Error: too many arguments to function call
```

#### Real Project Warning

This was an actual warning from our project:
```
warning: a function declaration without a prototype is deprecated 
in all versions of C [-Wstrict-prototypes]
    int newline();
                ^
                 void
```

#### Complete Example

```c
// ❌ WRONG - Dangerous declaration
int get_input();

int main(void) {
    get_input(10, "extra");  // Compiles! Undefined behavior at runtime
    return 0;
}

int get_input() {
    // Expected no parameters, but got 2!
    // Stack corruption possible
    return 0;
}
```

```c
// ✅ CORRECT - Safe declaration
int get_input(void);

int main(void) {
    get_input(10, "extra");  // Compile error: too many arguments
    return 0;
}

int get_input(void) {
    return 0;
}
```

#### Best Practices

1. **Always use `(void)` for no-parameter functions:**
   ```c
   void init_game(void);
   int get_random_number(void);
   ```

2. **Enable strict prototype warnings:**
   ```bash
   gcc -Wstrict-prototypes -Werror=strict-prototypes ...
   ```

3. **Use proper prototypes for all functions:**
   ```c
   int add(int a, int b);              // ✅ Good
   int add(int, int);                  // ✅ Also good
   int add();                          // ❌ Bad - unspecified
   ```

#### Historical Context

This behavior exists for backward compatibility with K&R C (pre-1989):
```c
/* Old K&R style - deprecated */
int add(a, b)
    int a;
    int b;
{
    return a + b;
}
```

Modern C (ANSI C89 and later) requires proper prototypes.

---

### 15. Header Guard Typos

**Severity:** 🟡 Medium - Subtle Include Issues  
**Frequency:** Common copy-paste mistake

#### The Problem

```c
// board.h
#ifndef TICTACTOE_H  // WRONG guard! (copied from tictactoe.h)
#define TICTACTOE_H
// ... board declarations ...
#endif

// Now if tictactoe.h is included first, board.h contents are skipped!
```

#### Why This is Dangerous

Header guards prevent multiple inclusion:

```c
// Without guards:
#include "game.h"  // Includes board.h
#include "board.h" // board.h included again - redefinition errors!

// With CORRECT guards:
// First inclusion: guard not defined, contents included
// Second inclusion: guard defined, contents skipped ✓

// With WRONG guards:
// board.h uses TICTACTOE_H guard
// If tictactoe.h included first, board.h contents never included!
```

#### Real-World Example

```c
// tictactoe.h
#ifndef TICTACTOE_H
#define TICTACTOE_H
typedef enum { X_WINS, O_WINS, DRAW } GameResult;
#endif

// board.h - COPY-PASTED guard!
#ifndef TICTACTOE_H  // BUG: Should be BOARD_H
#define TICTACTOE_H
typedef enum { CELL_X, CELL_O, CELL_EMPTY } CellValue;
#endif

// main.c
#include "tictactoe.h"  // TICTACTOE_H now defined
#include "board.h"      // Contents skipped! CellValue not defined

int main(void) {
    CellValue cell = CELL_X;  // ERROR: unknown type CellValue
    return 0;
}
```

#### The Solution

**Use unique guards matching the filename:**

```c
// board.h
#ifndef BOARD_H  // Matches filename
#define BOARD_H

typedef enum {
    CELL_EMPTY,
    CELL_X,
    CELL_O
} CellValue;

#endif  // BOARD_H

// tictactoe.h
#ifndef TICTACTOE_H  // Matches filename
#define TICTACTOE_H

#include "board.h"

typedef struct {
    CellValue board[9];
    // ...
} GameState;

#endif  // TICTACTOE_H
```

#### Best Practices

**1. Naming Convention:**
```c
// Filename: ai_module.h
#ifndef AI_MODULE_H
#define AI_MODULE_H
// ...
#endif  // AI_MODULE_H

// Or with project prefix:
#ifndef TICTACTOE_AI_MODULE_H
#define TICTACTOE_AI_MODULE_H
// ...
#endif
```

**2. Use `#pragma once` (non-standard but widely supported):**
```c
// board.h
#pragma once  // Simpler, no typo risk

typedef enum {
    CELL_EMPTY,
    CELL_X,
    CELL_O
} CellValue;
```

**3. Consistent format:**
```c
// Format 1: Simple
#ifndef BOARD_H
#define BOARD_H
// content
#endif

// Format 2: With comment on #endif
#ifndef BOARD_H
#define BOARD_H
// content
#endif  // BOARD_H

// Format 3: With project prefix
#ifndef MYPROJECT_BOARD_H
#define MYPROJECT_BOARD_H
// content
#endif  // MYPROJECT_BOARD_H
```

**4. Editor templates:**
Most IDEs can generate headers with correct guards automatically.

#### Modern Alternative: `#pragma once`

```c
// board.h
#pragma once  // Instead of header guards

typedef enum {
    CELL_EMPTY,
    CELL_X,
    CELL_O
} CellValue;

// That's it! No #endif, no typos possible
```

**Pros:**
- Simpler
- No typo risk
- Slightly faster compilation

**Cons:**
- Not standard C (though widely supported)
- May not work on all compilers
- Some build systems have issues with it

#### Detection

**Check for duplicate guards:**
```bash
#!/bin/bash
guards=$(grep -h "^#ifndef" include/*.h | awk '{print $2}' | sort)
duplicates=$(echo "$guards" | uniq -d)

if [ -n "$duplicates" ]; then
    echo "Duplicate header guards found:"
    echo "$duplicates"
    exit 1
fi
```

**Check guards match filenames:**
```bash
for file in include/*.h; do
    basename=$(basename "$file" .h | tr 'a-z' 'A-Z')
    expected="${basename}_H"
    actual=$(grep "^#ifndef" "$file" | awk '{print $2}')
    
    if [ "$actual" != "$expected" ]; then
        echo "Mismatch in $file: expected $expected, got $actual"
    fi
done
```

