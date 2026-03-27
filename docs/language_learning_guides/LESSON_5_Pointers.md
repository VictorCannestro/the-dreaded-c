# Lesson 5: Pointers
> **Before you start:** Make sure you're comfortable with [Lesson 0: Basic Types](LESSON_0_Basic_Types.md),
> [Lesson 3: Varieties of Named Constants](LESSON_3_Named_Constants.md), and [Lesson 4: Functions](LESSON_4_Functions.md). Pointers build on everything 
> you know about variables.

A **pointer** stores the **memory address** of another variable — not its value, but *where* it lives in memory. This 
is one of C's most powerful features, and the one beginners struggle with most. Take it section by section.

---

## Table of Contents
### Core Concepts
**Foundations**
1. [What Is Memory? A Quick Mental Model](#1-what-is-memory-a-quick-mental-model) — addresses, values, the "house number" analogy
2. [Pointer Operators](#2-pointer-operators) — `&` address-of, `*` dereference, `->` arrow
3. [Your First Pointer — Step by Step](#3-your-first-pointer--step-by-step) — reading and writing through a pointer
4. [Declaring Pointer Variables](#4-declaring-pointer-variables) — `int *p`; the multi-variable gotcha

**Why Pointers Matter**
5. [Why Pointers? Three Reasons That Matter](#5-why-pointers-three-reasons-that-matter)

**Safety**
6. [NULL — The "No Address" Value](#6-null--the-no-address-value) — safe initialisation; checking before dereference
7. [Common Pointer Mistakes](#8-common-pointer-mistakes) — uninitialised, missing `&`, missing `*`

**Struct Access**
8. [Struct Pointers and the Arrow Operator](#7-struct-pointers-and-the-arrow-operator--) 

### Practice
9. [Pointers Throughout This Project](#9-pointers-throughout-this-project) — real examples from the codebase
10. [Exercises](#10-exercises)

### Going Further
11. [Summary](#11-summary)
12. [Next Steps](#next-steps)
13. [Bonus: Advanced Topics](#bonus-advanced-topics) *(return after Lessons 6 & 9)*
    - `const` With Pointers — read-only access
    - Returning a Pointer to a Local Variable — the dangling pointer trap

---

## 1. What Is Memory? A Quick Mental Model
Every variable you declare occupies a slot in your computer's RAM. Each slot has:

1. A **value** — the data stored there
2. An **address** — a unique number identifying *where* in RAM it lives

Think of RAM like a street of numbered houses. A variable is a house with a value inside. A pointer is a piece of paper 
with a *house number* written on it.

```
RAM (simplified):

Address │ Value
────────┼───────
0x7ffc  │  42      ← int x = 42;
0x7ffd  │   0
0x7ffe  │ 0x7ffc   ← int *ptr = &x;  (stores the address of x)
0x7fff  │   0
```

---

## 2. Pointer Operators
There are three operators you need to know:

| Operator | Name        | What It Does                                 | Example         |
|----------|-------------|----------------------------------------------|-----------------|
| `&`      | Address-of  | Gets the **address** of a variable           | `&x` → `0x7ffc` |
| `*`      | Dereference | Gets the **value** at an address             | `*ptr` → `42`   |
| `->`     | Arrow       | Access a struct member **through** a pointer | `state->status` |

---

## 3. Your First Pointer — Step by Step
```c
int x = 42;        // A normal integer variable
int *ptr = &x;     // ptr holds the ADDRESS of x
                   //  └── The * in "int *ptr" means "ptr is a pointer to int"
                   //  └── The & in "&x" means "give me the address of x"
```

Now let's see what we can do with `ptr`:

```c
printf("%d\n",  x);     // 42        — the value of x
printf("%p\n", &x);     // 0x7ffc... — the address of x
printf("%p\n",  ptr);   // 0x7ffc... — ptr holds that same address
printf("%d\n", *ptr);   // 42        — "go to that address and read the int there"
```

And the key superpower — **modifying `x` through the pointer**:

```c
*ptr = 100;           // "go to the address in ptr and write 100 there"
printf("%d\n", x);    // 100 — x itself changed!
```

> 💡 Reading `*ptr` is called **dereferencing** the pointer. You're following the arrow from the piece of paper to the house and looking inside.

---

## 4. Declaring Pointer Variables
The `*` in a declaration means "this variable is a pointer to...":

```c
int    *p;   // pointer to int
char   *c;   // pointer to char
float  *f;   // pointer to float
```

⚠️ **Common gotcha with multi-variable declarations:**

```c
int *a, b;    // a is int*, but b is plain int  ← confusing!
int *a, *b;   // both a and b are int*          ← explicit and clear
```

Best practice: declare one variable per line to avoid this trap.

---

## 5. Why Pointers? Three Reasons That Matter
### Reason 1 — Pass by Reference (Let Functions Modify Your Variables)
By default, C **copies** values into functions. The original is untouched:

```c
// ❌ This does NOT work — num is a copy
void broken_increment(int num) {
    num = num + 1;   // modifies the copy only
}

int score = 5;
broken_increment(score);
printf("%d\n", score);   // still 5 — the original didn't change!
```

Pass a **pointer** and the function can reach back out and change the original:

```c
// ✅ This DOES work — num is a pointer to the original
void increment(int *num) {
    *num = *num + 1;   // follows the pointer and modifies the original
}

int score = 5;
increment(&score);         // pass the address of score
printf("%d\n", score);     // 6 — it changed!
```

### Reason 2 — Avoid Expensive Copies
Structs can be large. Passing them by value copies every byte:

```c
// ❌ Copies the entire GameState struct on every call (slow for large structs!)
void display_bad(GameState state) { ... }

// ✅ Copies only the address (4 or 8 bytes, depending on platform)
void display_good(const GameState *state) { ... }
```

You'll see `const GameState *state` throughout this project. The `const` says:
*"I have the address but I promise not to modify what's there."*

### Reason 3 — Return Multiple Values
A function can only `return` one value — but it can write to several pointers:

```c
void get_min_max(int arr[], int size, int *min, int *max) {
    *min = arr[0];
    *max = arr[0];
    for (int i = 1; i < size; i++) {
        if (arr[i] < *min) *min = arr[i];
        if (arr[i] > *max) *max = arr[i];
    }
}

// Usage:
int scores[] = {3, 7, 1, 9, 4};
int lo, hi;
get_min_max(scores, 5, &lo, &hi);
printf("min=%d  max=%d\n", lo, hi);   // min=1  max=9
```

---

## 6. NULL — The "No Address" Value
`NULL` is the special pointer value meaning "this pointer points to nothing".
Always initialise pointers to `NULL` when you don't have an address yet, and
**always check before dereferencing**:

```c
int *ptr = NULL;         // safe starting state

if (ptr != NULL) {
    *ptr = 5;            // only safe if ptr actually points somewhere
}
```

What happens if you dereference `NULL`? Instant crash — a **segmentation fault**:

```c
int *ptr = NULL;
*ptr = 5;   // 💥 CRASH: "Segmentation fault"
```

> 💡 `NULL` is not a keyword — it's a macro defined in `<stdlib.h>`, `<stdio.h>`,
> and `<stddef.h>`. Make sure you include one of those headers.

Every function in this project that takes a pointer checks for `NULL` first:

```c
// src/tictactoe.c
void game_init_session(GameState *state) {
    if (state == NULL) {   // ← guard clause: bail out if called with NULL
        return;
    }
    // ... safe to use state below
}
```

This is a critical defensive programming habit. If someone accidentally passes
`NULL`, the function returns safely instead of crashing.

---

## 7. Struct Pointers and the Arrow Operator `->`
When you have a **pointer to a struct**, you access its members through that
pointer. There are two ways — one ugly, one clean:

```c
GameState  game;          // the struct itself
GameState *ptr = &game;   // a pointer to the struct

// Option A — dereference then dot (ugly, easy to get wrong)
(*ptr).status = ONGOING;   // parentheses are required!

// Option B — arrow notation (preferred, always use this)
ptr->status = ONGOING;     // identical meaning, much cleaner
```

`ptr->status` literally means: *"follow the pointer and give me the `status` member."*

> Once you've worked through [Lesson 6: Structs](LESSON_6_Structs.md), you'll use `->` constantly. Lesson 6 covers
> dot vs. arrow in full, including a real walk-through of `game_init_session` and the rules for when to use each.

---

## 8. Common Pointer Mistakes
### ❌ Uninitialized Pointer
```c
int *ptr;       // points to garbage — contains a random address
*ptr = 42;      // 💥 writing to a random location — undefined behaviour
```

**Fix:** always initialise before use:
```c
int value = 0;
int *ptr = &value;   // ✅ points to a real variable
// — or —
int *ptr = NULL;     // ✅ clearly "no address yet"
```

### ❌ Forgetting `&` When Passing to a Pointer Parameter
```c
int score = 5;
increment(score);    // ❌ passes the value 5, not an address
increment(&score);   // ✅ passes the address of score
```

### ❌ Forgetting `*` When Writing Through a Pointer
```c
void increment(int *num) {
    num = num + 1;    // ❌ moves the pointer forward in memory — doesn't change the int!
    *num = *num + 1;  // ✅ follows the pointer and changes the int
}
```

---

## 9. Pointers Throughout This Project

| Function                                                 | Pointer Parameter        | Why a Pointer?                                     |
|----------------------------------------------------------|--------------------------|----------------------------------------------------|
| `game_init_session(GameState *state)`                    | `GameState *`            | Writes initial values into the caller's struct     |
| `game_make_move(GameState *state, int pos)`              | `GameState *`            | Updates board, move count, and status              |
| `board_place(CellValue board[], int pos, CellValue sym)` | Array (implicit pointer) | Writes a symbol into the caller's array            |
| `board_is_empty(const CellValue board[], int pos)`       | `const` array            | Read-only: promises not to modify the board        |
| `game_get_current_player(GameState *state)`              | `GameState *`            | Reads `move_count` (could use `const` but doesn't) |

Notice the pattern: **functions that modify** data must use non-`const` pointers.
**Functions that only read** *should* use `const` — like `board_is_empty` does.
However, some functions in this codebase (like `game_get_current_player`) don't
follow this best practice. When you write your own code, prefer `const` for
read-only access.

---

## 10. Exercises
Work through these before moving on. Struggle is part of learning — try each
one yourself before revealing the answer.

---

### Exercise 1 — Tracing Pointer Values
Given this code, what does each `printf` print?

```c
int a = 10;
int b = 20;
int *p = &a;

printf("1: %d\n", *p);
*p = 99;
printf("2: %d\n", a);

p = &b;
printf("3: %d\n", *p);
*p = *p + 1;
printf("4: %d\n", b);
```

<details>
<summary>Reveal Answer</summary>

```
1: 10    — p points to a, which is 10
2: 99    — *p = 99 wrote through the pointer into a
3: 20    — p now points to b, which is 20
4: 21    — *p = *p + 1 incremented b through the pointer
```

Key insight: re-assigning `p = &b` changes *where* the pointer points — it does
not affect the value that `p` previously pointed to.

</details>

---

### Exercise 2 — Spot the Bug
This function is supposed to swap two integers. What's wrong?

```c
void swap(int a, int b) {
    int temp = a;
    a = b;
    b = temp;
}

int x = 5, y = 10;
swap(x, y);
printf("%d %d\n", x, y);   // Expected: 10 5 — but what actually prints?
```

<details>
<summary>Reveal Answer</summary>

`a` and `b` are **copies** of `x` and `y`. The function swaps the copies, but
the originals `x` and `y` are completely unchanged. The output is still `5 10`.

**Fix:** pass pointers so the function can reach the originals:

```c
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int x = 5, y = 10;
swap(&x, &y);
printf("%d %d\n", x, y);   // 10 5 ✅
```

</details>

---

### Exercise 3 — Fill in the Blanks
Complete the function so that it doubles the value at the given pointer:

```c
void double_value(int ___ num) {
    ___ = ___ * 2;
}

// Should work like this:
int score = 7;
double_value(___);
printf("%d\n", score);   // 14
```

<details>
<summary>Reveal Answer</summary>

```c
void double_value(int *num) {
    *num = *num * 2;
}

int score = 7;
double_value(&score);    // pass the address of score
printf("%d\n", score);   // 14
```

- The parameter type is `int *` — a pointer to int
- Inside the function, `*num` dereferences the pointer to read/write the value
- The call site passes `&score` — the address of the variable to modify

</details>

---

### Exercise 4 — Read the Project Code
Look at `game_make_move` in [`src/tictactoe.c`](../../src/tictactoe.c):

```c
int game_make_move(GameState *state, int position) {
    if (state == NULL || state->status != ONGOING) {
        return -1;
    }

    CellValue current = game_get_current_player(state);
    if (board_place(state->board, position, current) != 0) {
        return -1;
    }

    state->move_count++;
    update_status(state);
    return 0;
}
```

Answer these questions:

1. Why does this function take `GameState *state` instead of `GameState state`?
2. What does `state->move_count++` do, and why use `->` instead of `.`?
3. Why does the function return `int` instead of `void`?

<details>
<summary>Reveal Answer</summary>

**1. `GameState *state` instead of `GameState state`**

The function needs to *modify* the `GameState` (it increments `move_count` and
calls `update_status` which changes `status`). If it received a copy
(`GameState state`), all those modifications would be discarded when the
function returned. The pointer gives the function direct access to the
*caller's* struct.

**2. `state->move_count++`**

`state` is a pointer (`GameState *`), not a struct variable, so you cannot use
`.` on it directly — the `.` operator is for struct variables, `->` is for
struct *pointers*. The `->` operator dereferences the pointer and accesses the
member in one step. This line reads `move_count`, adds 1, and writes it back —
all into the original `GameState` the caller owns.

**3. Returns `int` instead of `void`**

The function can fail for several reasons: `state` is `NULL`, the game is
already over, the position is out of bounds, or the cell is already occupied.
Returning `0` for success and `-1` for any error lets the caller react
appropriately:

```c
if (game_make_move(&game, pos) == -1) {
    printf("Invalid move — try again.\n");
}
```

A `void` function would silently do nothing on error, hiding the problem.

</details>

---

### Exercise 5 — Write It Yourself
Write a function `clamp` that takes a pointer to an integer and two bounds
`lo` and `hi`. If the value is below `lo`, set it to `lo`. If it's above `hi`,
set it to `hi`. Otherwise, leave it unchanged.

```c
void clamp(int *value, int lo, int hi) {
    // your code here
}

// Should work like this:
int a = -5;  clamp(&a, 0, 100);  printf("%d\n", a);  // 0
int b = 150; clamp(&b, 0, 100);  printf("%d\n", b);  // 100
int c = 42;  clamp(&c, 0, 100);  printf("%d\n", c);  // 42
```

<details>
<summary>Reveal Answer</summary>

```c
void clamp(int *value, int lo, int hi) {
    if (*value < lo) {
        *value = lo;
    } else if (*value > hi) {
        *value = hi;
    }
    // if in range, do nothing — the value is already correct
}
```

**Why no return value?** The function communicates its result by writing
through the pointer — the caller's variable is updated in place, exactly the
same way `game_make_move` updates the `GameState`.

**Common mistake to watch for:** writing `value = lo` (moves the pointer
itself, changes nothing) instead of `*value = lo` (follows the pointer and
changes the integer it points to).

</details>

---

## 11. Summary

| Concept                   | Syntax             | When to Use                                 |
|---------------------------|--------------------|---------------------------------------------|
| Get an address            | `&variable`        | Passing a variable to a pointer parameter   |
| Declare a pointer         | `int *ptr`         | To hold an address                          |
| Read through a pointer    | `*ptr`             | To get the value at the address             |
| Write through a pointer   | `*ptr = val`       | To modify the original variable             |
| Struct member via pointer | `ptr->member`      | When you have a pointer to a struct         |
| Safe null check           | `if (ptr != NULL)` | Before every dereference                    |
| Read-only pointer         | `const T *ptr`     | When the function must not modify the value |

---

## Next Steps

Continue to [Lesson 6: Structs](LESSON_6_Structs.md) →

← Previous: [Lesson 4: Functions](LESSON_4_Functions.md)

← Back to [Learning Guide Index](LEARNING_GUIDE.md)

---

## Bonus: Advanced Topics
> **📚 Come back later!** The topics below build on concepts from later lessons.
> Read them now for a preview, or bookmark this section and return after completing:
> - [Lesson 6: Structs](LESSON_6_Structs.md) — for understanding `const` with struct pointers
> - [Lesson 9: Memory Management](LESSON_9_Memory_Management.md) — for understanding stack vs heap memory


### `const` With Pointers
`const` and pointers can combine in two different ways. Read the type
declaration **right-to-left** to understand it:

```c
const int *ptr;     // pointer to a const int — you can't change the value
int *const ptr;     // const pointer to int   — you can't change the address
```

In practice, you'll mostly see the first form, used to promise "read but not write":

```c
// ✅ Reads board contents, promises not to change them
int board_is_empty(const CellValue board[BOARD_SIZE], int position) {
    return board[position] == CELL_EMPTY;
}
```

Using `const` makes your intent explicit and lets the compiler catch accidental
writes. If you try to assign through a `const` pointer the compiler stops you:

```c
void bad(const int *ptr) {
    *ptr = 0;   // ❌ compiler error: assignment to const location
}
```

> [Lesson 6: Structs](LESSON_6_Structs.md) covers `const` applied to struct pointer parameters in depth,
> including a table of which project functions use it and why.


### Returning a Pointer to a Local Variable
This is a classic and dangerous bug:

```c
int *get_value(void) {
    int local = 42;
    return &local;    // 💥 local is destroyed when the function returns!
}
```

Local variables live on the **stack** and vanish when their function returns.
The returned pointer now points to freed memory — a **dangling pointer**. Using
it is undefined behaviour.

**Why this happens:** Every function call creates a "stack frame" that holds its
local variables. When the function returns, that frame is discarded. Any pointer
to memory inside that frame is now invalid.

**Solutions:**
1. Return the value itself (not a pointer): `int get_value(void) { return 42; }`
2. Have the caller pass in a pointer: `void get_value(int *result) { *result = 42; }`
3. Use heap allocation (covered in [Lesson 9](LESSON_9_Memory_Management.md))

