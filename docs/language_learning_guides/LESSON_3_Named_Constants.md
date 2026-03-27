# Lesson 3: Varieties of Named Constants

Magic numbers make code unreadable. What does `if (status == 3)` mean? Is `board[4] = 2` placing
an X or an O? C gives you three tools to replace magic numbers with meaningful names.

This lesson introduces all three tools in order of complexity, then focuses on **enums** — the
tool you'll use most in this project and in most real-world C code.

---

## Table of Contents
### Core Concepts
**The Problem and Three Solutions**
1. [The Problem: Magic Numbers](#1-the-problem-magic-numbers)
2. [Three Tools for Named Constants](#2-three-tools-for-named-constants)

**Deep Dive: Enums (the main focus)**
3. [Basic Enum Syntax](#3-basic-enum-syntax)
4. [How Enums Work Under the Hood](#4-how-enums-work-under-the-hood)
5. [All Enums in This Project](#5-all-enums-in-this-project)
6. [Using Enums with Logical Operators](#6-using-enums-with-logical-operators)
7. [Common Mistakes and Best Practices](#7-common-mistakes-and-best-practices)

### Practice
8. [Decision Guide: Which Tool When?](#8-decision-guide-which-tool-when)
9. [Practical Exercises](#practical-exercises)

### Going Further
10. [Summary](#summary)
11. [Next Steps](#next-steps)
12. [Bonus: Advanced Topics](#bonus-advanced-topics) *(return after Lessons 2 & 4)*


---

## 1. The Problem: Magic Numbers
Consider this code:

```c
int board[9];
board[0] = 1;
board[4] = 2;

if (board[0] == 1) {
    printf("Someone is here\n");
}

if (status == 3) {
    printf("Game over\n");
}
```

What do `1`, `2`, and `3` mean? Is `1` an X? An O? A player ID? You can't tell without hunting
through the codebase — and neither can you six months from now, or anyone else reading your code.

These unexplained numeric literals are called **magic numbers**, and they cause bugs:

```c
board[5] = 3;  // Is 3 valid? The compiler won't tell you.
```

The fix is to give every significant value a **name**. C provides three ways to do this.

---

## 2. Three Tools for Named Constants
Here they are, from simplest to most powerful:

### Tool 1: `#define` — Text Substitution

```c
#define BOARD_SIZE 9
#define CELL_X     1
#define CELL_O     2
```

The preprocessor replaces every occurrence of the name with its text *before* the compiler runs.
The compiler never sees `BOARD_SIZE` — only `9`.

- **Pros:** Works for any value (integers, floats, strings). Required for array sizes in C89/C90.
- **Cons:** No type. No scope. No debugger visibility. Easy to misuse.

### Tool 2: `const` — Typed Constant Variable

```c
const char X_MARKER = 'X';
const double PI = 3.14159;
```

A regular variable marked `const` — the compiler enforces that it can't be changed after
initialisation. It has a type, a scope, and a name the debugger can display.

- **Pros:** Type-safe. Debugger-friendly. Scoped. 
- **Cons:** Can't be used for array sizes in C89/C90. Doesn't group related values.

### Tool 3: `enum` — Named Integer Sets

```c
typedef enum {
    CELL_EMPTY = 0,
    CELL_X     = 1,
    CELL_O     = 2
} CellValue;
```

Creates a new type with named integer constants. The compiler knows these values belong together,
and a debugger shows `CELL_X` instead of `1`.

- **Pros:** Type-safe. Debugger-friendly. Groups related values under one name.
- **Cons:** Integers only. Can't compute across enums.


### Quick Comparison

| Property             | `#define`          | `const`            | `enum`              |
|----------------------|--------------------|--------------------|---------------------|
| Has a type           | ❌ No               | ✅ Yes              | ✅ Yes (int)         |
| Visible in debugger  | ❌ No               | ✅ Yes              | ✅ Yes               |
| Groups related values| ❌ No               | ❌ No               | ✅ Yes               |
| Works for floats     | ✅ Yes              | ✅ Yes              | ❌ No                |
| Works for array sizes| ✅ Yes              | ❌ No (C89)         | ❌ No                |

**Bottom line:** For sets of related integer values (game states, cell types, player kinds), use
**enums**. That's what the rest of this lesson focuses on.

---

## 3. Basic Enum Syntax

Here's the anatomy of an enum definition:

```c
typedef enum {       // ← Start with 'typedef enum {'
    CELL_EMPTY = 0,  // ← Named constant with explicit value
    CELL_X = 1,      // ← Values are optional but recommended for clarity
    CELL_O = 2       // ← Each name must be unique
} CellValue;         // ← Type name you'll use in code
```

### Breaking It Down

| Part | Purpose |
|------|---------|
| `typedef` | Creates a new type name (so you can use `CellValue` instead of `enum CellValue`) |
| `enum` | Keyword that defines an enumeration |
| `CELL_EMPTY`, `CELL_X`, `CELL_O` | The named constants (by convention, UPPERCASE) |
| `= 0`, `= 1`, `= 2` | Explicit values (optional but recommended) |
| `CellValue` | The type name you'll use to declare variables |

### Declaring and Using Enum Variables

```c
// Declare a variable of type CellValue
CellValue cell = CELL_EMPTY;

// Assign a new value
cell = CELL_X;

// Compare values
if (cell == CELL_X) {
    printf("This cell contains X\n");
}

// Use in switch statements (a natural fit!)
switch (cell) {
    case CELL_EMPTY:
        printf("Empty\n");
        break;
    case CELL_X:
        printf("X\n");
        break;
    case CELL_O:
        printf("O\n");
        break;
}
```

---

## 4. How Enums Work Under the Hood

Enums are actually just integers. When you write `CELL_X`, the compiler sees `1`.

### Automatic Value Assignment

If you don't specify values, C assigns them automatically starting from 0:

```c
typedef enum {
    CELL_EMPTY,   // = 0 (automatic)
    CELL_X,       // = 1 (automatic: previous + 1)
    CELL_O        // = 2 (automatic: previous + 1)
} CellValue;
```

You can also mix explicit and automatic:

```c
typedef enum {
    STATE_INIT = 100,   // = 100 (explicit)
    STATE_RUNNING,      // = 101 (automatic: 100 + 1)
    STATE_PAUSED,       // = 102 (automatic: 101 + 1)
    STATE_STOPPED = 200,// = 200 (explicit - jumps!)
    STATE_ERROR         // = 201 (automatic: 200 + 1)
} GameState;
```

### Enums Are Really Just Integers

Since enums are integers, you can:

```c
CellValue cell = CELL_X;

// Print the underlying integer value
printf("CELL_X = %d\n", cell);  // Output: CELL_X = 1

// Perform arithmetic (though not usually recommended)
printf("CELL_X + 1 = %d\n", CELL_X + 1);  // Output: CELL_X + 1 = 2

// Compare with integers
if (cell == 1) {  // Works, but CELL_X is clearer
    printf("It's X\n");
}
```

⚠️ **Caution:** Just because you *can* do arithmetic on enums doesn't mean you *should*. Use the named constants for clarity.

---

## 5. All Enums in This Project

Let's explore each enum defined in [`include/constants.h`](../../include/constants.h):

### CellValue: What's in Each Board Cell

```c
typedef enum {
    CELL_EMPTY = 0,  // No piece placed yet
    CELL_X = 1,      // X occupies this cell
    CELL_O = 2       // O occupies this cell
} CellValue;
```

**Usage example:**
```c
CellValue board[9];          // The game board
board[4] = CELL_X;           // X plays center
CellValue current = CELL_O;  // O's turn next

if (board[0] == CELL_EMPTY) {
    printf("Corner is available!\n");
}
```

### GameStatus: Is the Game Over?

```c
typedef enum {
    ONGOING = 0,  // Game still in progress
    X_WINS = 1,   // X has won
    O_WINS = 2,   // O has won
    DRAW = 3      // Board full, no winner
} GameStatus;
```

**Usage example:**
```c
GameStatus status = check_winner(board);

switch (status) {
    case ONGOING:
        printf("Keep playing!\n");
        break;
    case X_WINS:
        printf("X wins!\n");
        break;
    case O_WINS:
        printf("O wins!\n");
        break;
    case DRAW:
        printf("It's a draw!\n");
        break;
}
```

### PlayerType: Human or Computer?

```c
typedef enum {
    PLAYER_HUMAN = 0,     // Human player
    PLAYER_COMPUTER = 1   // AI player
} PlayerType;
```

**Usage example:**
```c
PlayerType player_x = PLAYER_HUMAN;
PlayerType player_o = PLAYER_COMPUTER;

if (player_x == PLAYER_HUMAN) {
    get_human_input();
} else {
    get_ai_move();
}
```

### Difficulty: AI Strength

```c
typedef enum {
    DIFFICULTY_EASY = 0,  // Random moves
    DIFFICULTY_HARD = 1   // Minimax algorithm (unbeatable)
} Difficulty;
```

**Usage example:**
```c
Difficulty level = DIFFICULTY_HARD;

int ai_move;
if (level == DIFFICULTY_EASY) {
    ai_move = random_move(board);
} else {
    ai_move = minimax_move(board);
}
```

---

## 6. Using Enums with Logical Operators

Enums work naturally with C's logical operators (`&&`, `||`, `!`). Here are practical patterns from game development:

### Combining Conditions with AND (`&&`)

Check if multiple conditions are true:

```c
// Is it safe to make a move?
if (status == ONGOING && board[position] == CELL_EMPTY) {
    board[position] = current_player;
}

// Is this a human's turn and is AI disabled?
if (player_type == PLAYER_HUMAN && ai_enabled == 0) {
    wait_for_input();
}
```

### Checking Alternatives with OR (`||`)

Check if any condition is true:

```c
// Is the game over? (either player won OR it's a draw)
if (status == X_WINS || status == O_WINS || status == DRAW) {
    printf("Game Over!\n");
}

// Shorter form: check if NOT ongoing
if (status != ONGOING) {
    printf("Game Over!\n");
}

// Is the current cell occupied by either player?
if (board[i] == CELL_X || board[i] == CELL_O) {
    printf("Cell %d is occupied\n", i);
}
```

### Negation with NOT (`!`)

Check if something is NOT true:

```c
// Is the cell NOT empty?
if (board[position] != CELL_EMPTY) {
    printf("Cell already occupied!\n");
}

// Alternative using !:
// First check if the cell IS empty, then negate
int is_empty = (board[position] == CELL_EMPTY);
if (!is_empty) {
    printf("Cell already occupied!\n");
}
```

### Combining Multiple Operators

Complex conditions require careful ordering:

```c
// Valid move: game ongoing AND position in range AND cell empty
if (status == ONGOING && position >= 0 && position < 9 && board[position] == CELL_EMPTY) {
    make_move(board, position, current_player);
}

// Game continues: not won AND moves remaining
int moves_left = count_empty_cells(board);
if (status == ONGOING && moves_left > 0) {
    continue_game();
}
```

### Short-Circuit Evaluation with Enums

Remember: C stops evaluating `&&` at the first `false` and `||` at the first `true`:

```c
// Safe: if status is not ONGOING, we don't bother checking the board
if (status == ONGOING && board[position] == CELL_EMPTY) {
    // Only check board[] if game is ongoing
}

// Game over: stops checking after finding a winner
if (status == X_WINS || status == O_WINS || check_draw(board)) {
    // If X or O won, we don't call check_draw()
}
```

---

## 7. Common Mistakes and Best Practices
### ❌ Don't: Use Raw Numbers
```c
// Bad - what does 2 mean?
if (board[0] == 2) { ... }

// Good - self-documenting
if (board[0] == CELL_O) { ... }
```

### ❌ Don't: Forget Switch Cases
```c
// Bad - missing DRAW case!
switch (status) {
    case ONGOING: break;
    case X_WINS: break;
    case O_WINS: break;
    // DRAW is not handled - might cause bugs!
}

// Good - handle all cases, use default for safety
switch (status) {
    case ONGOING: break;
    case X_WINS: break;
    case O_WINS: break;
    case DRAW: break;
    default:
        printf("Unknown status!\n");  // Catch unexpected values
        break;
}
```

### ✅ Do: Use Explicit Values for Clarity
```c
// Better to be explicit, especially for values that might be stored/transmitted
typedef enum {
    CELL_EMPTY = 0,
    CELL_X = 1,
    CELL_O = 2
} CellValue;
```

### ✅ Do: Name Enums Descriptively
```c
// Bad names
typedef enum { A, B, C } Thing;

// Good names - clear meaning
typedef enum {
    CELL_EMPTY,
    CELL_X,
    CELL_O
} CellValue;
```

---

## 8. Decision Guide: Which Tool When?

Now that you understand all three tools, here's how to choose between them. We'll look at how the
project uses each one deliberately, then give you a simple flowchart.

### How the Project Uses Each Tool

The split in `constants.h` and `constants.c` is deliberate:

```c
// include/constants.h

// ── #define: configuration values used as array sizes and in arithmetic
#define BOARD_DIM  3
#define BOARD_SIZE (BOARD_DIM * BOARD_DIM)   // needed at compile time for array declarations
#define WIN_LENGTH BOARD_DIM

// ── extern const: display characters — typed, non-integer, one value each
extern const char X_MARKER;      // defined in constants.c
extern const char O_MARKER;
extern const char EMPTY_MARKER;

// ── enum: closed sets of named game states
typedef enum { CELL_EMPTY = 0, CELL_X = 1, CELL_O = 2 } CellValue;
typedef enum { ONGOING = 0, X_WINS = 1, O_WINS = 2, DRAW = 3 } GameStatus;
```

**Why `#define` for `BOARD_DIM` and not `const int`?**
Array sizes in C must be compile-time constants. A `const int` variable is not a compile-time
constant in C89/C90 — you can't write `int board[board_size]` if `board_size` is a `const int`.
`#define` works because the number is substituted before the compiler sees the declaration.

```c
// ✅ Works — BOARD_SIZE is substituted to 9 before compilation
CellValue board[BOARD_SIZE];

// ❌ Fails in C89/C90 — const int is not a compile-time constant
const int board_size = 9;
CellValue board[board_size];   // error: variable-length array
```

> **C99 note:** C99 introduced variable-length arrays (VLAs), so `board[board_size]` would
> technically compile — but it's still poor practice for fixed sizes like this.

**Why `const char` for the markers and not `#define`?**
The markers are characters — they have a clear type (`char`) and benefit from type checking.
Using `const char` also means a debugger can show `X_MARKER = 'X'` rather than just `88` (the
ASCII code). Since they aren't used as array sizes, there's no reason to reach for `#define`.

**Why `enum` for `CellValue`, `GameStatus`, etc. and not `#define`?**
These are *closed sets* — there are exactly three cell states, exactly four game outcomes. An enum
bundles them under a named type so the compiler can warn if you assign a `GameStatus` where a
`CellValue` was expected, and a debugger can show `CELL_X` instead of `1`.

---

### The Flowchart

When you need a constant, work through these questions:

```
Is it a set of related integer values (states, categories, options)?
  └─ YES → enum

Is it a non-integer value (char, float, string)?
  └─ YES → const variable

Does it need to be used as an array size, or in a #ifdef?
  └─ YES → #define

Otherwise → const variable (safer default than #define)
```

---

> **Want to go deeper on `#define`?** The mechanics of text substitution, function-like macros,
> pitfall patterns, and a full `#define` vs. `enum` comparison are in the
> [More Advanced Topics](#10-more-advanced-topics) section — return here after
> [Lesson 4: Functions](LESSON_4_Functions.md) when macros with parameters will make more sense.

---

## 9. Practical Exercises
These exercises cover all three sections of the lesson: enums, `const`, and `#define`. Try to
solve each one before revealing the answer.

---

### Exercise 1: Basic Enum Declaration
Create an enum called `Direction` with four values: `UP`, `DOWN`, `LEFT`, `RIGHT`. Make `UP = 0`
and let the others auto-increment.

<details>
<summary>Click to reveal answer</summary>

```c
typedef enum {
    UP = 0,    // = 0 (explicit)
    DOWN,      // = 1 (auto)
    LEFT,      // = 2 (auto)
    RIGHT      // = 3 (auto)
} Direction;
```

</details>

---

### Exercise 2: Using Enums in a Switch
Write a `switch` that takes a `GameStatus` variable and prints an appropriate message. Handle all
four cases: `ONGOING`, `X_WINS`, `O_WINS`, `DRAW`.

<details>
<summary>Click to reveal answer</summary>

```c
switch (status) {
    case ONGOING:
        printf("The game is in progress.\n");
        break;
    case X_WINS:
        printf("Congratulations! X wins!\n");
        break;
    case O_WINS:
        printf("Congratulations! O wins!\n");
        break;
    case DRAW:
        printf("It's a draw! Well played both.\n");
        break;
    default:
        printf("Unknown game status.\n");
        break;
}
```

</details>

---

### Exercise 3: Enum with Logical Operators
Write an expression that evaluates to `1` if the game has ended (any result except `ONGOING`), and
`0` otherwise. Write it two ways: once using `||`, once using `!=`.

<details>
<summary>Click to reveal answer</summary>

```c
// Using OR — explicitly lists terminal states
(status == X_WINS || status == O_WINS || status == DRAW)

// Using NOT — simpler and still correct
(status != ONGOING)
```

Both are valid. The `!=` form is preferred when the set of non-ongoing states might grow — you
only have to add new cases to the enum, not to every condition.

</details>

---

### Exercise 4: Enum Arithmetic
Given `CellValue` where `CELL_X = 1` and `CELL_O = 2`, write an expression that takes a player's
symbol and returns the opponent's symbol. Write it two ways. (Hint: what does `3 - CELL_X` equal?)

<details>
<summary>Click to reveal answer</summary>

```c
// Arithmetic — exploits the symmetric values
3 - player        // 3 - 1 = 2 (CELL_O), 3 - 2 = 1 (CELL_X)

// Ternary — explicit, works for any values
(player == CELL_X) ? CELL_O : CELL_X
```

The arithmetic trick is clever but fragile: it breaks if the enum values ever change. The ternary
is safer and clearer for production code.

</details>

---

### Exercise 5: Validating Input with Enums
Write a logical expression that returns `1` if an integer `value` is a valid `CellValue` (0, 1,
or 2), and `0` otherwise. Write it two ways.

<details>
<summary>Click to reveal answer</summary>

```c
// Explicit membership check — works for any set of values
(value == CELL_EMPTY || value == CELL_X || value == CELL_O)

// Range check — only works because values happen to be 0, 1, 2
(value >= CELL_EMPTY && value <= CELL_O)
```

Prefer the explicit form unless you know the values are a contiguous range and that will never
change.

</details>

---

### Exercise 6: Complex Condition
Write a boolean expression that evaluates to `1` when all of the following are true:
- The game status is `ONGOING`
- The cell at `position` is `CELL_EMPTY`
- Either the position is `4` (center) OR the player type is `PLAYER_HUMAN`

<details>
<summary>Click to reveal answer</summary>

```c
int condition = (status == ONGOING) &&
                (board[position] == CELL_EMPTY) &&
                (position == 4 || player == PLAYER_HUMAN);
```

The inner `||` must be parenthesised because `&&` binds more tightly than `||`. Without them,
the expression would be parsed as `(... && position == 4) || (player == PLAYER_HUMAN)` — a
completely different condition.

</details>

---

### Exercise 7: Which Constant Tool?
For each of the following, state which constant tool you would use — `#define`, `const`, or `enum`
— and briefly explain why.

1. The number of rows on the game board (used to declare an array)
2. The character `'-'` used to display an empty cell
3. The four possible outcomes of a game: win, lose, draw, ongoing
4. The mathematical constant π (3.14159…)
5. A flag that enables extra debug logging when defined at build time

<details>
<summary>Click to reveal answer</summary>

1. **`#define`** — array sizes must be compile-time constants; `const int` doesn't qualify in C89/C90
2. **`const char`** — it's a single non-integer value that benefits from a type and debugger visibility
3. **`enum`** — a closed set of related integer states; bundled under one type name for safety
4. **`#define`** (or `const double`) — enums can only hold integers; `#define PI 3.14159` is the traditional form, though `const double PI = 3.14159` is safer
5. **`#define`** — only `#define` supports `#ifdef`/`#ifndef` conditional compilation; a `const` variable can't be tested with the preprocessor

</details>

---

### Exercise 8: Reading `constants.h`
Look at the snippet from `include/constants.h` below and answer the questions underneath it.

```c
#define BOARD_DIM  3
#define BOARD_SIZE (BOARD_DIM * BOARD_DIM)
#define WIN_LENGTH BOARD_DIM

extern const char X_MARKER;
extern const char O_MARKER;
extern const char EMPTY_MARKER;

typedef enum { CELL_EMPTY = 0, CELL_X = 1, CELL_O = 2 } CellValue;
```

1. What value does `BOARD_SIZE` expand to, and at what stage of compilation does that happen?
2. Why is `X_MARKER` declared with `extern` in the header but defined in `constants.c`?
3. Why isn't `X_MARKER` a `#define` instead?
4. What would break if `CELL_X` were changed from `1` to `10`?

<details>
<summary>Click to reveal answer</summary>

1. `BOARD_SIZE` expands to `(3 * 3)` = `9`. This happens at the **preprocessing** stage, before
   the compiler runs. The compiler only ever sees the number `9`.

2. `extern` means "this variable is defined somewhere else — trust me, it exists". The header
   declares it so any file that `#include`s the header knows its type; `constants.c` provides the
   single actual definition with the value `'X'`. This avoids duplicate-definition linker errors
   (covered fully in [Lesson 7: Header Files](LESSON_7_Header_Files.md)).

3. `X_MARKER` is a `char`, not an integer, and it benefits from type checking. A `#define 'X'`
   would work mechanically, but the debugger would show `88` (ASCII for `'X'`) instead of the
   variable name. `const char` is safer and more readable.

4. Code that uses the arithmetic trick `3 - player` to get the opponent's symbol would break,
   because `3 - 10 = -7`, not `CELL_O`. The ternary form `(player == CELL_X) ? CELL_O : CELL_X`
   would still work because it compares by name, not by value.

</details>

---

### Exercise 9: Spot the `#define` Bugs
Each snippet below contains a `#define` mistake. Identify the bug and write the corrected version.

```c
// (a)
#define AREA(w, h)  w * h
int a = AREA(2 + 1, 4);    // expects 12

// (b)
#define BOARD_SIZE 9
#define HALF_BOARD BOARD_SIZE / 2   // expects 4
int h = HALF_BOARD + 1;    // expects 5, gets what?

// (c)
#define MAX_PLAYERS 2
#define MAX_PLAYERS 4      // updated limit
```

<details>
<summary>Click to reveal answer</summary>

**(a) Missing parentheses around parameters:**
```c
// Bug: AREA(2+1, 4) expands to  2 + 1 * 4 = 6  (not 12!)
#define AREA(w, h)  w * h

// Fix: parenthesise every parameter and the whole expression
#define AREA(w, h)  ((w) * (h))
// AREA(2+1, 4) expands to  ((2+1) * (4)) = 12  ✅
```

**(b) Missing parentheses around the whole expression:**
```c
// Bug: HALF_BOARD + 1 expands to  BOARD_SIZE / 2 + 1  = 9 / 2 + 1 = 5 ✅
//      but HALF_BOARD * 2 expands to  BOARD_SIZE / 2 * 2 = 9 / 2 * 2 = 8 (not 9!)
#define HALF_BOARD BOARD_SIZE / 2

// Fix: wrap the whole replacement in parentheses
#define HALF_BOARD (BOARD_SIZE / 2)
```

**(c) Redefining a `#define` without `#undef` first:**
```c
// Bug: most compilers warn (or error) on a redefinition with a different value
#define MAX_PLAYERS 2
#define MAX_PLAYERS 4      // ⚠️ redefinition warning

// Fix: undefine before redefining
#define MAX_PLAYERS 2
#undef  MAX_PLAYERS
#define MAX_PLAYERS 4      // ✅ clean redefinition
```

In practice, if you're changing a configuration value, just edit the original `#define` — `#undef`
+ redefine is for situations where a value is set externally (e.g., by a build system) and you
need to override it.

</details>

---

## Summary
In this lesson, you learned:

1. **What enums are**: Named integer constants that make code self-documenting
2. **Enum syntax**: `typedef enum { ... } TypeName;`
3. **How enums work**: Under the hood, they're just integers
4. **Project enums**: `CellValue`, `GameStatus`, `PlayerType`, `Difficulty`
5. **Logical operators with enums**: `&&`, `||`, `!` for complex conditions
6. **Best practices**: Explicit values, descriptive names, complete switch statements
7. **Three constant tools**: `#define` (text substitution, array sizes), `const` (typed, debuggable single values), `enum` (named integer sets) — use the Decision Guide to choose
8. **`#define` mechanics**: Text substitution before compilation — no type, no scope, no debugger visibility; prefer enums for related integer sets

See [include/constants.h](../../include/constants.h) for all enum definitions in this project.

---

## Next Steps

Continue to [Lesson 4: Functions](LESSON_4_Functions.md) →

← Previous: [Lesson 2: Arrays](LESSON_2_Arrays.md)

← Back to [Learning Guide Index](LEARNING_GUIDE.md)

---

## Bonus: Advanced Topics
> **📚 Come back later!** The topics below build on concepts from later lessons.
> Read them now for a preview, or bookmark this section and return after completing:
> - [Lesson 2: Arrays](LESSON_2_Arrays.md) — for understanding array indexing
> - [Lesson 4: Functions](LESSON_4_Functions.md) — for understanding function-like macros

### Using Enum Values as Array Indices
Since enum values are just integers, they can directly index into arrays. This is a powerful
pattern that replaces verbose `if-else` or `switch` chains with simple array lookups.

#### The Basic Idea

Instead of writing:

```c
// Verbose and error-prone
char get_marker(CellValue cell) {
    if (cell == CELL_EMPTY) return ' ';
    if (cell == CELL_X) return 'X';
    if (cell == CELL_O) return 'O';
    return '?';
}
```

You write:

```c
// Clean and fast
const char markers[] = {' ', 'X', 'O'};
//                       0    1    2
//                    EMPTY   X    O

char get_marker(CellValue cell) {
    return markers[cell];  // cell IS the index
}
```

The enum value *is* the array index — no conversion needed.

#### Visual Example
```
CellValue enum:
  CELL_EMPTY = 0
  CELL_X     = 1
  CELL_O     = 2

markers array:
  Index:   [0]   [1]   [2]
  Value:   ' '   'X'   'O'
            ↑     ↑     ↑
         EMPTY   X     O

markers[CELL_X] → markers[1] → 'X'
```

#### When the Enum Doesn't Start at 0
The `CellValue` enum conveniently starts at 0, so `CELL_EMPTY` maps directly to index 0. But
what if your enum starts at 1?

```c
// CELL_X = 1, CELL_O = 2 — no CELL_EMPTY in this array
// We need indices 0 and 1, but CELL_X is 1 and CELL_O is 2

PlayerType players[2];
players[0] = PLAYER_HUMAN;     // X's type
players[1] = PLAYER_COMPUTER;  // O's type

// To look up a player, subtract 1 from the symbol:
CellValue symbol = CELL_X;     // = 1
PlayerType type = players[symbol - 1];  // players[0] → PLAYER_HUMAN
```

This pattern appears in the project's `GameState` struct:

```c
// From include/tictactoe.h
typedef struct {
    Player players[2];  // Index 0 = X, Index 1 = O
    // ...
} GameState;

// Usage:
Player *current = &state->players[symbol - 1];
```

#### Why This Pattern Is Useful
1. **Speed:** Array lookup is O(1) — no conditionals to evaluate
2. **Maintainability:** Add a new enum value? Just add an entry to the array
3. **Clarity:** The mapping is visible in one place, not scattered across `if` statements

---

### Enum for Symbol-to-Character Mapping
This is a specific, common application of the array-indexing pattern: mapping game symbols to
their display characters.

#### The Problem

You need to convert `CellValue` (an enum) to a printable character:

```c
CellValue board[9] = { CELL_X, CELL_EMPTY, CELL_O, ... };

// How do you print this board?
// You need: CELL_X → 'X', CELL_O → 'O', CELL_EMPTY → ' '
```

#### The Solution: Lookup Array
```c
// Define the mapping once
const char cell_to_char[] = {
    ' ',   // CELL_EMPTY = 0 → ' '
    'X',   // CELL_X     = 1 → 'X'
    'O'    // CELL_O     = 2 → 'O'
};

// Use it anywhere
void print_board(CellValue board[9]) {
    for (int i = 0; i < 9; i++) {
        printf("%c ", cell_to_char[board[i]]);
        if ((i + 1) % 3 == 0) printf("\n");
    }
}
```

Output for a sample board:
```
X   O
  X  
O   X
```

#### Extending the Pattern
You can create multiple lookup arrays for different purposes:

```c
// Character representation
const char cell_chars[] = {' ', 'X', 'O'};

// Full names for messages
const char *cell_names[] = {"Empty", "X", "O"};

// Unicode symbols for fancy output
const char *cell_unicode[] = {"·", "✕", "○"};

// Usage:
printf("Cell contains: %s\n", cell_names[board[4]]);
// Output: "Cell contains: X"
```

#### Real Code from This Project
In `src/ui_cli.c`, this pattern is used to display the board:

```c
// The project uses const char variables instead of an array,
// but the principle is the same:
extern const char X_MARKER;      // 'X'
extern const char O_MARKER;      // 'O'
extern const char EMPTY_MARKER;  // '-'

// A lookup array would work identically:
const char markers[] = {EMPTY_MARKER, X_MARKER, O_MARKER};

char display = markers[board[position]];
```

#### Bidirectional Mapping

Sometimes you need to go both ways — character to enum and enum to character:

```c
// Enum → char (easy: array lookup)
const char cell_chars[] = {' ', 'X', 'O'};
char c = cell_chars[CELL_X];  // 'X'

// Char → enum (needs a function or switch)
CellValue char_to_cell(char c) {
    switch (c) {
        case ' ': return CELL_EMPTY;
        case 'X': return CELL_X;
        case 'O': return CELL_O;
        default:  return CELL_EMPTY;  // or handle error
    }
}
```

The enum-to-char direction is O(1) via array lookup. The char-to-enum direction requires a
`switch` or `if` chain because you're searching for a value, not indexing by position.

---

### Counting Enum Values
A common need is knowing how many values an enum has — for array sizes, loop bounds, or validation.
C doesn't have a built-in way to count enum members, but there's a clever idiom.

#### The `_COUNT` Sentinel Pattern

Add a final member that exists only to hold the count:

```c
// Hypothetical example with three difficulty levels
typedef enum {
    DIFFICULTY_EASY,    // = 0
    DIFFICULTY_MEDIUM,  // = 1
    DIFFICULTY_HARD,    // = 2
    DIFFICULTY_COUNT    // = 3 — not a real difficulty, just the count!
} Difficulty;
```

Since enum values auto-increment from 0, the last member's value equals the number of "real"
members before it. `DIFFICULTY_COUNT` is 3, and there are exactly 3 difficulties.

> **Note:** The actual project's `Difficulty` enum only has two values (`EASY` and `HARD`).
> This example adds a hypothetical `MEDIUM` to better illustrate the counting pattern.

#### Why This Works

```
Index:    0              1               2              3
Value:    DIFFICULTY_EASY  DIFFICULTY_MEDIUM  DIFFICULTY_HARD  DIFFICULTY_COUNT
                                                              ↑
                                                         This equals 3,
                                                         which is the count
                                                         of real values
```

The pattern relies on:
1. Starting at 0 (the default)
2. No gaps in the numbering
3. The `_COUNT` member being last

#### Practical Uses

**1. Declaring correctly-sized arrays:**

```c
// Array with one slot per difficulty
const char *difficulty_names[DIFFICULTY_COUNT] = {
    "Easy",
    "Medium", 
    "Hard"
};

// The compiler ensures the array has exactly 3 elements
```

**2. Loop bounds:**

```c
// Iterate over all difficulties
for (int i = 0; i < DIFFICULTY_COUNT; i++) {
    printf("Level %d: %s\n", i, difficulty_names[i]);
}
```

**3. Input validation:**

```c
int is_valid_difficulty(int value) {
    return value >= 0 && value < DIFFICULTY_COUNT;
}
```

**4. Random selection:**

```c
Difficulty random_difficulty(void) {
    return rand() % DIFFICULTY_COUNT;  // Returns 0, 1, or 2
}
```

#### Applying It to the Project's Enums
The project's enums don't use this pattern, but they could:

```c
// Current:
typedef enum { CELL_EMPTY = 0, CELL_X = 1, CELL_O = 2 } CellValue;

// With count sentinel:
typedef enum {
    CELL_EMPTY = 0,
    CELL_X = 1,
    CELL_O = 2,
    CELL_COUNT  // = 3
} CellValue;

// Now you can write:
const char markers[CELL_COUNT] = {' ', 'X', 'O'};
```

#### When This Pattern Breaks

The `_COUNT` trick only works when values are contiguous starting from 0:

```c
// ❌ BROKEN: gaps in values
typedef enum {
    ERROR_NONE = 0,
    ERROR_FILE = 100,    // jumped to 100!
    ERROR_NETWORK = 200, // jumped to 200!
    ERROR_COUNT          // = 201, NOT 3!
} ErrorCode;

// ❌ BROKEN: doesn't start at 0
typedef enum {
    HTTP_OK = 200,
    HTTP_NOT_FOUND = 404,
    HTTP_ERROR = 500,
    HTTP_COUNT  // = 501, NOT 3!
} HttpStatus;
```

For non-contiguous enums, you must track the count manually:

```c
#define HTTP_STATUS_COUNT 3  // manually maintained

// Or use a separate constant at the end:
typedef enum {
    HTTP_OK = 200,
    HTTP_NOT_FOUND = 404,
    HTTP_ERROR = 500
} HttpStatus;
#define HTTP_STATUS_COUNT 3
```

#### Naming Conventions

Common suffixes for the count sentinel:

| Suffix   | Example            | Notes                                     |
|----------|--------------------|-------------------------------------------|
| `_COUNT` | `DIFFICULTY_COUNT` | Most common, clear intent                 |
| `_NUM`   | `DIFFICULTY_NUM`   | Alternative                               |
| `_MAX`   | `DIFFICULTY_MAX`   | Can be confusing (is it inclusive?)       |
| `_LAST`  | `DIFFICULTY_LAST`  | Usually means last valid value, not count |

Stick with `_COUNT` for clarity — it says exactly what it is.

--- 

### `#define` Deep Dive — Mechanics and Pitfalls
`#define` is a **preprocessor directive** — an instruction to the C preprocessor, which runs
*before* the compiler ever sees your code. Its job is simple: find every occurrence of a name and
replace it with the given text, mechanically, like a find-and-replace in a text editor.

```c
#define BOARD_SIZE 9
```

After preprocessing, every mention of `BOARD_SIZE` in the file is replaced with the literal text
`9`. The compiler never sees the name `BOARD_SIZE` at all — only the number.

```c
// What you write:
int board[BOARD_SIZE];
for (int i = 0; i < BOARD_SIZE; i++) { ... }

// What the compiler sees:
int board[9];
for (int i = 0; i < 9; i++) { ... }
```

#### The Three Common Uses of `#define`
**1. Named constants** — give a magic number a meaningful name:

```c
#define BOARD_DIM  3
#define BOARD_SIZE (BOARD_DIM * BOARD_DIM)   // expressions work too
#define WIN_LENGTH BOARD_DIM
```

**2. Conditional compilation** — include or exclude blocks of code at build time:

```c
#define DEBUG_MODE

#ifdef DEBUG_MODE
    printf("board state: ...\n");   // only compiled when DEBUG_MODE is defined
#endif
```

**3. Function-like macros** — inline code fragments (use sparingly):

```c
#define MAX(a, b)  ((a) > (b) ? (a) : (b))

int biggest = MAX(score_x, score_o);
// Expands to: int biggest = ((score_x) > (score_o) ? (score_x) : (score_o));
```

#### `#define` Has No Type
This is its most important characteristic. A `#define` constant is pure text — the preprocessor
substitutes it before the compiler runs, so the compiler has no idea a name ever existed.

```c
#define CELL_X 1

CellValue cell = CELL_X;   // preprocessor rewrites this to: CellValue cell = 1;
```

That means:

- No type checking — the compiler can't warn you if you use `CELL_X` where a `GameStatus` was expected
- No debugger visibility — stepping through code in a debugger shows `1`, not `CELL_X`
- No scope — `#define` applies from the point of definition to the end of the file (or until `#undef`)
- No symbol in the compiled output — it genuinely vanishes before compilation

#### Common `#define` Pitfalls
**Missing parentheses in expressions:**

```c
#define DOUBLE(x)  x * 2          // ❌ dangerous!
int result = DOUBLE(3 + 1);
// Expands to: 3 + 1 * 2 = 5  (not 8!)

#define DOUBLE(x)  ((x) * 2)      // ✅ always parenthesise
int result = DOUBLE(3 + 1);
// Expands to: ((3 + 1) * 2) = 8
```

**Accidental double evaluation:**

```c
#define MAX(a, b)  ((a) > (b) ? (a) : (b))

int i = 5;
int m = MAX(i++, 3);   // ⚠️ i++ is evaluated TWICE → i incremented twice!
```

**No type information for the debugger:**

```c
#define CELL_X 1

// Debugger shows:  cell = 1
// You have to remember what 1 means

CellValue cell = CELL_X;
// With enum, debugger shows:  cell = CELL_X   ← much more readable
```

#### `#define` vs. `enum` for Named Integer Sets
You might see code using `#define` instead of enums for groups of related constants:

```c
// Using #define
#define CELL_EMPTY 0
#define CELL_X     1
#define CELL_O     2

// Using enum
typedef enum {
    CELL_EMPTY = 0,
    CELL_X     = 1,
    CELL_O     = 2
} CellValue;
```

**Why enums are usually better for groups of related values:**

| Feature               | `#define`                        | `enum`                       |
|-----------------------|----------------------------------|------------------------------|
| Type safety           | ❌ Just text substitution         | ✅ Has a named type           |
| Debugger visibility   | ❌ Shows the raw number           | ✅ Shows the constant's name  |
| Scope                 | ❌ Always file-global             | ✅ Can be limited             |
| Groups related values | ❌ No grouping — separate lines   | ✅ All values in one block    |
| Compiler warnings     | ❌ No warning for wrong values    | ⚠️ Compiler may warn         |

#### When `#define` Is Still the Right Choice
Enums aren't always better. Reach for `#define` when:

- **The constant isn't an integer** — `#define PI 3.14159` (enums can only be integers)
- **Conditional compilation** — `#ifdef ENABLE_AI` has no enum equivalent
- **Computed size constants** — `#define BOARD_SIZE (BOARD_DIM * BOARD_DIM)` works before any
  type system is involved; an enum constant can't reference another constant this cleanly
- **Cross-language interfaces** — header files shared with C++ or assembly may rely on `#define`

---


### Advanced Exercises
#### Exercise: Enum as Array Index

The following code has a bug. Can you spot and fix it?

```c
const char *status_messages[] = {"Playing", "X Wins", "O Wins", "Draw"};

void print_status(GameStatus status) {
    printf("%s\n", status_messages[status + 1]);
}
```

<details>
<summary>Click to reveal answer</summary>

The bug is the `+ 1` in `status_messages[status + 1]`.

Since `ONGOING = 0`, `X_WINS = 1`, `O_WINS = 2`, `DRAW = 3`, and the array indices are 0, 1, 2, 3, the enum values already match the array indices directly.

**Fixed code:**
```c
const char *status_messages[] = {"Playing", "X Wins", "O Wins", "Draw"};

void print_status(GameStatus status) {
    printf("%s\n", status_messages[status]);  // No +1 needed!
}
```

</details>


#### Exercise: State Machine (Challenge)
Design an enum for a simple game state machine with the following states:
- `MENU` (value 0) - Main menu
- `PLAYING` - Active gameplay
- `PAUSED` - Game paused
- `GAME_OVER` - Game ended
- `QUIT` - Exit the game

Then write a function `can_transition_to_playing` that returns `1` if the current state allows transitioning to `PLAYING` (only from `MENU` or `PAUSED`).

<details>
<summary>Click to reveal answer</summary>

```c
typedef enum {
    STATE_MENU = 0,
    STATE_PLAYING,
    STATE_PAUSED,
    STATE_GAME_OVER,
    STATE_QUIT
} GameState;

int can_transition_to_playing(GameState current) {
    return (current == STATE_MENU || current == STATE_PAUSED);
}

// More complete version with additional validation:
int can_start_game(GameState current) {
    // Can only start playing from menu or resume from paused
    if (current == STATE_MENU || current == STATE_PAUSED) {
        return 1;
    }
    return 0;
}
```

</details>
