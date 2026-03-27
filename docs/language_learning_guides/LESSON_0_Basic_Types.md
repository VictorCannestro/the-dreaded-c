# Lesson 0: Bits and Bytes, Types and Operators
Welcome to C! In this lesson, you'll learn how C stores different kinds of data and how to work with that data using 
operators. We'll start simple and build up your understanding piece by piece.


## Table of Contents
### Core Concepts
**Foundations**
0. [Bits and Bytes — The Foundation of Everything](#0-bits-and-bytes--the-foundation-of-everything)

**Types**
1. [What Is a Type?](#1-what-is-a-type) — memory, interpretation, valid operations
2. [Integer Types](#2-integer-types) — `char`, `short`, `int`, `long`; signed vs. unsigned
3. [Floating-Point Types](#3-floating-point-types) — `float` vs. `double`; precision pitfalls
4. [The `void` Type](#4-the-void-type) — "nothing" type for functions
5. [Boolean Values in C](#5-boolean-values-in-c) — `0`/non-zero, `<stdbool.h>`

**Operators**
6. [Arithmetic Operators](#6-arithmetic-operators) — `+`, `-`, `*`, `/`, `%`; integer division; `++`/`--`
7. [Comparison Operators](#7-comparison-operators) — `==`, `!=`, `<`, `>`; the `=` vs `==` trap
8. [Logical Operators](#8-logical-operators) — `&&`, `||`, `!`; short-circuit evaluation
9. [The Ternary Operator](#9-the-ternary-operator) — `cond ? a : b`; when to use it
10. [Combining Operators](#10-combining-operators) — precedence rules; when to add parentheses
11. [Type Conversions](#11-type-conversions) — implicit promotion, explicit casts

### Practice
12. [Practical Exercises](#practical-exercises)
13. [Summary](#summary)

### Going Further
14. [Next Steps](#next-steps)
15. [Bonus: Advanced Topics](#bonus-advanced-topics) *(return after Lessons 2 & 4)*
    - Guaranteed-Size Types (`<stdint.h>`)
    - Pre vs. Post Increment in Complex Expressions
    - The Comma Operator
    - Bitwise Operations

---

## 0. Bits and Bytes — The Foundation of Everything

Before talking about types, you need a mental model of what a computer actually stores. Every variable, every
instruction, every piece of data in any program ultimately lives in memory as a sequence of **bits**.

### What Is a Bit?

A **bit** (binary digit) is the smallest unit of information a computer can store. It has exactly two possible
values: `0` or `1`. Think of it as a light switch — on or off, nothing in between.

```
Bit examples:
  0    →  off, false, no
  1    →  on,  true,  yes
```

### What Is a Byte?

A **byte** is a group of **8 bits** held together. It's the smallest unit of memory that C can address directly.

```
One byte, eight bits:
  ┌───┬───┬───┬───┬───┬───┬───┬───┐
  │ 0 │ 1 │ 1 │ 0 │ 0 │ 1 │ 0 │ 1 │
  └───┴───┴───┴───┴───┴───┴───┴───┘
    7   6   5   4   3   2   1   0     ← bit position (right = lowest)
```

8 bits can represent 2⁸ = **256** different values (0 through 255).

### From Bits to Numbers

Each bit position represents a power of 2. The rightmost bit is worth 2⁰ = 1, the next is 2¹ = 2, then 2² = 4, and
so on. To find the value of a byte, add up the powers of 2 wherever a `1` appears:

```
  0 1 1 0 0 1 0 1
  │ │ │ │ │ │ │ └─  1 × 2⁰ =   1
  │ │ │ │ │ │ └───  0 × 2¹ =   0
  │ │ │ │ │ └─────  1 × 2² =   4
  │ │ │ │ └───────  0 × 2³ =   0
  │ │ │ └─────────  0 × 2⁴ =   0
  │ │ └───────────  1 × 2⁵ =  32
  │ └─────────────  1 × 2⁶ =  64
  └───────────────  0 × 2⁷ =   0
                            ─────
                              101  ← decimal value
```

### Common Sizes

| Unit     | Bits | Distinct values | Unsigned range   |
|----------|------|-----------------|------------------|
| 1 byte   | 8    | 256             | 0 – 255          |
| 2 bytes  | 16   | 65,536          | 0 – 65,535       |
| 4 bytes  | 32   | ~4.3 billion    | 0 – 4,294,967,295|
| 8 bytes  | 64   | ~18.4 quintillion | 0 – 2⁶⁴−1     |

### How C Types Map to Bytes

Every C type occupies a fixed number of bytes. That byte count determines both the *range* of values it can hold and
*how much memory* each variable consumes:

```
char     →  1 byte   (8 bits)
short    →  2 bytes  (16 bits)
int      →  4 bytes  (32 bits)   ← most common
long     →  4–8 bytes
float    →  4 bytes  (32 bits)
double   →  8 bytes  (64 bits)
```

A single `int` uses 4 bytes = 32 bits, which gives 2³² = 4,294,967,296 possible bit patterns. For a *signed* `int`,
half of those represent negative numbers, giving a range of roughly −2.1 billion to +2.1 billion.

### Why This Matters for C

Most languages hide all of this. C does not. Because:
- You choose the type, you choose how many bytes are used
- Integer overflow, truncation, and bitwise bugs all come back to this model
- Understanding bits and bytes makes the rest of this lesson — and everything that follows — click into place

> **You'll revisit this** when you reach the Bitwise Operations section in More Advanced Topics, where you'll
> manipulate individual bits directly using C's bitwise operators.

---

## 1. What Is a Type?
Every variable in C has a **type** that tells the compiler:
- **How much memory** to allocate
- **How to interpret** the bits stored in that memory
- **What operations** are valid

Think of types like different containers: a shot glass, a coffee mug, and a bucket can all hold liquid, but they have 
different capacities and uses.

---

## 2. Integer Types
Integers are whole numbers (no decimal points). C provides several sizes:

| Type    | Size (typical) | Range                           | Example Use             |
|---------|----------------|---------------------------------|-------------------------|
| `char`  | 1 byte         | -128 to 127                     | Characters, tiny counts |
| `short` | 2 bytes        | -32,768 to 32,767               | Memory-constrained apps |
| `int`   | 4 bytes        | -2,147,483,648 to 2,147,483,647 | Most integer needs      |
| `long`  | 4-8 bytes      | At least same as `int`          | Large numbers           |

### Basic Integer Examples
```c
int move_count = 0;         // How many moves have been played
int position = 4;           // Center of a 3x3 board (0-8)
char marker = 'X';          // A character is just a small integer!
```

### Signed vs. Unsigned
By default, integers are **signed** (can be negative). Add `unsigned` for non-negative only:

```c
int temperature = -5;           // Signed: can be negative
unsigned int age = 25;          // Unsigned: 0 to 4,294,967,295

// Useful when negative values don't make sense
unsigned int board_position = 3;  // Board positions are never negative
```

**⚠️ Warning:** Mixing signed and unsigned can cause subtle bugs:

```c
unsigned int a = 5;
int b = -1;

if (b < a) {
    // You might expect this to run, but it doesn't!
    // -1 is converted to a huge unsigned number
}
```

---

## 3. Floating-Point Types
For numbers with decimal points:

| Type     | Size    | Precision     | Example Use          |
|----------|---------|---------------|----------------------|
| `float`  | 4 bytes | ~6-7 digits   | Graphics, games      |
| `double` | 8 bytes | ~15-16 digits | Scientific computing |

```c
float win_percentage = 75.5f;    // The 'f' suffix makes it a float literal
double pi = 3.14159265358979;    // More precision
double score = 95.5;             // Doubles are the default for decimal literals
```

### Floating-Point Pitfall
Floating-point numbers can't represent all values exactly:

```c
float result = 0.1f + 0.2f;
// result is NOT exactly 0.3! It's 0.30000001192...

// DON'T compare floats with ==
if (result == 0.3f) { }  // ❌ Dangerous!

// DO use a tolerance
if (result > 0.29f && result < 0.31f) { }  // ✅ Safe
```

---

## 4. The `void` Type
`void` means "nothing" or "no type." It has two main uses:

```c
// 1. Function returns nothing
void board_init(CellValue board[BOARD_SIZE]) {
    // Sets up the board, doesn't return a value
}

// 2. Function takes no parameters
int get_board_size(void) {
    return BOARD_SIZE;
}
```

---

## 5. Boolean Values in C
C89/C90 has **no built-in boolean type**. Instead, integers represent truth:

| Value  | Meaning |
|--------|---------|
| `0`    | False   |
| Non-zero | True  |

```c
int game_over = 0;          // False
int is_valid = 1;           // True (any non-zero works)
int also_true = -42;        // Also true! (non-zero)
```

### Modern C (C99+) Boolean
C99 added `<stdbool.h>` for cleaner code:

```c
#include <stdbool.h>

bool game_over = false;
bool is_valid = true;

if (is_valid) {
    // This reads more naturally
}
```

**In this project:** We use `int` with 0/1 for simplicity and compatibility:

```c
// From src/board.c
int board_is_empty(const CellValue board[BOARD_SIZE], int position) {
    return board[position] == CELL_EMPTY;  // Returns 1 (true) or 0 (false)
}
```

---

## 6. Arithmetic Operators
C supports the standard math operations:

| Operator | Name           | Example      | Result |
|----------|----------------|--------------|--------|
| `+`      | Addition       | `5 + 3`      | `8`    |
| `-`      | Subtraction    | `5 - 3`      | `2`    |
| `*`      | Multiplication | `5 * 3`      | `15`   |
| `/`      | Division       | `5 / 3`      | `1`    |
| `%`      | Modulo         | `5 % 3`      | `2`    |

### Integer Division Truncates!
```c
int result = 5 / 3;     // result is 1, NOT 1.666...
int remainder = 5 % 3;  // remainder is 2

// To get a decimal result, at least one operand must be floating-point
double proper = 5.0 / 3;  // proper is 1.666...
```

### Compound Assignment Operators
Shorthand for modifying a variable:

```c
int count = 10;

count += 5;   // Same as: count = count + 5;  (now 15)
count -= 3;   // Same as: count = count - 3;  (now 12)
count *= 2;   // Same as: count = count * 2;  (now 24)
count /= 4;   // Same as: count = count / 4;  (now 6)
count %= 4;   // Same as: count = count % 4;  (now 2)
```

### Increment and Decrement
```c
int move = 5;

move++;   // Increment: move is now 6
move--;   // Decrement: move is now 5

// Pre vs. Post (matters when used in expressions!)
int a = 5;
int b = a++;  // b = 5, then a becomes 6 (post-increment)

int c = 5;
int d = ++c;  // c becomes 6, then d = 6 (pre-increment)
```

---

## 7. Comparison Operators
Comparison operators return `1` (true) or `0` (false):

| Operator | Meaning               | Example    | Result |
|----------|-----------------------|------------|--------|
| `==`     | Equal to              | `5 == 5`   | `1`    |
| `!=`     | Not equal to          | `5 != 3`   | `1`    |
| `<`      | Less than             | `5 < 3`    | `0`    |
| `>`      | Greater than          | `5 > 3`    | `1`    |
| `<=`     | Less than or equal    | `5 <= 5`   | `1`    |
| `>=`     | Greater than or equal | `3 >= 5`   | `0`    |

### Common Mistake: `=` vs `==`
```c
int x = 5;

// WRONG: Assignment, not comparison!
if (x = 3) {    // This ASSIGNS 3 to x, then checks if 3 is non-zero (true)
    // Always runs!
}

// CORRECT: Comparison
if (x == 3) {   // Checks if x equals 3
    // Only runs if x is actually 3
}
```

**Pro tip:** Some programmers write `3 == x` so that `3 = x` causes a compiler error.

---

## 8. Logical Operators
Logical operators combine boolean expressions:

| Operator | Name | Description                           | Example           | Result |
|----------|------|---------------------------------------|-------------------|--------|
| `&&`     | AND  | True if BOTH operands are true        | `1 && 1`          | `1`    |
| `\|\|`   | OR   | True if EITHER operand is true        | `0 \|\| 1`        | `1`    |
| `!`      | NOT  | Inverts the truth value               | `!0`              | `1`    |

### Truth Tables

**AND (`&&`):**

| A | B | A && B |
|---|---|--------|
| 0 | 0 | 0      |
| 0 | 1 | 0      |
| 1 | 0 | 0      |
| 1 | 1 | 1      |

**OR (`||`):**

| A | B | A \|\| B |
|---|---|----------|
| 0 | 0 | 0        |
| 0 | 1 | 1        |
| 1 | 0 | 1        |
| 1 | 1 | 1        |

### Real-World Examples from This Project
```c
// From src/board.c - Checking if a move is valid
int board_is_valid_move(const CellValue board[BOARD_SIZE], int position) {
    // Position must be in range AND the cell must be empty
    if (position < 0 || position >= BOARD_SIZE) {
        return 0;  // Invalid: out of bounds
    }
    return board_is_empty(board, position);  // Valid only if cell is empty
}
```

```c
// Checking if the game should continue
int game_should_continue = (status == ONGOING) && (move_count < BOARD_SIZE);

// Checking if it's a player's turn (either human OR computer)
int is_players_turn = (current_player == CELL_X) || (current_player == CELL_O);

// Checking if a cell is NOT empty
int cell_occupied = !board_is_empty(board, position);
```

### Short-Circuit Evaluation
C **stops evaluating** as soon as the result is known:

```c
// If position is invalid, board[position] is NEVER accessed!
if (position >= 0 && position < 9 && board[position] == CELL_EMPTY) {
    // Safe: we only check board[position] after validating position
}

// With OR, stops at first true
if (status == X_WINS || status == O_WINS) {
    // If X wins, we don't bother checking if O wins
}
```

**This is powerful for avoiding errors:**

```c
// Safe: NULL check happens first
if (ptr != NULL && ptr->value > 10) {
    // Only access ptr->value if ptr is not NULL
}

// DANGEROUS: might crash!
if (ptr->value > 10 && ptr != NULL) {
    // Accesses ptr->value BEFORE checking if ptr is NULL!
}
```

---

## 9. The Ternary Operator
The **ternary operator** `?:` is a compact way to write simple `if-else` statements. It's the only operator in C that takes three operands:

```c
condition ? value_if_true : value_if_false
```

### Basic Usage
```c
int score = 85;
char *grade = (score >= 60) ? "Pass" : "Fail";
// grade is "Pass"

int a = 5, b = 3;
int max = (a > b) ? a : b;  // max is 5

// Equivalent to:
int max_verbose;
if (a > b) {
    max_verbose = a;
} else {
    max_verbose = b;
}
```

### Real-World Examples from This Project
```c
// Display X, O, or empty space
char display_char = (cell == CELL_X) ? 'X' : 
                    (cell == CELL_O) ? 'O' : ' ';

// Switch player after a move
CellValue next_player = (current == CELL_X) ? CELL_O : CELL_X;

// Get absolute value
int abs_value = (x < 0) ? -x : x;
```

### Nesting Ternary Operators
You can nest ternary operators, but use caution—it can get confusing:

```c
// Nested ternary (harder to read)
int sign = (x > 0) ? 1 : (x < 0) ? -1 : 0;

// Same logic with if-else (clearer for complex cases)
int sign_clear;
if (x > 0) {
    sign_clear = 1;
} else if (x < 0) {
    sign_clear = -1;
} else {
    sign_clear = 0;
}
```

### Ternary in Print Statements
Very useful for conditional output:

```c
int count = 1;
printf("You have %d item%s\n", count, (count == 1) ? "" : "s");
// Output: "You have 1 item"

count = 5;
printf("You have %d item%s\n", count, (count == 1) ? "" : "s");
// Output: "You have 5 items"
```

### When to Use Ternary
**✅ Good use cases:**
- Simple value selection
- Inline conditionals in expressions
- Return statements with simple conditions

```c
return (is_valid) ? SUCCESS : ERROR;
```

**❌ Avoid when:**
- Logic is complex or nested deeply
- Multiple statements are needed
- Readability suffers

```c
// DON'T do this
int result = (a > b) ? (c > d) ? (e > f) ? 1 : 2 : 3 : 4;  // ❌ Unreadable!
```

---

## 10. Combining Operators
Operators have **precedence** (order of evaluation). When in doubt, use parentheses!

```c
// Arithmetic happens before comparison
int result = 5 + 3 > 6;   // Same as: (5 + 3) > 6 → 8 > 6 → 1

// Comparison happens before logical
int both = 5 > 3 && 2 < 4;  // Same as: (5 > 3) && (2 < 4) → 1 && 1 → 1

// When in doubt, be explicit!
int clear = ((5 + 3) > 6) && ((2 * 2) == 4);
```

### Precedence Summary (highest to lowest)
1. `()` (parentheses)
2. `++`, `--` (postfix), `!`, `~` (logical/bitwise NOT)
3. `++`, `--` (prefix), `+`, `-` (unary)
4. `*`, `/`, `%` (multiplication, division, modulo)
5. `+`, `-` (addition, subtraction)
6. `<<`, `>>` (bit shifts)
7. `<`, `<=`, `>`, `>=` (relational comparison)
8. `==`, `!=` (equality)
9. `&` (bitwise AND)
10. `^` (bitwise XOR)
11. `|` (bitwise OR)
12. `&&` (logical AND)
13. `||` (logical OR)
14. `?:` (ternary conditional)
15. `=`, `+=`, `-=`, etc. (assignment)
16. `,` (comma operator)

---

## 11. Type Conversions
C automatically converts between types in some situations:

```c
int a = 5;
double b = 2.5;
double result = a + b;  // 'a' is converted to double, result is 7.5

// Be careful with integer division!
double wrong = 5 / 3;       // 1.0 (integer division, then converted)
double right = 5.0 / 3;     // 1.666... (double division)
double also_right = (double)5 / 3;  // Cast forces double division
```

### Explicit Casting
Use `(type)` to explicitly convert:

```c
int total = 17;
int count = 5;

// Integer division gives 3
int avg_int = total / count;

// Cast to get the real average
double avg_double = (double)total / count;  // 3.4

// Converting between integer sizes
int big = 300;
char small = (char)big;  // Warning: loses data! (300 doesn't fit in char)
```

---

## Practical Exercises
These exercises only use concepts from this lesson: types, variables, arithmetic, comparison, logical, ternary, and type conversion operators. Exercises that require running code say so explicitly — the rest are pencil-and-paper predictions.

> **Note:** Some exercises reference arrays, functions, or `if` statements as context. You don't need to write them yourself yet — just read the surrounding code and focus on the expression being asked about.

---

### Exercise 1: Type Sizes
Write a program called `sizes.c` that prints the size of each basic type on your system:

```c
#include <stdio.h>

int main(void) {
    printf("Size of char:   %zu bytes\n", sizeof(char));
    printf("Size of int:    %zu bytes\n", sizeof(int));
    printf("Size of long:   %zu bytes\n", sizeof(long));
    printf("Size of float:  %zu bytes\n", sizeof(float));
    printf("Size of double: %zu bytes\n", sizeof(double));
    return 0;
}
```

**Compile and run:** `gcc -o sizes sizes.c && ./sizes`

**Questions:**
1. Is your `int` 2 or 4 bytes?
2. Is your `long` the same size as your `int`?

<details>
<summary>Click to reveal typical answers</summary>

On most modern 64-bit systems:
```
Size of char:   1 bytes
Size of int:    4 bytes
Size of long:   8 bytes
Size of float:  4 bytes
Size of double: 8 bytes
```

**Answers:**
1. Your `int` is most likely **4 bytes** on modern systems (it was 2 bytes on old 16-bit systems).
2. On **macOS/Linux 64-bit**: `long` is 8 bytes (larger than `int`). On **Windows 64-bit**: `long` is 4 bytes (same as `int`).

</details>

---

### Exercise 2: Integer Division vs. Floating-Point
Predict the output of each `printf`, then verify by running the code:

```c
#include <stdio.h>

int main(void) {
    int a = 7;
    int b = 3;

    printf("7 / 3 = %d\n", a / b);
    printf("7 %% 3 = %d\n", a % b);
    printf("7.0 / 3 = %f\n", 7.0 / 3);
    printf("(double)7 / 3 = %f\n", (double)a / b);

    return 0;
}
```

**Questions:**
1. Why is `7 / 3` equal to `2` and not `2.333...`?
2. What does the modulo operator `%` give you?

<details>
<summary>Click to reveal answers</summary>

**Output:**
```
7 / 3 = 2
7 % 3 = 1
7.0 / 3 = 2.333333
(double)7 / 3 = 2.333333
```

**Answers:**
1. `7 / 3` equals `2` because **integer division truncates** (discards the fractional part). Both operands are `int`, so the result is `int`. To get `2.333...`, at least one operand must be a floating-point type.
2. The modulo operator `%` gives you the **remainder** after integer division. `7 = 3 * 2 + 1`, so the remainder is `1`.

</details>

---

### Exercise 3: Boolean Logic
What does each expression evaluate to? Predict *all eight* results before checking.

```c
int x = 5;
int y = 3;
int z = 0;

x > y           // ?
x < y           // ?
x == 5          // ?
z               // ? (used as a boolean)
!z              // ?
x > y && y > z  // ?
x < y || y > z  // ?
!(x == y)       // ?
```

<details>
<summary>Click to reveal answers</summary>

```
x > y           →  1   (5 > 3 is true)
x < y           →  0   (5 < 3 is false)
x == 5          →  1   (5 == 5 is true)
z               →  0   (zero is false)
!z              →  1   (NOT false is true)
x > y && y > z  →  1   (1 && 1)
x < y || y > z  →  1   (0 || 1)
!(x == y)       →  1   (!(5 == 3) → !0)
```

</details>

---

### Exercise 4: Short-Circuit Evaluation
Answer these questions *without running any code* — use only what you know about `&&` and `||`.

Given:
```c
int a = 1;
int b = 0;
int c = 1;
```

For each expression below, state:
- What it evaluates to (`0` or `1`)
- Which operands C actually **reads** before stopping

```c
a && b && c   // (a)
b || a || c   // (b)
a || c        // (c)
b && c        // (d)
```

<details>
<summary>Click to reveal answers</summary>

**(a) `a && b && c`**
- C reads `a` (1 — truthy, keep going), then `b` (0 — falsy, **stop**).
- `c` is **never read**.
- Result: **0**

**(b) `b || a || c`**
- C reads `b` (0 — falsy, keep going), then `a` (1 — truthy, **stop**).
- `c` is **never read**.
- Result: **1**

**(c) `a || c`**
- C reads `a` (1 — truthy, **stop immediately**).
- `c` is **never read**.
- Result: **1**

**(d) `b && c`**
- C reads `b` (0 — falsy, **stop immediately**).
- `c` is **never read**.
- Result: **0**

**Why this matters:** In real code, the "never read" operand is often an expression with a side effect (like accessing memory or calling a function). Short-circuit evaluation guarantees that side effect is skipped — which is how null-pointer guards like `ptr != NULL && ptr->value > 0` are made safe. You'll see this pattern constantly once you reach [Lesson 5: Pointers](LESSON_5_Pointers.md).

</details>

---

### Exercise 5: Ternary Operator — Reading and Writing
This exercise has two parts.

**Part A — Reading:** What does each expression evaluate to?

```c
int score = 72;
int passing = 60;

score >= passing ? 1 : 0                              // (a)
score >= 90 ? 'A' : score >= 60 ? 'B' : 'F'          // (b)
score != 0 ? score : -1                               // (c)
!(score >= passing) ? 0 : score                       // (d)
```

**Part B — Writing:** Rewrite each `if-else` as a single ternary expression:

```c
// (e)
int abs_val;
if (score < 0) { abs_val = -score; } else { abs_val = score; }

// (f)
int clamped;
if (score > 100) { clamped = 100; } else { clamped = score; }
```

<details>
<summary>Click to reveal answers</summary>

**Part A:**
- **(a)** `72 >= 60 ? 1 : 0` → **1**
- **(b)** `72 >= 90` is false → check `72 >= 60` which is true → **`'B'`** (66)
- **(c)** `72 != 0` is true → **72**
- **(d)** `!(72 >= 60)` → `!1` → `0` → condition is false → **72**

**Part B:**
```c
// (e)
int abs_val = (score < 0) ? -score : score;

// (f)
int clamped = (score > 100) ? 100 : score;
```

</details>

---

### Exercise 6: Type Conversions — Spot the Surprises
Predict the value stored in each variable. Watch for truncation and promotion.

```c
int   a = 9 / 2;              // (a)
double b = 9 / 2;             // (b)
double c = 9.0 / 2;           // (c)
double d = (double)(9 / 2);   // (d)
int   e = (int)3.9;           // (e)
int   f = (int)-3.9;          // (f)
```

<details>
<summary>Click to reveal answers</summary>

- **(a)** `9 / 2` is integer division → truncates to **4** → `a = 4`
- **(b)** `9 / 2` is still evaluated as integer division first (both operands are `int`) → 4, then promoted to double → `b = 4.0`
- **(c)** `9.0` is a `double` literal, so the division is floating-point → `c = 4.5`
- **(d)** `9 / 2` evaluates first as integer division → 4, then cast to double → `d = 4.0` — the cast is too late!
- **(e)** `(int)` truncates toward zero → **3** (not rounded)
- **(f)** `(int)` truncates toward zero → **-3** (not -4; C truncates, not floors)

**Key takeaways:**
- The cast in `(d)` happens *after* integer division — to fix it, cast *before*: `(double)9 / 2`
- `(int)` always truncates *toward zero*, not toward negative infinity

</details>

---

### Exercise 7: Challenge — Expression Evaluator
Work through this complex expression step by step using only operator precedence. No running allowed — pencil and paper only!

```c
int a = 2, b = 3, c = 4;
int result = (a + b) * c > 15 && !(c - a == b) || b % a == 1;
```

Evaluate in this order:
1. `a + b` = ?
2. `(a + b) * c` = ?
3. `(a + b) * c > 15` = ?
4. `c - a` = ?
5. `c - a == b` = ?
6. `!(c - a == b)` = ?
7. Result of the `&&` = ?
8. `b % a` = ?
9. `b % a == 1` = ?
10. Final result = ?

<details>
<summary>Click to reveal solution</summary>

1. `a + b` = 2 + 3 = **5**
2. `(a + b) * c` = 5 × 4 = **20**
3. `20 > 15` = **1** (true)
4. `c - a` = 4 − 2 = **2**
5. `2 == b` = `2 == 3` = **0** (false)
6. `!0` = **1** (true)
7. `1 && 1` = **1** (true)
8. **Short-circuit!** The left side of `||` is already `1` (truthy), so C skips the right side entirely
9. Steps 8–9 (`b % a == 1`) are **never evaluated** — C already knows the result is `1`
10. Final result = **1**

`result = 1`

**Key insight:** Once `(a + b) * c > 15 && !(c - a == b)` evaluates to `1`, the `||` short-circuits. The expression `b % a == 1` is never computed at runtime.

</details>

---

## Summary

| Concept        | Key Points                                                                    |
|----------------|-------------------------------------------------------------------------------|
| Integer types  | `char`, `int`, `long`; signed by default                                      |
| Floating-point | `float` (fast), `double` (precise); can't represent all values                |
| Boolean        | `0` is false, non-zero is true; use `<stdbool.h>` for clarity                 |
| `void`         | "Nothing" type for functions                                                  |
| Arithmetic     | `+`, `-`, `*`, `/`, `%`; integer division truncates                           |
| Increment      | `++x` (pre), `x++` (post); keep usage simple                                  |
| Comparison     | `==`, `!=`, `<`, `>`, `<=`, `>=`                                              |
| Logical        | `&&` (AND), `\|\|` (OR), `!` (NOT); short-circuit evaluation                  |
| Ternary        | `cond ? a : b`; compact if-else for simple value selection                    |

---

## Next Steps

Continue to [Lesson 1: Control Flow and Loops](LESSON_1_Control_Flow_And_Loops.md) →

← Back to [Learning Guide Index](LEARNING_GUIDE.md)

---

## Bonus: Advanced Topics
> **📚 Come back later!** The topics below build on concepts from later lessons.
> Read them now for a preview, or bookmark this section and return after completing:
> - [Lesson 2: Arrays](LESSON_2_Arrays.md) — for understanding array indexing with increment operators
> - [Lesson 4: Functions](LESSON_4_Functions.md) — for understanding function arguments and side effects


### Guaranteed-Size Types (`<stdint.h>`)
C doesn't guarantee exact sizes! An `int` could be 2 bytes on some old systems. When you need precise sizes, use `<stdint.h>`:

```c
#include <stdint.h>

int8_t    tiny = 127;          // Exactly 8 bits, signed (-128 to 127)
uint8_t   byte = 255;          // Exactly 8 bits, unsigned (0 to 255)
int16_t   medium = 32000;      // Exactly 16 bits, signed
uint32_t  large = 4000000000;  // Exactly 32 bits, unsigned
int64_t   huge = 9000000000;   // Exactly 64 bits, signed
```

**When to use `stdint.h` types:**
- Binary file formats (exact byte layouts)
- Network protocols (consistent across machines)
- Memory-mapped hardware (specific register sizes)

**When `int` is fine:**
- Loop counters
- Game logic (like our tic-tac-toe!)
- Most general programming

---

### Pre vs. Post Increment in Complex Expressions
The difference between `++x` (pre-increment) and `x++` (post-increment) becomes critical in complex expressions:

```c
int i = 3;
int result = i++ * 2;   // result = 6, then i = 4
                        // Uses OLD value (3) in multiplication, THEN increments

int j = 3;
int result2 = ++j * 2;  // j = 4, then result2 = 8
                        // Increments FIRST, then uses NEW value (4) in multiplication
```

**Array indexing example:**
```c
int arr[5] = {10, 20, 30, 40, 50};
int idx = 0;

int val1 = arr[idx++];  // val1 = 10 (arr[0]), then idx = 1
int val2 = arr[idx++];  // val2 = 20 (arr[1]), then idx = 2
int val3 = arr[++idx];  // idx = 3 first, then val3 = 40 (arr[3])
```

**Function argument pitfall:**
```c
int x = 5;
printf("%d %d\n", x++, x);  // ⚠️ UNDEFINED BEHAVIOR!
// The order of evaluation for function arguments is not specified
```

**⚠️ Golden Rule:** Don't modify a variable and use it elsewhere in the same expression:
```c
int i = 5;

// BAD: Using 'i' multiple times with side effects
int bad = i++ + i;    // ❌ Undefined behavior!
int worse = ++i + i++; // ❌ Undefined behavior!

// GOOD: Keep it simple
int good = i + i;      // ✅ Clear: 10
i++;                   // ✅ Increment separately
```

---

### The Comma Operator
The **comma operator** `,` evaluates multiple expressions from left to right and returns the value of the **rightmost** expression. It's the operator with the lowest precedence.

```c
int result = (expr1, expr2, expr3);  // Evaluates all, returns expr3
```

**Basic example:**
```c
int a, b, c;
c = (a = 5, b = 10, a + b);  // a=5, b=10, then c=15
printf("%d %d %d\n", a, b, c);  // Output: 5 10 15
```

**Common use — for loop with multiple variables:**
```c
// Two loop variables updated together
for (int i = 0, j = 10; i < j; i++, j--) {
    printf("i=%d, j=%d\n", i, j);
}
```

**Important:** Not every comma is the comma operator!

```c
// These are comma SEPARATORS, not operators:
int a, b, c;                    // Variable declarations
void func(int x, int y);        // Function parameters
int arr[] = {1, 2, 3};          // Array initializers

// This IS the comma OPERATOR:
int result = (a = 1, b = 2, a + b);  // The parentheses are needed!
```

**When to use the comma operator:**
- ✅ `for` loop initialization and update with multiple variables
- ✅ Macros that need to evaluate multiple expressions
- ❌ Avoid for complex expressions that reduce readability

---

### Bitwise Operations
Bitwise operators work on individual **bits** within integers. They're essential for low-level programming, flags, and optimization.

#### Topic: The Operators

| Operator | Name        | Description                    | Example (8-bit)               |
|----------|-------------|--------------------------------|-------------------------------|
| `&`      | AND         | 1 if both bits are 1           | `0b1100 & 0b1010` → `0b1000`  |
| `\|`     | OR          | 1 if either bit is 1           | `0b1100 \| 0b1010` → `0b1110` |
| `^`      | XOR         | 1 if bits are different        | `0b1100 ^ 0b1010` → `0b0110`  |
| `~`      | NOT         | Flips all bits                 | `~0x0F` → `0xF0` (8-bit)      |
| `<<`     | Left shift  | Shifts bits left, fills with 0 | `0b0011 << 2` → `0b1100`      |
| `>>`     | Right shift | Shifts bits right              | `0b1100 >> 2` → `0b0011`      |

> **📝 Note on Binary Literals:** The `0b` prefix (e.g., `0b1100`) for binary literals became standard in **C23**. 
> Before C23, it was a GCC/Clang extension. If you're using an older standard, use hexadecimal instead:
> - `0b1100` = `0x0C` = 12
> - `0b1010` = `0x0A` = 10
> - `0b0011` = `0x03` = 3
> 
> We use `0b` here because it makes bit patterns easier to visualize, and most modern compilers support it.

#### Topic: Visualizing Bitwise AND, OR, XOR
```
    1 1 0 0   (12 in decimal)
  & 1 0 1 0   (10 in decimal)
  ---------
    1 0 0 0   (8 in decimal)    ← AND: both bits must be 1

    1 1 0 0   (12)
  | 1 0 1 0   (10)
  ---------
    1 1 1 0   (14)              ← OR: either bit can be 1

    1 1 0 0   (12)
  ^ 1 0 1 0   (10)
  ---------
    0 1 1 0   (6)               ← XOR: bits must differ
```

#### Topic: Common Use Case — Flags

Imagine you're writing a game and need to track several yes/no states: Is the game running? Paused? Over? Did the player win? 

**The naive approach** uses separate variables:
```c
int is_running = 1;
int is_paused = 0;
int is_game_over = 0;
int has_won = 0;
// 4 variables = 16 bytes (4 ints × 4 bytes each)
```

**The bitwise approach** packs all four states into a single integer:
```c
unsigned int status = 0;
// 1 variable = 4 bytes, and we're only using 4 of its 32 bits!
```

**Why is this better?**

| Benefit | Explanation |
|---------|-------------|
| **Memory efficiency** | 32 boolean flags in 4 bytes vs. 128 bytes for 32 separate `int`s |
| **Atomic operations** | Save/load entire state with one assignment: `saved_status = status;` |
| **Easy state comparison** | Check if two objects have identical state: `if (obj1.status == obj2.status)` |
| **Network/file friendly** | Send or store state as a single integer — no serialization needed |
| **Cache friendly** | Related data stays together in memory, improving CPU cache performance |

**The pattern:** Each flag is a power of 2, so each uses exactly one bit position:

```c
// Define flags as powers of 2 (each uses one bit)
#define FLAG_RUNNING   0x01  // 0001 — bit 0
#define FLAG_PAUSED    0x02  // 0010 — bit 1
#define FLAG_GAME_OVER 0x04  // 0100 — bit 2
#define FLAG_WON       0x08  // 1000 — bit 3

unsigned int status = 0;

// Set a flag (turn bit ON) using OR
status |= FLAG_RUNNING;      // status = 0001

// Set multiple flags at once
status |= FLAG_PAUSED;       // status = 0011
status |= FLAG_RUNNING | FLAG_PAUSED;  // Set both in one operation

// Check if a flag is set using AND
if (status & FLAG_RUNNING) {
    printf("Game is running\n");
}

// Check multiple flags at once
if ((status & (FLAG_RUNNING | FLAG_PAUSED)) == (FLAG_RUNNING | FLAG_PAUSED)) {
    printf("Game is running AND paused\n");
}

// Clear a flag (turn bit OFF) using AND with NOT
status &= ~FLAG_PAUSED;      // status = 0001

// Toggle a flag (flip the bit) using XOR
status ^= FLAG_RUNNING;      // If it was 1, now 0. If it was 0, now 1.
```

**Real-world examples where you'll see flags:**
- File permissions: `O_RDONLY | O_CREAT | O_TRUNC` in `open()`
- Window styles: `WS_VISIBLE | WS_CHILD` in Windows API
- Compiler options: `gcc -Wall -Wextra` internally use flag bits
- Database queries: `SELECT_FLAG | JOIN_FLAG | SORT_FLAG`

#### Topic: Bit Shifting for Multiplication/Division

Shifting is faster than multiplication for powers of 2:

```c
int x = 5;

int doubled = x << 1;    // 5 * 2 = 10  (shift left by 1)
int quadrupled = x << 2; // 5 * 4 = 20  (shift left by 2)
int halved = x >> 1;     // 5 / 2 = 2   (shift right by 1, truncates)
```

#### Topic: Creating Bit Masks

A **bit mask** is a value used to select, modify, or inspect specific bits within a larger value. Think of it like a stencil — the mask determines which bits "show through" and which are blocked.

**Why use bit masks?**

| Use Case | Description |
|----------|-------------|
| **Extract fields** | Pull out specific bits from a packed value (e.g., RGB color components) |
| **Modify selectively** | Change certain bits without affecting others |
| **Test conditions** | Check if specific bits are set without caring about the rest |
| **Hardware registers** | Many hardware registers pack multiple settings into bit fields |

**The `BIT(n)` macro — your bit mask generator:**

```c
// Create a mask with bit N set (bits are numbered 0, 1, 2, ...)
#define BIT(n) (1 << (n))

// Visual: what BIT(n) produces
// BIT(0) = 0b00000001 = 0x01 = 1
// BIT(1) = 0b00000010 = 0x02 = 2
// BIT(2) = 0b00000100 = 0x04 = 4
// BIT(3) = 0b00001000 = 0x08 = 8
// BIT(7) = 0b10000000 = 0x80 = 128
```

**Common bit mask operations:**

```c
unsigned int value = 0b10101010;  // Starting value: 170 in decimal

// CHECK if bit N is set (AND with mask)
if (value & BIT(5)) {             // 0b10101010 & 0b00100000 = 0b00100000 (non-zero = true)
    printf("Bit 5 is set\n");
}
if (!(value & BIT(4))) {          // 0b10101010 & 0b00010000 = 0b00000000 (zero = false)
    printf("Bit 4 is NOT set\n");
}

// SET bit N (OR with mask) — turns bit ON regardless of current state
value |= BIT(2);                  // 0b10101010 | 0b00000100 = 0b10101110

// CLEAR bit N (AND with inverted mask) — turns bit OFF regardless of current state  
value &= ~BIT(5);                 // 0b10101110 & 0b11011111 = 0b10001110

// TOGGLE bit N (XOR with mask) — flips the bit
value ^= BIT(3);                  // 0b10001110 ^ 0b00001000 = 0b10000110
```

**Extracting multi-bit fields:**

Sometimes you need more than one bit. For example, extracting the green component from a 16-bit RGB565 color:

```c
// RGB565: 5 bits red, 6 bits green, 5 bits blue
// Bit layout: RRRR RGGG GGGB BBBB
//             15       8 7       0

uint16_t color = 0x07E0;  // Pure green: 0b00000 111111 00000

// To extract green (bits 5-10), we need a mask and a shift:
#define GREEN_MASK  0x07E0   // 0b00000 111111 00000 — bits 5-10 are 1
#define GREEN_SHIFT 5

int green = (color & GREEN_MASK) >> GREEN_SHIFT;  // green = 63 (max green)
```

**Why `(1 << n)` instead of just writing the number?**

```c
// These are equivalent:
#define FLAG_ENABLED  0x08
#define FLAG_ENABLED  (1 << 3)

// But (1 << n) is self-documenting:
#define REG_BIT_7  (1 << 7)   // Clearly bit 7
#define REG_BIT_7  0x80       // Have to mentally convert

// And it's essential when the bit position is a variable:
void set_bit(unsigned int *val, int bit_num) {
    *val |= (1 << bit_num);   // Works for any bit position
}
```

#### Topic: Real-World Example — Packing Data
Store a tic-tac-toe board (9 cells, 2 bits each = 18 bits) in a single `int`:

```c
// Each cell needs 2 bits: 00=empty, 01=X, 10=O
// Board positions: 0-8, each takes 2 bits

unsigned int board = 0;

// Set cell at position to value
void set_cell(unsigned int *board, int pos, int value) {
    int shift = pos * 2;
    *board &= ~(0x3 << shift);      // Clear the 2 bits
    *board |= (value << shift);      // Set new value
}

// Get cell at position
int get_cell(unsigned int board, int pos) {
    int shift = pos * 2;
    return (board >> shift) & 0x3;   // Extract 2 bits
}

// Usage:
set_cell(&board, 4, 1);  // Place X in center
set_cell(&board, 0, 2);  // Place O in top-left
int center = get_cell(board, 4);  // Returns 1 (X)
```

#### Topic: When to Use Bitwise Operations
**✅ Good use cases:**
- Hardware registers and device drivers
- Network protocols (parsing packet headers)
- Flags and boolean sets
- Graphics programming (color manipulation)
- Cryptography and hashing
- Performance-critical code

**❌ Usually overkill for:**
- Simple boolean logic (use `&&`, `||`, `!`)
- General application code where clarity matters more
- When individual `bool` variables are clearer

> **See also:** [Lesson 10: The Challenge](LESSON_10_The_Challenge.md) shows advanced bit-packing
> in the SDL Snake game, where the entire game grid is stored using just 3 bits per cell.

