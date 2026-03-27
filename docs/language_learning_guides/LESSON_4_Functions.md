# Lesson 4: Functions
> **Before you start:** Make sure you're comfortable with
> [Lesson 0: Basic Types](LESSON_0_Basic_Types.md) and [Lesson 3: Varieties of Named Constants](LESSON_3_Named_Constants.md).
> Functions are where everything comes together — they consume the types you already know.

Functions are the **building blocks of C programs**. Every action your program takes — placing a
piece on the board, checking for a winner, drawing the grid — is expressed as a function call.
This lesson builds your mental model from the ground up: anatomy → declarations → return types →
parameters → visibility.

---

## Table of Contents
### Core Concepts
**Foundations**
1. [Anatomy of a Function](#1-anatomy-of-a-function) — return type, name, parameters, body
2. [Declarations vs. Definitions](#2-declarations-vs-definitions) — header promises, source delivers

**Return Types**
3. [Return Types — Communicating Results](#3-return-types--communicating-results)
   - Pattern A: `void` — side-effect only
   - Pattern B: `int` success/failure — `0` = OK, `-1` = error
   - Pattern C: `int` boolean — `0` = false, non-zero = true
   - Pattern D: value return — query functions

**Parameters**
4. [Parameters — How Data Flows In](#4-parameters--how-data-flows-in) — value vs. pointer; `const`

### Practice
5. [Exercises](#5-exercises)

### Going Further
6. [Summary](#6-summary)
7. [Next Steps](#next-steps)
8. [Bonus: Advanced Topics](#bonus-advanced-topics) *(return after Lessons 5 & 7)*
   - Static Functions — Private Helpers
   - The `(void)` Parameter List
   - Macros — Function-Like Text Substitution


---

## 1. Anatomy of a Function
Every function has four parts:

```
return_type  function_name ( parameters ) { body }
```

Let's look at a real, minimal example first, then layer on the complexity:

```c
int add(int a, int b) {
    return a + b;
}
```

Annotated:

```
int       add      (int a, int b)    { return a + b; }
 ↑         ↑              ↑                  ↑
 │         │              │                  └─ Body: the work done
 │         │              └─ Parameters: inputs the caller provides
 │         └─ Name: how you call it
 └─ Return type: what type of value comes back
```

And calling it:

```c
int result = add(3, 4);   // result == 7
```

> 💡 **Mental model:** A function is a vending machine. You insert coins (parameters), the machine
> does something, and it hands you a snack (the return value). The mechanism inside is hidden —
> the caller only cares about what goes in and what comes out.

---

## 2. Declarations vs. Definitions
We'll cover this in more detail in the lesson on header files, but for now just keep in mind that this distinction 
is **fundamental** in C and shows up constantly in this project.

### The Declaration (`.h` file) — "I promise this function exists"
A declaration tells the compiler the function's **name**, **return type**, and **parameter types**
— everything a caller needs to use it, nothing more.

```c
// include/tictactoe.h
int  game_make_move(GameState *state, int position);
void game_init_session(GameState *state);
int  game_is_over(GameState *state);
```

### The Definition (`.c` file) — "Here is how it actually works"
The definition contains the body — the real implementation:

```c
// src/tictactoe.c
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

### Why keep them separate?

| Concern                                   | Header (`.h`) | Source (`.c`) |
|-------------------------------------------|---------------|---------------|
| What a caller needs to know               | ✅             | ✅             |
| How it works internally                   | ❌             | ✅             |
| Compiled into every file that includes it | ✅             | ❌             |

When `ui_cli.c` calls `game_make_move`, it only needs the header. It does not need to know about
`update_status`, `board_place`, or any internal logic. This **enforces a boundary** between
modules and is the foundation of good C architecture.

> ⚠️ **Common mistake:** Defining a function in a `.h` file. If two `.c` files both `#include`
> that header, you get a "multiple definition" linker error. Keep bodies in `.c` files.

---

## 3. Return Types — Communicating Results
A function always has exactly one return type. Here are the four patterns used throughout this
project, and when to reach for each.


### Pattern A — `void`: Do something, report nothing back
Use `void` when the function's job is a **side effect** (filling the board, printing the screen)
and the caller doesn't need a result.

```c
// src/tictactoe.c
void game_init_session(GameState *state) {
    if (state == NULL) {
        return;           // Early exit — no value to return
    }

    board_init(state->board);
    state->status    = ONGOING;
    state->move_count = 0;
    state->game_count = 0;
    state->last_winner = CELL_EMPTY;
    state->difficulty  = DIFFICULTY_HARD;

    state->players[0].type   = PLAYER_HUMAN;
    state->players[0].symbol = CELL_X;
    state->players[1].type   = PLAYER_COMPUTER;
    state->players[1].symbol = CELL_O;

    srand((unsigned int)time(NULL));
}
```

Notice the **guard clause** at the top: `if (state == NULL) { return; }`. Every public function in
this project that receives a pointer does this. It is defensive programming — never trust the
caller.


### Pattern B — `int` as success/failure: 0 = OK, -1 = error

This is C's conventional way of reporting whether an operation succeeded. It is used whenever the
function **does something** that might fail.

```c
// src/board.c
int board_place(CellValue board[BOARD_SIZE], int position, CellValue symbol) {
    if (position < 0 || position >= BOARD_SIZE) {
        return -1;                       // Out of range
    }
    if (!board_is_empty(board, position)) {
        return -1;                       // Cell already occupied
    }
    board[position] = symbol;
    return 0;                            // Success
}
```

The caller **must check the return value**:

```c
// src/tictactoe.c
if (board_place(state->board, position, current) != 0) {
    return -1;   // Propagate the error up the call stack
}
```

This pattern composes: `game_make_move` calls `board_place`, checks its return, and returns its
own `-1` if something went wrong. Errors bubble upward naturally.


### Pattern C — `int` as boolean: non-zero = true, 0 = false
When a function **answers a yes/no question**, return `1` (true) or `0` (false):

```c
// src/tictactoe.c
int game_is_over(GameState *state) {
    if (state == NULL) {
        return 0;
    }
    return state->status != ONGOING;   // Evaluates to 1 or 0
}

// src/board.c
int board_is_valid_move(const CellValue board[BOARD_SIZE], int position) {
    if (position < 0 || position >= BOARD_SIZE) {
        return 0;
    }
    return board_is_empty(board, position);
}
```

These feel natural in `if` conditions:

```c
if (game_is_over(&game)) {
    printf("Game over!\n");
}

while (!game_is_valid_move(state->board, position)) {
    printf("Try again.\n");
}
```


### Pattern D — Return the actual value: query functions
When a function **reads and reports** a value, return it directly:

```c
// src/tictactoe.c
// X moves on even turns (0, 2, 4…), O on odd turns.
CellValue game_get_current_player(GameState *state) {
    if (state == NULL) {
        return CELL_X;
    }
    return (state->move_count % 2 == 0) ? CELL_X : CELL_O;
}

GameStatus game_get_status(GameState *state) {
    if (state == NULL) {
        return ONGOING;
    }
    return state->status;
}
```

The ternary operator `condition ? value_if_true : value_if_false` is a compact way to choose
between two values. Think of it as a one-line `if/else` that produces a value.

> 💡 **Choosing a pattern:** Ask yourself — does my function *do something that might fail*
> (→ pattern B), *answer a yes/no* (→ pattern C), *fetch a value* (→ pattern D), or *purely
> change state with no interesting outcome* (→ pattern A)?

---

## 4. Parameters — How Data Flows In
### Value parameters — C copies the argument
```c
// This function receives a COPY of n.
// The original variable in the caller is untouched.
void print_incremented(int n) {
    n = n + 1;
    printf("%d\n", n);   // Prints the copy + 1
}

int score = 10;
print_incremented(score);
printf("%d\n", score);   // Still 10 — unchanged!
```

This is called **pass by value** and is C's default.

### Pointer parameters — a preview
What if a function needs to **modify** the caller's variable? Or you want to avoid copying a large
struct? You pass a **pointer** instead — the memory address of the variable.

```c
GameState game;
game_init_session(&game);   // & means "address of game"
```

The `&` operator gives the address. Inside the function, the `*` in `GameState *state` means
"this is a pointer to a GameState." The function can then modify the original through that pointer.

> 💡 We'll cover pointers in depth in [Lesson 5: Pointers](LESSON_5_Pointers.md). For now, just
> know that when you see `*` in a parameter type, the function can reach back and modify your
> variable, and you pass the address with `&`.

### `const` pointer parameters — read-only access
When a function only **reads** data through a pointer but doesn't modify it, mark the pointer
`const`:

```c
// board.c — the compiler will reject any attempt to modify board inside here.
int board_is_empty(const CellValue board[BOARD_SIZE], int position) {
    return board[position] == CELL_EMPTY;   // Read-only
}
```

`const` serves two purposes:
1. **Documentation** — it tells readers "this function won't change your data".
2. **Safety** — any accidental write inside the function becomes a compile error.

### Naming parameters in declarations
You can omit parameter **names** in a declaration (only types are required), but including names
makes headers self-documenting:

```c
// Minimal — technically valid, but harder to read
int board_place(CellValue[], int, CellValue);

// Better — names document intent
int board_place(CellValue board[BOARD_SIZE], int position, CellValue symbol);
```

Always include parameter names in your header declarations.

---

## 5. Exercises

Work through these before looking at the answers. The goal is to build the habit of reading
function signatures and predicting behaviour before running the code.

---

### Exercise 1 — Reading a signature

Look at this declaration from `tictactoe.h`:

```c
PlayerType game_get_player_type(GameState *state, CellValue symbol);
```

**Questions:**
1. What does this function return?
2. What are its two inputs and what type is each?
3. Should the caller check a return value for errors? Why or why not?
4. Does this function look like it modifies `state`? What keyword would make that guarantee explicit?

<details>
<summary>Click to reveal answer</summary>

1. It returns a `PlayerType` — either `PLAYER_HUMAN` or `PLAYER_COMPUTER`.
2. `state` is a `GameState *` (pointer to the current game state); `symbol` is a `CellValue`
   (which player to look up: `CELL_X` or `CELL_O`).
3. It returns an enum, not an `int` used as success/failure, so there is no error code to check.
   However, the implementation falls back to `PLAYER_HUMAN` if `state` is NULL or `symbol` is
   `CELL_EMPTY` — a safe default.
4. The function does not appear to modify `state` (it only reads from it). The `const` keyword
   on the pointer would make that explicit: `game_get_player_type(const GameState *state, ...)`.

</details>

---

### Exercise 2 — Tracing a return value

Given these two functions:

```c
CellValue game_get_current_player(GameState *state) {
    if (state == NULL) {
        return CELL_X;
    }
    return (state->move_count % 2 == 0) ? CELL_X : CELL_O;
}

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

A freshly initialised game has `move_count = 0`. Three moves are made at positions 0, 4, and 8
(all valid, all succeed).

**Questions:**
1. What does `game_get_current_player` return before any move?
2. What does it return after the first move?
3. What does `game_make_move` return for each of the three calls?
4. What is `move_count` after all three moves?

<details>
<summary>Click to reveal answer</summary>

1. `move_count` is 0 → `0 % 2 == 0` is true → returns `CELL_X`.
2. After move 1, `move_count` is 1 → `1 % 2 == 0` is false → returns `CELL_O`.
3. All three calls return `0` (success) because the positions are valid and the game is `ONGOING`.
4. `move_count` is 3 after three successful moves.

</details>

---

### Exercise 3 — `static` visibility

`update_status` is declared `static` in `tictactoe.c`. Imagine a developer in `ui_cli.c` tries to
call it directly:

```c
// ui_cli.c
update_status(&game);   // Will this compile?
```

**Questions:**
1. Will this compile? Why or why not?
2. What is the correct way for `ui_cli.c` to trigger a status update?
3. Name one advantage of keeping `update_status` static.

<details>
<summary>Click to reveal answer</summary>

1. **No, it will not compile.** `static` restricts the function's visibility to the translation
   unit (`.c` file) where it is defined. `ui_cli.c` cannot see it — the linker will report
   "undefined reference to `update_status`".
2. `ui_cli.c` calls `game_make_move`, which internally calls `update_status` after every move.
   The status update is an implementation detail — `ui_cli.c` never needs to invoke it directly.
3. Any of: it cannot be accidentally called from the wrong place; the implementation can be
   changed or renamed without breaking any other module; it removes noise from the public API.

</details>

---

### Exercise 4 — Write a function

Write a function called `game_count_empty_cells` that takes a `const GameState *` and returns
the number of empty cells remaining on the board. Use only concepts from this lesson and earlier
ones. Write the declaration (for a header) and the definition (for a `.c` file) separately.

<details>
<summary>Click to reveal answer</summary>

**Declaration** (would go in `tictactoe.h` or `board.h`):
```c
int game_count_empty_cells(const GameState *state);
```

**Definition:**
```c
int game_count_empty_cells(const GameState *state) {
    if (state == NULL) {
        return 0;
    }

    int count = 0;
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (state->board[i] == CELL_EMPTY) {
            count++;
        }
    }
    return count;
}
```

Key decisions:
- `const GameState *` because we only read the board — never write to it.
- Guard clause for `NULL` matching the project's convention.
- Returns `int` (Pattern D: query function).
- Could alternatively delegate to `board_is_empty` per cell — either is fine.

</details>

---

### Exercise 5 — Spot the bugs
This function has **three bugs**. Find them all.

```c
void game_set_difficulty(GameState *state, Difficulty difficulty) {
    state->difficulty = difficulty;
}
```

Hint: look at the real implementation in `src/tictactoe.c` and compare.

<details>
<summary>Click to reveal answer</summary>

There is actually only **one bug** in this short snippet, but it is critical:

1. **Missing NULL guard.** If `state` is `NULL`, `state->difficulty` dereferences a null pointer —
   undefined behaviour (almost certainly a crash). The fix:

```c
void game_set_difficulty(GameState *state, Difficulty difficulty) {
    if (state == NULL) {
        return;
    }
    state->difficulty = difficulty;
}
```

The original lesson listed three bugs as a trick question — but practising with a `NULL` check
is the single most important habit to build for pointer-receiving functions in C.

</details>

---

## 6. Summary

| Concept                    | Key Point                                                  |
|----------------------------|------------------------------------------------------------|
| Declaration vs. definition | Header declares the contract; source defines the body      |
| `void` return              | Side-effect only; nothing to report back                   |
| `int` success/failure      | `0` = OK, `-1` = error; caller must check                  |
| `int` boolean              | `0` = false, non-zero = true; use in `if`/`while` directly |
| Value return               | Query functions return the data directly                   |
| Pointer parameter          | Lets a function modify the caller's variable               |
| `const` pointer            | Read-only access; compiler-enforced                        |

---

## Next Steps

Continue to [Lesson 5: Pointers](LESSON_5_Pointers.md) →

← Previous: [Lesson 3: Varieties of Named Constants](LESSON_3_Named_Constants.md)

← Back to [Learning Guide Index](LEARNING_GUIDE.md)

---

## Bonus Advanced Topics
> **📚 Come back later!** The topics below build on concepts from later lessons.
> Read them now for a preview, or bookmark this section and return after completing:
> - [Lesson 5: Pointers](LESSON_5_Pointers.md) — for deeper understanding of `*` and `&`
> - [Lesson 7: Header Files](LESSON_7_Header_Files.md) — for understanding declarations vs definitions
>
> The **Macros** section expands on `#define` from [Lesson 3](LESSON_3_Named_Constants.md),
> showing how macros can mimic functions (with important caveats).


### Static Functions — Private Helpers
The `static` keyword on a function limits its **visibility** to the file it is defined in. Think
of it as "private" in C's module system.

```c
// src/tictactoe.c  — only code in THIS file can call this function
static void update_status(GameState *state) {
    CellValue winner = wcc_check_winner(state->board);

    if (winner == CELL_X) {
        state->status      = X_WINS;
        state->last_winner = CELL_X;
    } else if (winner == CELL_O) {
        state->status      = O_WINS;
        state->last_winner = CELL_O;
    } else if (wcc_is_draw(state->board, state->move_count)) {
        state->status = DRAW;
    }
}
```

`update_status` is an **implementation detail** of `tictactoe.c`. It is called by `game_make_move`
after every successful move, but no other module needs to know it exists. There is no declaration
in `tictactoe.h`.

Similarly, `win_condition_calculator.c` has a private helper:

```c
// src/win_condition_calculator.c
static CellValue check_line(const CellValue board[BOARD_SIZE], int start, int step) {
    CellValue first = board[start];
    if (first == CELL_EMPTY) {
        return CELL_EMPTY;
    }
    for (int i = 1; i < WIN_LENGTH; i++) {
        if (board[start + i * step] != first) {
            return CELL_EMPTY;
        }
    }
    return first;
}
```

`check_line` does the real work for *all four* direction-checking functions (rows, columns,
diagonals, anti-diagonals). By making it `static`, it is encapsulated inside the file that owns it.

**Rule of thumb:** if a function is not in the header, it should be `static`.

> 📖 **Case Study:** For a deep dive into how `check_line` and the public `wcc_check_*` functions
> work together as a real-world example of static helper architecture, see
> [Win Condition Calculator Algorithm](../algorithms/WIN_CONDITION_CALCULATOR.md). That document
> shows the full function hierarchy, explains each step value, and walks through the algorithm
> with visual examples — a practical application of the declaration/definition and visibility
> concepts from this lesson.


### The `(void)` Parameter List
This is a subtle but important C-specific gotcha:

```c
int get_difficulty();      // ⚠️  "I don't know how many parameters this takes" (deprecated C)
int get_difficulty(void);  // ✅  "This function takes exactly zero parameters"
```

In C (unlike C++), an empty parameter list `()` in a **declaration** means "unspecified
parameters", not "no parameters". This is a legacy behaviour from pre-ANSI C that can lead to
silent bugs. Always write `(void)` when a function takes no arguments.

You can see this in the project:

```c
// include/ui_interface.h
Difficulty (*get_difficulty)(void);  // Function pointer: takes nothing, returns Difficulty
```


### Macros — Function-Like Text Substitution
[Lesson 3](LESSON_3_Named_Constants.md) introduced `#define` for simple value constants like
`BOARD_SIZE`. But `#define` can also create **function-like macros** — text substitutions that
take parameters. This is where macros become both powerful and dangerous.

#### Basic Syntax

```c
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define SQUARE(x) ((x) * (x))
#define IS_VALID_POSITION(pos) ((pos) >= 0 && (pos) < BOARD_SIZE)
```

Usage:

```c
int biggest = MAX(score1, score2);      // Expands to: ((score1) > (score2) ? (score1) : (score2))
int area = SQUARE(5);                   // Expands to: ((5) * (5))
if (IS_VALID_POSITION(move)) { ... }    // Expands to: ((move) >= 0 && (move) < 9)
```

> 💡 **Key insight:** Macros are not functions — they are **text substitutions** performed by the
> preprocessor *before* compilation. The compiler never sees `MAX`, only the expanded code.

#### Why All the Parentheses?
This is the most common macro pitfall. Consider this broken macro:

```c
#define SQUARE_BAD(x) x * x
```

It looks fine, but:

```c
int result = SQUARE_BAD(3 + 2);   // Expands to: 3 + 2 * 3 + 2 = 3 + 6 + 2 = 11 (wrong!)
```

Operator precedence ruins the calculation. The fix is to wrap **every parameter use** and the
**entire expression** in parentheses:

```c
#define SQUARE(x) ((x) * (x))

int result = SQUARE(3 + 2);       // Expands to: ((3 + 2) * (3 + 2)) = 25 (correct!)
```

**Rule:** Always parenthesise (1) each parameter use and (2) the whole macro body.

#### The Double-Evaluation Trap
Another subtle danger: macro parameters are evaluated **every time they appear** in the
expansion:

```c
#define MAX(a, b) ((a) > (b) ? (a) : (b))

int x = 5;
int result = MAX(x++, 3);   // Expands to: ((x++) > (3) ? (x++) : (3))
                             // x is incremented TWICE if x > 3!
```

This is undefined behaviour and a real bug. Functions don't have this problem — they evaluate
arguments exactly once.

#### When to Use Macros vs. Functions

| Aspect                | Macro                                    | Function                         |
|-----------------------|------------------------------------------|----------------------------------|
| Evaluated             | At compile time (text substitution)      | At runtime                       |
| Type-checked          | ❌ No                                     | ✅ Yes                            |
| Debugger-friendly     | ❌ No (expands inline)                    | ✅ Yes (can set breakpoints)      |
| Arguments evaluated   | Every time they appear                   | Exactly once                     |
| Performance           | ✅ Zero call overhead                     | Minimal overhead (often inlined) |
| Can be type-generic   | ✅ Yes (works with any type)              | ❌ No (without `_Generic` in C11) |

**General advice:**
- Prefer functions for anything complex or with side effects
- Use macros for simple, type-generic operations (like `MIN`/`MAX`)
- Always use parentheses liberally
- Avoid passing expressions with side effects (`x++`, function calls) to macros

#### Macros in This Project
This project uses macros conservatively, primarily for constants:

```c
// include/constants.h
#define BOARD_DIM 3
#define BOARD_SIZE (BOARD_DIM * BOARD_DIM)   // Note: parentheses around expression
#define WIN_LENGTH BOARD_DIM
```

Notice `BOARD_SIZE` uses parentheses even though `BOARD_DIM * BOARD_DIM` seems safe. This is
defensive programming — if someone later writes `int x = 2 * BOARD_SIZE`, the parentheses ensure
correct precedence.

#### Include Guards — A Special Macro Pattern
Every header file in this project uses macros for **include guards**:

```c
// include/board.h
#ifndef BOARD_H      // If BOARD_H is not defined...
#define BOARD_H      // ...define it (so this block runs only once)

// ... header contents ...

#endif // BOARD_H
```

This pattern prevents a header from being included multiple times in the same compilation unit,
which would cause "redefinition" errors. You'll learn more about this in
[Lesson 7: Header Files](LESSON_7_Header_Files.md).

#### Multi-Line Macros
Macros can span multiple lines using backslash continuation:

```c
#define PRINT_BOARD_POSITION(board, pos) \
    do { \
        printf("Position %d: ", pos); \
        printf("%c\n", board[pos] == CELL_X ? 'X' : \
                       board[pos] == CELL_O ? 'O' : '.'); \
    } while (0)
```

The `do { ... } while (0)` idiom wraps statement macros so they behave correctly in all contexts
(like after `if` without braces). This is an advanced pattern you'll rarely need to write, but
you should recognise it when reading C code.

> ⚠️ **Debugging note:** When a macro causes an error, the compiler reports the error at the
> *expansion site*, not the macro definition. This can make bugs hard to track down. If you
> suspect a macro issue, try expanding it mentally or replacing it with its expansion temporarily.

