# Pitfalls: Operators & Control Flow

> Part of the [C Pitfalls](README.md) series.

Errors in this category stem from operator misuse and control flow constructs that behave differently in C than most programmers expect.

## Pitfalls in This Section

- [9. Assignment vs Comparison (`=` vs `==`)](#9-assignment-vs-comparison--vs-)
- [12. Forgetting `break` in `switch`](#12-forgetting-break-in-switch)

---

### 9. Assignment vs Comparison (`=` vs `==`)

**Severity:** 🔴 Critical - Logic Bug  
**Frequency:** Common mistake

#### The Problem

```c
if (state->status = ONGOING) {  // BUG: Assignment, not comparison!
    printf("Game is ongoing\n");  // ALWAYS executes if ONGOING != 0
}
// state->status has been changed to ONGOING!
```

#### Why This is Dangerous

1. **Assignment operator `=` returns a value:** The assigned value
2. **Used in conditions:** Non-zero is true, zero is false
3. **Side effect:** Variable is modified as well as evaluated
4. **Hard to spot:** Looks almost identical to `==`

```c
int x = 5;

if (x = 10) {  // Assigns 10 to x, then checks if 10 is non-zero (true)
    // Always executes because 10 != 0
    printf("x is now %d\n", x);  // Prints: x is now 10
}
```

#### Real-World Example

```c
GameStatus check_winner(GameState *state) {
    // Intended: check if X won
    // Bug: assigns X_WINS to status!
    if (state->status = X_WINS) {
        return X_WINS;
    }
    
    // Now state->status is X_WINS even if O actually won!
    return NO_WINNER;
}
```

#### The Solution

**1. Use comparison operator `==`:**
```c
if (state->status == ONGOING) {  // ✅ Comparison
    printf("Game is ongoing\n");
}
```

**2. Yoda conditions (optional style):**
```c
// Put constant on left — assignment will cause a compile error
if (ONGOING = state->status) {  // Error: cannot assign to constant
    // ...
}

if (ONGOING == state->status) {  // ✅ OK
    // ...
}
```

**3. Enable compiler warnings:**
```bash
gcc -Wall -Wextra -Wparentheses program.c

# Warning: suggest parentheses around assignment used as truth value
```

**4. Extra parentheses when assignment in condition is intentional:**
```c
// Rare case where assignment in condition is desired:
if ((status = get_status()) == ERROR) {
    // Double parentheses signal: "yes, I meant assignment"
}
```

#### Complete Example

```c
// ❌ WRONG
void update_game(GameState *state) {
    if (state->status = GAME_OVER) {  // BUG: assigns, doesn't compare
        save_game(state);
    }
    // state->status is now always GAME_OVER!
}

// ✅ CORRECT
void update_game(GameState *state) {
    if (state->status == GAME_OVER) {
        save_game(state);
    }
}

// ✅ Intentional assignment (rare, must use extra parentheses)
void process(void) {
    int value;
    if ((value = get_next()) > 0) {
        process_value(value);
    }
}
```

#### Detection and Prevention

**1. Compiler Warnings:**
```bash
gcc -Wall -Wextra program.c
# Warns: suggest parentheses around assignment used as truth value
```

**2. Static Analysis:**
```bash
clang-tidy program.c --checks=-*,bugprone-assignment-in-if-condition
```

#### Best Practices

1. **Always use `==` for comparison:**
   ```c
   if (x == 10) { }
   while (status == ONGOING) { }
   ```

2. **Enable all warnings and treat them as errors:**
   ```bash
   gcc -Wall -Wextra -Werror ...
   ```

3. **Consider Yoda conditions for constants:**
   ```c
   if (NULL == ptr) { }      // A typo like NULL = ptr won't compile
   if (ONGOING == status) { } // Forces you to use ==
   ```

---

### 12. Forgetting `break` in `switch`

**Severity:** 🟡 Medium - Logic Bug  
**Frequency:** Common

#### The Problem

```c
switch (cell) {
    case CELL_X:
        printf("X");
        // FALLS THROUGH to next case!
    case CELL_O:
        printf("O");
        break;
    case CELL_EMPTY:
        printf(" ");
        break;
}
```

**Result:** When `cell == CELL_X`, prints `"XO"` instead of just `"X"`.

#### Why This Happens

In C, `switch` statements have **fall-through behavior**:
- After executing a `case`, execution continues into the next `case`
- You must use `break` (or `return`) to exit the switch
- This is inherited from C's predecessor language, B

```c
int x = 1;

switch (x) {
    case 1:
        printf("one\n");    // Executes
    case 2:
        printf("two\n");    // Also executes! (fall-through)
    case 3:
        printf("three\n");  // Also executes!
        break;              // Finally exits
}

// Output:
// one
// two
// three
```

#### When Fall-Through is Intentional

Fall-through can be useful for grouping cases with the same action:

```c
switch (symbol) {
    case 'a':
    case 'A':  // Falls through from 'a'
        printf("Letter A\n");
        break;
    
    case 'e':
    case 'E':
        printf("Letter E\n");
        break;
}
```

**Always document intentional fall-through:**
```c
switch (command) {
    case CMD_INIT:
        initialize();
        /* FALLTHROUGH */  // Comment makes intent explicit
    case CMD_START:
        start_game();
        break;
    
    case CMD_QUIT:
        cleanup();
        break;
}
```

#### The Solution

**Always use `break` (unless fall-through is intentional):**

```c
// ✅ CORRECT
switch (cell) {
    case CELL_X:
        printf("X");
        break;
    
    case CELL_O:
        printf("O");
        break;
    
    case CELL_EMPTY:
        printf(" ");
        break;
    
    default:
        printf("?");
        break;  // Good practice even on the last case
}
```

#### Real-World Example from Tic-Tac-Toe

```c
// ❌ WRONG - Missing break
char get_symbol(CellValue cell) {
    char symbol;
    switch (cell) {
        case CELL_X:
            symbol = 'X';
            // Missing break! Falls through to CELL_O
        case CELL_O:
            symbol = 'O';  // Overwrites 'X'!
            break;
        case CELL_EMPTY:
            symbol = ' ';
            break;
        default:
            symbol = '?';
            break;
    }
    return symbol;
}
// get_symbol(CELL_X) returns 'O'!

// ✅ CORRECT - Use return to avoid needing break at all
char get_symbol(CellValue cell) {
    switch (cell) {
        case CELL_X:     return 'X';
        case CELL_O:     return 'O';
        case CELL_EMPTY: return ' ';
        default:         return '?';
    }
}
```

#### Alternative: Use `return`

```c
// No breaks needed if every case returns
int get_score(GameResult result) {
    switch (result) {
        case WIN:  return 10;
        case DRAW: return 5;
        case LOSS: return 0;
        default:   return -1;
    }
}
```

#### Detection

**1. Compiler Warnings:**
```bash
gcc -Wimplicit-fallthrough program.c

# Warning: this statement may fall through
```

**2. Annotate Intentional Fall-Through (C23 attribute):**
```c
switch (x) {
    case 1:
        action1();
        [[fallthrough]];  // C23 — suppresses the warning
    case 2:
        action2();
        break;
}
```

**3. Static Analysis:**
```bash
clang-tidy --checks=bugprone-switch-missing-default-case program.c
```

#### Best Practices

1. **Always use `break` or `return` in every case:**
   ```c
   case CELL_X:
       printf("X");
       break;
   ```

2. **Always include `default`:**
   ```c
   default:
       fprintf(stderr, "Unexpected value: %d\n", cell);
       break;
   ```

3. **Comment intentional fall-through:**
   ```c
   case 'a':
       /* FALLTHROUGH */
   case 'A':
       handle_a();
       break;
   ```

4. **Prefer `return` over `break` when possible** — it makes fall-through impossible:
   ```c
   switch (cell) {
       case CELL_X: return 'X';
       case CELL_O: return 'O';
       default:     return ' ';
   }
   ```

