# Lesson 10: The Challenge — Building Snake 🐍

Congratulations! You've completed all the foundational lessons. Now it's time to put everything together by studying 
and understanding a **real-world C program**: the SDL Snake game from the official SDL library.

This lesson analyzes `snake.c` — a complete, optimized game in ~300 lines of C. You'll see how all the concepts from 
Lessons 1-9 combine in production code.

---

## Table of Contents
1. [The Challenge](#the-challenge)
2. [Full Source Reference](#full-source-reference)
3. [Concept Mapping: What You'll Recognize](#concept-mapping-what-youll-recognize)
4. [Part 1: Constants and Macros (Lesson 3)](#part-1-constants-and-macros-lesson-3)
5. [Part 2: Bit Packing — Advanced Array Technique (Lessons 2 & 5)](#part-2-bit-packing--advanced-array-technique-lessons-2--5)
6. [Part 3: Enums — Dual Purpose (Lesson 3)](#part-3-enums--dual-purpose-lesson-3)
7. [Part 4: Structs — Separating Concerns (Lesson 6)](#part-4-structs--separating-concerns-lesson-6)
8. [Part 5: Static Functions — Encapsulation (Lesson 4)](#part-5-static-functions--encapsulation-lesson-4)
9. [Part 6: Pointer Patterns (Lesson 5)](#part-6-pointer-patterns-lesson-5)
10. [Part 7: The Game Loop — SDL Callbacks (Lesson 8)](#part-7-the-game-loop--sdl-callbacks-lesson-8)
11. [Part 8: Memory Management (Lesson 9)](#part-8-memory-management-lesson-9)
12. [Part 9: Game Logic Deep Dive](#part-9-game-logic-deep-dive)
13. [Comprehension Checkpoint](#comprehension-checkpoint)
14. [Practical Exercises](#practical-exercises)
15. [Challenges](#challenges)
16. [Key Takeaways](#key-takeaways)
17. [What's Next?](#whats-next)
18. [Final Words](#final-words)
19. [Self-Assessment Checklist](#self-assessment-checklist)

---

## The Challenge
Your mission: **Study this code until you can explain every line.** Then, attempt the extension challenges at the end.

> 💡 **Active Learning Tip**: Don't just read passively. After each section, try to answer the 
> comprehension questions *before* revealing the answers. This retrieval practice dramatically 
> improves retention.


## Full Source Reference
📄 **Source**: [SDL Snake Demo](https://github.com/libsdl-org/SDL/blob/main/examples/demo/01-snake/snake.c)

**Before continuing**: Open the source file in another tab. Skim it for 5 minutes. Don't try to understand everything — just get familiar with the overall structure.

<details>
<summary>✅ What did you notice? (Click after skimming)</summary>

You should have observed:
- The file is ~300 lines long
- It starts with `#define` macros and `typedef enum`/`struct` declarations
- Many functions are marked `static`
- There are SDL-specific callbacks like `SDL_AppInit`, `SDL_AppIterate`
- The code is well-commented in some places, sparse in others

</details>

---

## Concept Mapping: What You'll Recognize

| Lesson | Concept                | Snake.c Example                                 |
|--------|------------------------|-------------------------------------------------|
| 0      | Basic Types            | `char`, `int`, `unsigned`, `float` throughout   |
| 1      | Control Flow and Loops | `if`/`switch` for input, game loop iteration    |
| 2      | Arrays                 | `cells[]` array, 2D→1D conversion               |
| 3      | Named Constants        | `SnakeCell`, `SnakeDirection`, `#define` macros |
| 4      | Functions              | `static` helper functions, clear naming         |
| 5      | Pointers               | `SnakeContext *ctx`, `SDL_Window *window`       |
| 6      | Structs                | `SnakeContext`, `AppState`                      |
| 7      | Header Files           | `#include <SDL3/SDL.h>`                         |
| 8      | Function Pointers      | SDL callback architecture                       |
| 9      | Memory Management      | `SDL_calloc`, `SDL_free`                        |

### 🧠 Quick Check: Concept Recall
Before diving in, test your knowledge from previous lessons:

<details>
<summary>Q1: What's the difference between <code>#define</code> and <code>const</code>?</summary>

**Answer**: 
- `#define` is a preprocessor directive — text substitution before compilation. No type checking, no memory allocated.
- `const` creates a typed, read-only variable that exists in memory.

Example:
```c
#define MAX_SIZE 100    // Replaced by 100 everywhere before compiling
const int max_size = 100;  // Actual variable, can take its address
```

</details>

<details>
<summary>Q2: What does <code>static</code> mean when applied to a function?</summary>

**Answer**: The function has **internal linkage** — it can only be called from within the same `.c` file. It's hidden from other translation units. This is C's way of making "private" functions.

</details>

<details>
<summary>Q3: What's the purpose of <code>typedef</code> with structs?</summary>

**Answer**: `typedef` creates an alias so you don't need to write `struct` every time:

```c
// Without typedef:
struct Point { int x, y; };
struct Point p1;  // Must use "struct Point"

// With typedef:
typedef struct { int x, y; } Point;
Point p1;  // Can use just "Point"
```

</details>

---

## Part 1: Constants and Macros (Lesson 3)
```c
#define STEP_RATE_IN_MILLISECONDS  125
#define SNAKE_BLOCK_SIZE_IN_PIXELS 24
#define SDL_WINDOW_WIDTH           (SNAKE_BLOCK_SIZE_IN_PIXELS * SNAKE_GAME_WIDTH)
#define SDL_WINDOW_HEIGHT          (SNAKE_BLOCK_SIZE_IN_PIXELS * SNAKE_GAME_HEIGHT)

#define SNAKE_GAME_WIDTH  24U
#define SNAKE_GAME_HEIGHT 18U
#define SNAKE_MATRIX_SIZE (SNAKE_GAME_WIDTH * SNAKE_GAME_HEIGHT)
```

### What's Happening
- **Computed constants**: Window size is calculated from game dimensions
- **The `U` suffix**: `24U` means "unsigned integer literal" — prevents signed/unsigned warnings
- **Configurable design**: Change `SNAKE_GAME_WIDTH` and the entire game scales

### Compare to Tic-Tac-Toe
```c
// Our project uses the same pattern!
#define BOARD_DIM 3
#define BOARD_SIZE (BOARD_DIM * BOARD_DIM)
```

### 📝 Exercise 1.1: Macro Calculation
Given these definitions, calculate the following values *by hand*:

```c
#define SNAKE_GAME_WIDTH  24U
#define SNAKE_GAME_HEIGHT 18U
#define SNAKE_BLOCK_SIZE_IN_PIXELS 24
#define SNAKE_MATRIX_SIZE (SNAKE_GAME_WIDTH * SNAKE_GAME_HEIGHT)
#define SDL_WINDOW_WIDTH  (SNAKE_BLOCK_SIZE_IN_PIXELS * SNAKE_GAME_WIDTH)
```

1. What is `SNAKE_MATRIX_SIZE`?
2. What is `SDL_WINDOW_WIDTH`?
3. If we change `SNAKE_BLOCK_SIZE_IN_PIXELS` to 32, what becomes of `SDL_WINDOW_WIDTH`?

<details>
<summary>🔍 Show Answers</summary>

1. `SNAKE_MATRIX_SIZE` = 24 × 18 = **432** cells
2. `SDL_WINDOW_WIDTH` = 24 × 24 = **576** pixels
3. With `SNAKE_BLOCK_SIZE_IN_PIXELS = 32`: `SDL_WINDOW_WIDTH` = 32 × 24 = **768** pixels

This demonstrates the power of computed constants — changing one value cascades through the entire codebase.

</details>

### 📝 Exercise 1.2: Design Your Own Constants
You're building a Tetris game. Define macros for:
- Game grid: 10 columns, 20 rows
- Block size: 30 pixels
- Total cells
- Window dimensions

<details>
<summary>🔍 Show Solution</summary>

```c
#define TETRIS_COLS          10U
#define TETRIS_ROWS          20U
#define TETRIS_BLOCK_SIZE    30
#define TETRIS_TOTAL_CELLS   (TETRIS_COLS * TETRIS_ROWS)      // 200
#define TETRIS_WINDOW_WIDTH  (TETRIS_BLOCK_SIZE * TETRIS_COLS)  // 300
#define TETRIS_WINDOW_HEIGHT (TETRIS_BLOCK_SIZE * TETRIS_ROWS)  // 600
```

**Why parentheses?** Consider `2 * TETRIS_WINDOW_WIDTH`. Without parentheses:
- `2 * TETRIS_BLOCK_SIZE * TETRIS_COLS` — happens to work
But with subtraction: `TETRIS_WINDOW_WIDTH - 10` without parentheses around the macro could cause bugs.

</details>

---

## Part 2: Bit Packing — Advanced Array Technique (Lessons 2 & 5)
This is the most advanced concept in the file:

```c
#define SNAKE_CELL_MAX_BITS 3U /* floor(log2(SNAKE_CELL_FOOD)) + 1 */
#define SNAKE_CELL_SET_BITS  (~(~0u << SNAKE_CELL_MAX_BITS))
#define SHIFT(x, y) (((x) + ((y) * SNAKE_GAME_WIDTH)) * SNAKE_CELL_MAX_BITS)
```

### The Problem
A 24×18 grid = 432 cells. A naive approach using one byte per cell would take 432 bytes. But each cell only needs 6 possible values (0-5), which fits in 3 bits!

### The Solution: Bit Packing
```c
unsigned char cells[(SNAKE_MATRIX_SIZE * SNAKE_CELL_MAX_BITS) / 8U];
// = (432 * 3) / 8 = 162 bytes (vs 432 bytes naive, or 1,728 bytes if using int!)
```

### How It Works
**Reading a cell:**
```c
SnakeCell snake_cell_at(const SnakeContext *ctx, char x, char y)
{
    const int shift = SHIFT(x, y);           // Bit position in array
    unsigned short range;
    SDL_memcpy(&range, ctx->cells + (shift / 8), sizeof(range));  // Read 2 bytes
    return (SnakeCell)((range >> (shift % 8)) & SNAKE_CELL_SET_BITS);  // Extract 3 bits
}
```

**Writing a cell:**
```c
static void put_cell_at_(SnakeContext *ctx, char x, char y, SnakeCell ct)
{
    const int shift = SHIFT(x, y);
    const int adjust = shift % 8;
    unsigned char *const pos = ctx->cells + (shift / 8);
    unsigned short range;
    SDL_memcpy(&range, pos, sizeof(range));
    range &= ~(SNAKE_CELL_SET_BITS << adjust);  // Clear the 3 bits
    range |= (ct & SNAKE_CELL_SET_BITS) << adjust;  // Set new value
    SDL_memcpy(pos, &range, sizeof(range));
}
```

### Visual Example
```
Cell values: NOTHING=0, SRIGHT=1, SUP=2, SLEFT=3, SDOWN=4, FOOD=5

In binary (3 bits each):
  NOTHING = 000
  SRIGHT  = 001
  SUP     = 010
  SLEFT   = 011
  SDOWN   = 100
  FOOD    = 101

Packed into bytes:
  Byte 0: [cell0:3bits][cell1:3bits][cell2:2bits...]
  Byte 1: [...cell2:1bit][cell3:3bits][cell4:3bits]...
```

### 💡 Why This Matters
This is **memory optimization** — critical for embedded systems, games, and large datasets. You rarely need this, but understanding it demonstrates mastery of bits and arrays.

### 📝 Exercise 2.1: Bit Mask Calculation
The macro `SNAKE_CELL_SET_BITS` creates a mask to extract 3 bits:
```c
#define SNAKE_CELL_SET_BITS  (~(~0u << SNAKE_CELL_MAX_BITS))
```

Work through this step by step (assume 8-bit for simplicity):

1. What is `~0u` in binary?
2. What is `~0u << 3`?
3. What is `~(~0u << 3)`?

<details>
<summary>🔍 Show Step-by-Step Solution</summary>

```
1. ~0u (8-bit example):
   0u = 00000000
   ~0u = 11111111  (all bits set)

2. ~0u << 3:
   11111111 << 3 = 11111000  (shifted left, zeros fill in)

3. ~(~0u << 3):
   ~11111000 = 00000111  (inverted: the low 3 bits are now 1)
```

So `SNAKE_CELL_SET_BITS = 0b111 = 7` — a mask that extracts the lowest 3 bits.

In actual code (32-bit unsigned): `~(~0u << 3) = 0x00000007`

</details>

### 📝 Exercise 2.2: Memory Savings Calculation
A game has a 100×100 grid. Each cell can have one of 10 states (0-9).

1. How many bits are needed per cell? (Hint: what power of 2 is ≥ 10?)
2. How many bytes using naive `unsigned char` array?
3. How many bytes using bit packing?
4. What's the memory savings percentage?

<details>
<summary>🔍 Show Solution</summary>

1. **Bits needed**: 10 states → need 4 bits (2⁴ = 16 ≥ 10, but 2³ = 8 < 10)

2. **Naive approach**: 100 × 100 × 1 byte = **10,000 bytes**

3. **Bit-packed**: 
   - Total bits = 100 × 100 × 4 = 40,000 bits
   - Bytes = 40,000 / 8 = **5,000 bytes**

4. **Savings**: (10,000 - 5,000) / 10,000 = **50% reduction**

If we used `int` (4 bytes) per cell naively, that would be 40,000 bytes — bit packing saves 87.5%!

</details>

### 📝 Exercise 2.3: Trace Through Cell Access
Given position (3, 1) on a 24-wide grid with 3 bits per cell:

```c
#define SHIFT(x, y) (((x) + ((y) * 24)) * 3)
```

Calculate:
1. The bit offset
2. Which byte(s) this cell spans
3. The bit position within the starting byte

<details>
<summary>🔍 Show Solution</summary>

1. **Bit offset**: `SHIFT(3, 1) = (3 + (1 × 24)) × 3 = 27 × 3 = 81`

2. **Byte position**: `81 / 8 = 10` (integer division), so starts in **byte 10**
   - Since we need 3 bits starting at bit 81, and bits 81-83 span positions 1-3 within bytes 10-11

3. **Bit within byte**: `81 % 8 = 1`, so starts at **bit position 1** (the second-lowest bit) of byte 10

Visual (LSB on right, as stored in memory):
```
Byte 10:  [bit7][bit6][bit5][bit4][CELL][CELL][CELL][bit0]
                                   bit3  bit2  bit1
                                    ↑     ↑     ↑
                               3rd bit of cell spans into bit 3
```

The 3-bit cell occupies bit positions 1, 2, and 3 of byte 10 — it fits entirely within one byte in this case.

</details>

---

## Part 3: Enums — Dual Purpose (Lesson 3)
```c
typedef enum
{
    SNAKE_CELL_NOTHING = 0U,
    SNAKE_CELL_SRIGHT = 1U,   // Snake body pointing RIGHT
    SNAKE_CELL_SUP = 2U,      // Snake body pointing UP
    SNAKE_CELL_SLEFT = 3U,    // Snake body pointing LEFT
    SNAKE_CELL_SDOWN = 4U,    // Snake body pointing DOWN
    SNAKE_CELL_FOOD = 5U
} SnakeCell;

typedef enum
{
    SNAKE_DIR_RIGHT,  // = 0
    SNAKE_DIR_UP,     // = 1
    SNAKE_DIR_LEFT,   // = 2
    SNAKE_DIR_DOWN    // = 3
} SnakeDirection;
```

### Clever Design: Direction + 1 = Cell
```c
const SnakeCell dir_as_cell = (SnakeCell)(ctx->next_dir + 1);
// SNAKE_DIR_RIGHT (0) + 1 = SNAKE_CELL_SRIGHT (1)
// SNAKE_DIR_UP (1) + 1 = SNAKE_CELL_SUP (2)
// etc.
```

This avoids a switch statement — the enum values are **designed** to convert with simple arithmetic.

### Compare to Tic-Tac-Toe
We used a similar trick:
```c
// players[symbol - 1] converts CELL_X (1) to index 0
return state->players[symbol - 1].type;
```

### 📝 Exercise 3.1: Enum Design
The `dir + 1` conversion trick relies on specific enum values. Explain why `SNAKE_CELL_NOTHING` must be 0 for this design to work correctly.

<details>
<summary>🔍 Show Answer</summary>

The direction values (0-3) map to cell values (1-4) by adding 1:
- `SNAKE_DIR_RIGHT (0) + 1 = SNAKE_CELL_SRIGHT (1)` ✓
- `SNAKE_DIR_UP (1) + 1 = SNAKE_CELL_SUP (2)` ✓
- etc.

`SNAKE_CELL_NOTHING` **must** be 0 because:
1. It leaves values 1-4 available for direction-based cells
2. Value 5 can be used for `SNAKE_CELL_FOOD` (not a direction)
3. Zero is the natural "empty/nothing" state (matches zeroed memory from `calloc`)

This is intentional enum design — the values aren't arbitrary. The developer chose them to make conversions trivial while reserving 0 for "nothing" and leaving room for non-direction states.

</details>

### 📝 Exercise 3.2: Design Your Own Enum Conversion
Design enums for a Pac-Man game where:
- Ghost states: CHASE, SCATTER, FRIGHTENED, EATEN
- Colors: RED, PINK, CYAN, ORANGE

Make the ghost's color determinable from its index without a switch statement.

<details>
<summary>🔍 Show Solution</summary>

```c
typedef enum {
    GHOST_BLINKY = 0,  // Red ghost
    GHOST_PINKY  = 1,  // Pink ghost
    GHOST_INKY   = 2,  // Cyan ghost
    GHOST_CLYDE  = 3   // Orange ghost
} GhostType;

typedef enum {
    COLOR_RED    = 0,
    COLOR_PINK   = 1,
    COLOR_CYAN   = 2,
    COLOR_ORANGE = 3
} GhostColor;

// Conversion is trivial:
GhostColor get_ghost_color(GhostType ghost) {
    return (GhostColor)ghost;  // Same values!
}

// Or use an array for more complex mappings:
const int RGB_VALUES[][3] = {
    [COLOR_RED]    = {255, 0, 0},
    [COLOR_PINK]   = {255, 184, 255},
    [COLOR_CYAN]   = {0, 255, 255},
    [COLOR_ORANGE] = {255, 184, 82}
};
```

</details>

---

## Part 4: Structs — Separating Concerns (Lesson 6)
```c
typedef struct
{
    unsigned char cells[(SNAKE_MATRIX_SIZE * SNAKE_CELL_MAX_BITS) / 8U];
    char head_xpos;
    char head_ypos;
    char tail_xpos;
    char tail_ypos;
    char next_dir;
    char inhibit_tail_step;   // Growth counter
    unsigned occupied_cells;
} SnakeContext;

typedef struct
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    SnakeContext snake_ctx;   // Nested struct!
    Uint64 last_step;
} AppState;
```

### Design Insight
- **`SnakeContext`**: Pure game logic — no SDL dependencies!
- **`AppState`**: SDL resources + game state

This separation means you could port the game to a different graphics library by only changing `AppState` and the rendering code.

### Compare to Tic-Tac-Toe
```c
typedef struct {
    CellValue board[BOARD_SIZE];
    GameStatus status;
    int move_count;
    Player players[2];  // Nested struct array: players[0] is X, players[1] is O
    // ...
} GameState;
```

### 📝 Exercise 4.1: Struct Memory Analysis
Looking at `SnakeContext`:
```c
typedef struct {
    unsigned char cells[162];  // (432 * 3) / 8
    char head_xpos;
    char head_ypos;
    char tail_xpos;
    char tail_ypos;
    char next_dir;
    char inhibit_tail_step;
    unsigned occupied_cells;
} SnakeContext;
```

1. What's the minimum size of this struct in bytes? (Assume `unsigned` is 4 bytes)
2. Why might the actual size be larger?

<details>
<summary>🔍 Show Answer</summary>

1. **Minimum size**:
   - `cells[162]` = 162 bytes
   - 6 × `char` = 6 bytes
   - `unsigned` = 4 bytes
   - **Total minimum: 172 bytes**

2. **Actual size might be larger due to padding/alignment**:
   - The compiler may add padding bytes to align `occupied_cells` (an `unsigned`) to a 4-byte boundary
   - After 162 + 6 = 168 bytes, we're at an address divisible by 4, so likely no padding needed here
   - But different compilers/platforms may vary

You can check with `printf("sizeof(SnakeContext) = %zu\n", sizeof(SnakeContext));`

</details>

### 📝 Exercise 4.2: Identify the Abstraction Layers
In the Snake code, identify which struct would change if:

1. We switched from SDL to a terminal-based display
2. We changed the game to have walls instead of wrapping
3. We added multiplayer with two snakes

<details>
<summary>🔍 Show Answers</summary>

1. **Terminal display**: Only `AppState` changes
   - Replace `SDL_Window*`, `SDL_Renderer*` with `ncurses` handles
   - `SnakeContext` remains unchanged — it has no rendering knowledge

2. **Walls instead of wrapping**: Only `SnakeContext` logic changes
   - Remove `wrap_around_()` calls
   - Add collision detection with walls
   - `AppState` unchanged (still just renders whatever state exists)

3. **Multiplayer**: Both change
   - `SnakeContext`: Add second head/tail position, or create array of snakes
   - `AppState`: May need to track which snake is which, different controls
   - Better design: Create `Snake` struct, have `GameContext` contain `Snake snakes[2]`

</details>

---

## Part 5: Static Functions — Encapsulation (Lesson 4)
```c
static void set_rect_xy_(SDL_FRect *r, short x, short y) { ... }
static void put_cell_at_(SnakeContext *ctx, char x, char y, SnakeCell ct) { ... }
static int are_cells_full_(SnakeContext *ctx) { ... }
static void new_food_pos_(SnakeContext *ctx) { ... }
static void wrap_around_(char *val, char max) { ... }
```

### What `static` Does
- **File-private**: These functions can only be called from within `snake.c`
- **Encapsulation**: Implementation details are hidden
- **Convention**: Trailing underscore `_` indicates "internal" function

### Public vs Private
```c
// PUBLIC (no static) — can be called from other files
void snake_initialize(SnakeContext *ctx) { ... }
void snake_redir(SnakeContext *ctx, SnakeDirection dir) { ... }
void snake_step(SnakeContext *ctx) { ... }
SnakeCell snake_cell_at(const SnakeContext *ctx, char x, char y) { ... }

// PRIVATE (static) — internal helpers
static void put_cell_at_(...) { ... }
static void new_food_pos_(...) { ... }
```

### 📝 Exercise 5.1: Public vs Private API Design
Look at the public functions. Why is `snake_cell_at` public but `put_cell_at_` private?

<details>
<summary>🔍 Show Answer</summary>

**`snake_cell_at` is public** because:
- External code (renderer) needs to read cell values to draw them
- It's a "getter" — read-only, safe to expose

**`put_cell_at_` is private** because:
- Only the game logic should modify cells
- External code modifying cells directly could corrupt game state
- Changes go through `snake_step` which maintains invariants

This is the **principle of least privilege** — expose only what's necessary.

</details>

### 📝 Exercise 5.2: Identify the API
If `snake.c` were split into a library, which functions would go in the header file?

List them and explain why each should be public.

<details>
<summary>🔍 Show Answer</summary>

**Public API (in `snake_game.h`):**

```c
// Initialize game state
void snake_initialize(SnakeContext *ctx);

// Change direction (called by input handler)
void snake_redir(SnakeContext *ctx, SnakeDirection dir);

// Advance game by one step (called by game loop)
void snake_step(SnakeContext *ctx);

// Read cell value (called by renderer)
SnakeCell snake_cell_at(const SnakeContext *ctx, char x, char y);
```

**Rationale:**
- `snake_initialize`: Game loop needs to start/restart the game
- `snake_redir`: Input handler needs to pass direction changes
- `snake_step`: Game loop advances time
- `snake_cell_at`: Renderer needs to read (but not write!) state

Everything else is implementation detail.

</details>

---

## Part 6: Pointer Patterns (Lesson 5)
### Pattern 1: Modifying Through Pointers
```c
static void wrap_around_(char *val, char max)
{
    if (*val < 0) {
        *val = max - 1;
    } else if (*val > max - 1) {
        *val = 0;
    }
}

// Usage:
wrap_around_(&ctx->tail_xpos, SNAKE_GAME_WIDTH);
```

> **Subtle detail**: `SNAKE_GAME_WIDTH` is `24U` (unsigned), but `max` is `char`. C implicitly 
> converts the unsigned value to char. This works because 24 fits in a signed char (-128 to 127).

### Pattern 2: Accessing Nested Struct Members
```c
AppState *as = (AppState *)appstate;
SnakeContext *ctx = &as->snake_ctx;  // Pointer to nested struct
ctx->head_xpos++;  // Modify through pointer chain
```

### Pattern 3: Output Parameters
```c
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    AppState *as = (AppState *)SDL_calloc(1, sizeof(AppState));
    *appstate = as;  // Write to caller's pointer
    // ...
}
```

### 📝 Exercise 6.1: Pointer Tracing
Given:
```c
AppState *as = (AppState *)appstate;
SnakeContext *ctx = &as->snake_ctx;
ctx->head_xpos++;
```

Which of these are equivalent to `ctx->head_xpos++`?

A. `(*ctx).head_xpos++`  
B. `as->snake_ctx.head_xpos++`  
C. `(&as->snake_ctx)->head_xpos++`  
D. `appstate->snake_ctx.head_xpos++`  

<details>
<summary>🔍 Show Answer</summary>

**A, B, and C are all equivalent.** ✅

- **A**: `(*ctx).head_xpos++` — Dereference pointer, access member
- **B**: `as->snake_ctx.head_xpos++` — Direct path through `as`
- **C**: `(&as->snake_ctx)->head_xpos++` — Take address, use arrow (redundant but valid)
- **D is WRONG**: `appstate` is `void*`, not `AppState*`. You can't dereference `void*`.

</details>

### 📝 Exercise 6.2: Write an Output Parameter Function
Write a function `get_snake_head` that takes a `const SnakeContext*` and two `char*` output parameters for x and y positions.

<details>
<summary>🔍 Show Solution</summary>

```c
void get_snake_head(const SnakeContext *ctx, char *out_x, char *out_y)
{
    if (ctx == NULL || out_x == NULL || out_y == NULL) {
        return;  // Defensive programming
    }
    *out_x = ctx->head_xpos;
    *out_y = ctx->head_ypos;
}

// Usage:
char hx, hy;
get_snake_head(&snake_ctx, &hx, &hy);
printf("Head at (%d, %d)\n", hx, hy);
```

**Why use output parameters instead of returning a struct?**
- Avoids creating a temporary struct
- Can return multiple values
- Common C pattern (less common in C++ due to std::pair/tuple)

</details>

---

## Part 7: The Game Loop — SDL Callbacks (Lesson 8)
SDL3 uses a callback-based architecture (similar to function pointers):

```c
#define SDL_MAIN_USE_CALLBACKS 1

// SDL calls these functions — you implement them
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]);
SDL_AppResult SDL_AppIterate(void *appstate);  // Called every frame
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event);
void SDL_AppQuit(void *appstate, SDL_AppResult result);
```

### The Frame Loop
```c
SDL_AppResult SDL_AppIterate(void *appstate)
{
    AppState *as = (AppState *)appstate;
    const Uint64 now = SDL_GetTicks();
    
    // Fixed timestep: run logic at consistent rate
    while ((now - as->last_step) >= STEP_RATE_IN_MILLISECONDS) {
        snake_step(&as->snake_ctx);
        as->last_step += STEP_RATE_IN_MILLISECONDS;
    }
    
    // Render
    SDL_RenderClear(as->renderer);
    // ... draw cells ...
    SDL_RenderPresent(as->renderer);
    
    return SDL_APP_CONTINUE;
}
```

### Compare to Tic-Tac-Toe
Our function pointer interface achieves similar abstraction:
```c
typedef struct UserInterface {
    void (*display_board)(const GameState *state);
    int  (*get_move)(GameState *game);
    // ...
} UserInterface;
```

### 📝 Exercise 7.1: Fixed Timestep Understanding
The game loop uses this pattern:
```c
while ((now - as->last_step) >= STEP_RATE_IN_MILLISECONDS) {
    snake_step(&as->snake_ctx);
    as->last_step += STEP_RATE_IN_MILLISECONDS;
}
```

Why a `while` loop instead of `if`? What problem does this solve?

<details>
<summary>🔍 Show Answer</summary>

**The `while` loop handles lag spikes.**

If the game freezes for 500ms (maybe loading a texture), and `STEP_RATE_IN_MILLISECONDS` is 125:
- `now - last_step` could be 500
- An `if` would run only ONE step, then rendering — the snake would slow down
- The `while` runs `500 / 125 = 4` steps to catch up, keeping game time accurate

This is called a **fixed timestep with catch-up**. It ensures:
- Game logic runs at consistent rate regardless of frame rate
- Physics/movement is deterministic
- Slow frames don't slow the game (just makes it choppy visually)

</details>

### 📝 Exercise 7.2: Map the Callbacks
Match each SDL callback to its Tic-Tac-Toe equivalent concept:

| SDL Callback     | Tic-Tac-Toe Concept |
|------------------|---------------------|
| `SDL_AppInit`    | ?                   |
| `SDL_AppEvent`   | ?                   |
| `SDL_AppIterate` | ?                   |
| `SDL_AppQuit`    | ?                   |

<details>
<summary>🔍 Show Answers</summary>

| SDL Callback     | Tic-Tac-Toe Equivalent                      |
|------------------|---------------------------------------------|
| `SDL_AppInit`    | `init_game_state()` — set up initial state  |
| `SDL_AppEvent`   | `ui->get_move()` — handle user input        |
| `SDL_AppIterate` | Main game loop body — update state, render  |
| `SDL_AppQuit`    | (Not needed — stack allocation, no cleanup) |

The key difference: Snake is **event-driven** (SDL calls you), while Tic-Tac-Toe uses a **polling loop** (you call input functions).

</details>

---

## Part 8: Memory Management (Lesson 9)
### Allocation
```c
AppState *as = (AppState *)SDL_calloc(1, sizeof(AppState));
if (!as) {
    return SDL_APP_FAILURE;  // Always check!
}
*appstate = as;
```

### Cleanup
```c
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    if (joystick) {
        SDL_CloseJoystick(joystick);
    }
    if (appstate != NULL) {
        AppState *as = (AppState *)appstate;
        SDL_DestroyRenderer(as->renderer);  // Release SDL resources
        SDL_DestroyWindow(as->window);
        SDL_free(as);  // Free allocated memory
    }
}
```

### Key Practices
1. **Check allocation success**: `if (!as) return SDL_APP_FAILURE;`
2. **NULL checks before free**: `if (appstate != NULL)`
3. **Release in reverse order**: Resources freed in opposite order of creation
4. **Use matching functions**: `SDL_calloc` → `SDL_free`, not `malloc` → `SDL_free`

### 📝 Exercise 8.1: Spot the Memory Bugs
Find the memory bugs in this code (SDL function signatures simplified for clarity):

```c
void create_game(void) {
    AppState *state = malloc(sizeof(AppState));
    state->window = SDL_CreateWindow("Game", 800, 600, 0);
    state->renderer = SDL_CreateRenderer(state->window, NULL);
    
    play_game(state);
    
    free(state);
}
```

<details>
<summary>🔍 Show All Bugs</summary>

**Bug 1**: No NULL check after `malloc`
```c
AppState *state = malloc(sizeof(AppState));
if (state == NULL) return;  // MISSING!
```

**Bug 2**: No NULL check after `SDL_CreateWindow`
```c
state->window = SDL_CreateWindow(...);
if (state->window == NULL) { free(state); return; }  // MISSING!
```

**Bug 3**: No NULL check after `SDL_CreateRenderer`
```c
state->renderer = SDL_CreateRenderer(...);
if (state->renderer == NULL) { 
    SDL_DestroyWindow(state->window); 
    free(state); 
    return;
}  // MISSING!
```

**Bug 4**: SDL resources not freed before `free(state)`
```c
// MISSING:
SDL_DestroyRenderer(state->renderer);
SDL_DestroyWindow(state->window);
free(state);
```

**Corrected version:**
```c
void create_game(void) {
    AppState *state = malloc(sizeof(AppState));
    if (!state) return;
    
    state->window = SDL_CreateWindow("Game", 800, 600, 0);
    if (!state->window) { free(state); return; }
    
    state->renderer = SDL_CreateRenderer(state->window, NULL);
    if (!state->renderer) { 
        SDL_DestroyWindow(state->window);
        free(state);
        return;
    }
    
    play_game(state);
    
    SDL_DestroyRenderer(state->renderer);
    SDL_DestroyWindow(state->window);
    free(state);
}
```

</details>

### 📝 Exercise 8.2: LIFO Cleanup Order
Why do we free resources in reverse order of creation?

```c
// Creation:
state = malloc(...);           // 1
state->window = SDL_CreateWindow(...);  // 2
state->renderer = SDL_CreateRenderer(state->window);  // 3

// Cleanup (reverse):
SDL_DestroyRenderer(state->renderer);  // 3
SDL_DestroyWindow(state->window);      // 2
free(state);                           // 1
```

<details>
<summary>🔍 Show Answer</summary>

**Dependencies require reverse order.**

The renderer *depends on* the window (it was created from the window). Destroying the window first would leave the renderer in an invalid state — using it would be undefined behavior.

Think of it like Russian nesting dolls:
- You can't close the outer doll until the inner one is removed
- You can't free `state` while it still "contains" valid pointers to active resources

General rule: **Last In, First Out (LIFO)** for cleanup.

</details>

---

## Part 9: Game Logic Deep Dive
### Initialization
```c
void snake_initialize(SnakeContext *ctx)
{
    int i;
    SDL_zeroa(ctx->cells);  // Clear entire grid
    ctx->head_xpos = ctx->tail_xpos = SNAKE_GAME_WIDTH / 2;  // Center
    ctx->head_ypos = ctx->tail_ypos = SNAKE_GAME_HEIGHT / 2;
    ctx->next_dir = SNAKE_DIR_RIGHT;
    ctx->inhibit_tail_step = ctx->occupied_cells = 4;  // Start with length 4
    --ctx->occupied_cells;
    put_cell_at_(ctx, ctx->tail_xpos, ctx->tail_ypos, SNAKE_CELL_SRIGHT);
    for (i = 0; i < 4; i++) {
        new_food_pos_(ctx);
        ++ctx->occupied_cells;
    }
}
```

> **Note**: The variable `int i` is declared at the top of the function. This is C89 style — all 
> variables must be declared before any statements. Modern C (C99+) allows `for (int i = 0; ...)` 
> but SDL code maintains broad compatibility.

### Direction Change (with 180° Prevention)
```c
void snake_redir(SnakeContext *ctx, SnakeDirection dir)
{
    SnakeCell ct = snake_cell_at(ctx, ctx->head_xpos, ctx->head_ypos);
    // Can't reverse direction (would immediately collide with self)
    if ((dir == SNAKE_DIR_RIGHT && ct != SNAKE_CELL_SLEFT) ||
        (dir == SNAKE_DIR_UP && ct != SNAKE_CELL_SDOWN) ||
        (dir == SNAKE_DIR_LEFT && ct != SNAKE_CELL_SRIGHT) ||
        (dir == SNAKE_DIR_DOWN && ct != SNAKE_CELL_SUP)) {
        ctx->next_dir = dir;
    }
}
```

### The Step Function (Simplified)
```c
void snake_step(SnakeContext *ctx)
{
    const SnakeCell dir_as_cell = (SnakeCell)(ctx->next_dir + 1);
    SnakeCell ct;
    char prev_xpos, prev_ypos;
    
    // 1. Move tail (unless growing)
    if (--ctx->inhibit_tail_step == 0) {
        ++ctx->inhibit_tail_step;
        ct = snake_cell_at(ctx, ctx->tail_xpos, ctx->tail_ypos);
        put_cell_at_(ctx, ctx->tail_xpos, ctx->tail_ypos, SNAKE_CELL_NOTHING);
        // Update tail position based on direction stored in cell...
        // (switch statement moves tail_xpos/tail_ypos)
        wrap_around_(&ctx->tail_xpos, SNAKE_GAME_WIDTH);
        wrap_around_(&ctx->tail_ypos, SNAKE_GAME_HEIGHT);
    }
    
    // 2. Move head
    prev_xpos = ctx->head_xpos;
    prev_ypos = ctx->head_ypos;
    // (switch statement moves head_xpos/head_ypos based on next_dir)
    wrap_around_(&ctx->head_xpos, SNAKE_GAME_WIDTH);
    wrap_around_(&ctx->head_ypos, SNAKE_GAME_HEIGHT);
    
    // 3. Check collisions
    ct = snake_cell_at(ctx, ctx->head_xpos, ctx->head_ypos);
    if (ct != SNAKE_CELL_NOTHING && ct != SNAKE_CELL_FOOD) {
        snake_initialize(ctx);  // Hit self = restart
        return;
    }
    
    // 4. Update cells
    put_cell_at_(ctx, prev_xpos, prev_ypos, dir_as_cell);
    put_cell_at_(ctx, ctx->head_xpos, ctx->head_ypos, dir_as_cell);
    
    // 5. Handle food
    if (ct == SNAKE_CELL_FOOD) {
        if (are_cells_full_(ctx)) {
            snake_initialize(ctx);  // Won the game!
            return;
        }
        new_food_pos_(ctx);
        ++ctx->inhibit_tail_step;  // Grow by delaying tail
        ++ctx->occupied_cells;
    }
}
```

### 📝 Exercise 9.1: Trace the Growth Mechanic
The snake grows by "inhibiting" tail movement. Trace what happens when the snake eats food:

1. What is `inhibit_tail_step` before eating?
2. What happens to it when food is eaten?
3. What effect does this have on the next N steps?

<details>
<summary>🔍 Show Answer</summary>

**Normal state**: `inhibit_tail_step = 1`

**When food is eaten**: `++ctx->inhibit_tail_step` → becomes 2

**Next step**:
- `if (--ctx->inhibit_tail_step == 0)` → `2 - 1 = 1`, not zero, so tail does NOT move
- The head moves forward but tail stays → snake is now 1 cell longer!
- After decrement, `inhibit_tail_step = 1` again

**Key insight**: Instead of tracking snake body positions, this code stores *direction* in each cell and just tracks head/tail positions. Growing is simply delaying the tail. Elegant!

</details>

### 📝 Exercise 9.2: Understand the 180° Prevention
Why does the direction check look at the *current cell* rather than `next_dir`?

```c
SnakeCell ct = snake_cell_at(ctx, ctx->head_xpos, ctx->head_ypos);
if (dir == SNAKE_DIR_RIGHT && ct != SNAKE_CELL_SLEFT) ...
```

<details>
<summary>🔍 Show Answer</summary>

**The cell stores the actual direction the snake traveled to get there**, not what the player requested.

Consider: 
1. Snake is moving RIGHT (head cell contains `SNAKE_CELL_SRIGHT`)
2. Player presses UP → `next_dir = SNAKE_DIR_UP` (valid, 90° turn)
3. Player quickly presses LEFT before the next step

If we checked `next_dir`, we'd think the snake is going UP and allow LEFT.
But the snake hasn't *actually* moved up yet! The head is still pointing RIGHT.
Allowing LEFT would cause immediate reversal.

By checking the **cell** (actual state), we prevent reversal regardless of how fast the player presses keys.

</details>

---

## Comprehension Checkpoint
Before moving to challenges, test your understanding of the complete system:

### 🎯 Quick Quiz
<details>
<summary>Q1: Why does <code>SnakeContext</code> have no SDL types, but <code>AppState</code> does?</summary>

**Answer**: Separation of concerns. `SnakeContext` is pure game logic that could work with any renderer (terminal, web, etc.). `AppState` holds SDL-specific resources. This makes the game logic portable and testable without graphics.

</details>

<details>
<summary>Q2: The snake is drawn by iterating through ALL cells and checking their values. Why not store a list of snake body positions?</summary>

**Answer**: The bit-packed grid approach:
1. **Uses fixed memory** — no dynamic allocation during gameplay
2. **O(1) cell access** — check any position instantly for collision
3. **Implicitly stores direction** — rendering knows which way body segments point
4. **Simple growth** — just delay tail, no list modification

A position list would need dynamic resizing and O(n) collision checks.

</details>

<details>
<summary>Q3: What happens if you call <code>snake_redir()</code> multiple times between steps?</summary>

**Answer**: Only the last call matters. `next_dir` gets overwritten each time (if the direction is valid). This is why the 180° check uses the cell value, not `next_dir` — multiple rapid inputs can't trick it.

</details>

<details>
<summary>Q4: Why use <code>SDL_calloc</code> instead of <code>malloc</code>?</summary>

**Answer**: `calloc` zeros the memory. This ensures all fields start at known values (0). The game relies on cells being `SNAKE_CELL_NOTHING` (0) initially. With `malloc`, you'd need to manually zero everything.

</details>

---

## Practical Exercises
These exercises build upon the Snake code. Complete them in order.

### Exercise A: Read and Annotate (30 minutes)
**Goal**: Deep understanding through annotation.

1. Open the [full source](https://github.com/libsdl-org/SDL/blob/main/examples/demo/01-snake/snake.c)
2. Copy it to a local file
3. Add a comment above EVERY function explaining:
   - What it does (one sentence)
   - Its inputs and outputs
   - Which lesson concept it demonstrates

<details>
<summary>🔍 Show Example Annotation</summary>

```c
/*
 * Lesson 4 (Functions), Lesson 5 (Pointers)
 * 
 * Wraps a coordinate value to stay within bounds [0, max-1].
 * Uses wrap-around (toroidal) logic: going past the edge appears on opposite side.
 *
 * Input:  val - pointer to coordinate to modify
 *         max - exclusive upper bound
 * Output: *val is modified in-place
 *
 * Why pointer? To modify the caller's variable directly.
 */
static void wrap_around_(char *val, char max)
{
    if (*val < 0) {
        *val = max - 1;  // Went past left/top → appear at right/bottom
    } else if (*val > max - 1) {
        *val = 0;        // Went past right/bottom → appear at left/top
    }
}
```

</details>

---

### Exercise B: Implement a Missing Feature (45 minutes)
**Goal**: Modify real code to add functionality.

**Task**: Add a "speed increase" feature. Every 5 food items eaten, the game should speed up by 10%.

**Hints**:
1. Where is the step rate stored?
2. How do you track food eaten?
3. When should speed change?

<details>
<summary>🔍 Show Step-by-Step Solution</summary>

**Step 1**: Add a score tracker to `SnakeContext`:
```c
typedef struct {
    // ... existing fields ...
    unsigned score;        // NEW: Track food eaten
    unsigned step_rate;    // NEW: Current step rate (dynamic)
} SnakeContext;
```

**Step 2**: Initialize in `snake_initialize`:
```c
void snake_initialize(SnakeContext *ctx) {
    // ... existing code ...
    ctx->score = 0;
    ctx->step_rate = STEP_RATE_IN_MILLISECONDS;
}
```

**Step 3**: Update when food is eaten in `snake_step`:
```c
if (ct == SNAKE_CELL_FOOD) {
    // ... existing food handling ...
    ++ctx->score;
    
    // Speed up every 5 points
    if (ctx->score % 5 == 0 && ctx->step_rate > 50) {  // Min 50ms
        ctx->step_rate = (ctx->step_rate * 90) / 100;  // 10% faster
    }
}
```

**Step 4**: Use dynamic rate in `SDL_AppIterate`:
```c
while ((now - as->last_step) >= as->snake_ctx.step_rate) {  // Changed!
    snake_step(&as->snake_ctx);
    as->last_step += as->snake_ctx.step_rate;              // Changed!
}
```

</details>

---

### Exercise C: Unit Test Design (30 minutes)
**Goal**: Think about testability.

Without writing actual test code, describe how you would test these functions:

1. `wrap_around_`
2. `snake_redir`
3. `snake_step` (specifically: eating food)

<details>
<summary>🔍 Show Test Descriptions</summary>

**1. Testing `wrap_around_`:**
```
Test cases:
- val=0, max=10 → val stays 0 (no wrap needed)
- val=-1, max=10 → val becomes 9 (wrap left)
- val=10, max=10 → val becomes 0 (wrap right)
- val=5, max=10 → val stays 5 (middle, no wrap)
- val=-5, max=10 → val becomes 9 (any negative wraps to max-1)

Setup: Just need a char variable, no SnakeContext needed
Verify: Check *val after call
```

**2. Testing `snake_redir`:**
```
Test cases:
- Moving RIGHT, request UP → allowed (90° turn)
- Moving RIGHT, request LEFT → denied (180° reversal)
- Moving UP, request DOWN → denied (180° reversal)
- Moving UP, request LEFT → allowed (90° turn)

Setup: Create SnakeContext, set head cell to known direction
Verify: Check ctx->next_dir changed (or didn't)
```

**3. Testing food consumption in `snake_step`:**
```
Test cases:
- Snake head moves onto FOOD cell → occupied_cells increases
- Snake head moves onto FOOD cell → inhibit_tail_step increases
- Snake head moves onto FOOD cell → new food appears elsewhere

Setup: 
  - Create SnakeContext with snake at position (5,5)
  - Place food at (6,5)
  - Set direction to RIGHT
Verify:
  - Call snake_step once
  - Check occupied_cells == previous + 1
  - Check inhibit_tail_step > 1
  - Check a FOOD cell exists somewhere (not at 6,5)
```

The fact that `SnakeContext` has no SDL dependencies means we can unit test it without graphics!

</details>

---

### Exercise D: Mini Implementation (60 minutes)
**Goal**: Apply the patterns to new code.

Implement a simplified `wrap_around_` function and test it:

```c
// File: wrap_test.c
#include <stdio.h>
#include <assert.h>

// Your implementation here
static void wrap_around_(char *val, char max) {
    // TODO: Implement
}

int main(void) {
    char test;
    
    // Test 1: No wrap needed
    test = 5;
    wrap_around_(&test, 10);
    assert(test == 5);
    printf("Test 1 passed: middle value unchanged\n");
    
    // Test 2: Wrap past right edge
    test = 10;
    wrap_around_(&test, 10);
    assert(test == 0);
    printf("Test 2 passed: wrap right->left\n");
    
    // Test 3: Wrap past left edge
    test = -1;
    wrap_around_(&test, 10);
    assert(test == 9);
    printf("Test 3 passed: wrap left->right\n");
    
    // Add more tests!
    
    printf("\nAll tests passed!\n");
    return 0;
}
```

Compile and run: `gcc wrap_test.c -o wrap_test && ./wrap_test`

<details>
<summary>🔍 Show Complete Solution</summary>

```c
#include <stdio.h>
#include <assert.h>

static void wrap_around_(char *val, char max) {
    if (*val < 0) {
        *val = max - 1;
    } else if (*val > max - 1) {
        *val = 0;
    }
}

int main(void) {
    char test;
    
    // Test 1: No wrap needed (middle)
    test = 5;
    wrap_around_(&test, 10);
    assert(test == 5);
    printf("Test 1 passed: middle value unchanged\n");
    
    // Test 2: No wrap needed (left edge)
    test = 0;
    wrap_around_(&test, 10);
    assert(test == 0);
    printf("Test 2 passed: left edge unchanged\n");
    
    // Test 3: No wrap needed (right edge)
    test = 9;
    wrap_around_(&test, 10);
    assert(test == 9);
    printf("Test 3 passed: right edge unchanged\n");
    
    // Test 4: Wrap past right
    test = 10;
    wrap_around_(&test, 10);
    assert(test == 0);
    printf("Test 4 passed: wrap right->left\n");
    
    // Test 5: Wrap past left
    test = -1;
    wrap_around_(&test, 10);
    assert(test == 9);
    printf("Test 5 passed: wrap left->right\n");
    
    // Test 6: Way past right (wraps any value > max-1)
    test = 15;
    wrap_around_(&test, 10);
    assert(test == 0);  // Any value >= max wraps to 0 (not modular arithmetic)
    printf("Test 6 passed: far right wraps to 0\n");
    
    // Test 7: Edge case - max of 1
    test = 0;
    wrap_around_(&test, 1);
    assert(test == 0);
    printf("Test 7 passed: single-cell grid\n");
    
    test = 1;
    wrap_around_(&test, 1);
    assert(test == 0);
    printf("Test 8 passed: single-cell wrap\n");
    
    printf("\nAll tests passed!\n");
    return 0;
}
```

</details>

---

## Challenges
Now that we've gone through the code, try these extensions:

### 🟢 Easy
1. **Change the colors**: Modify the RGB values in rendering code
2. **Adjust speed**: Change `STEP_RATE_IN_MILLISECONDS`
3. **Resize the grid**: Modify `SNAKE_GAME_WIDTH` and `SNAKE_GAME_HEIGHT`

### 🟡 Medium
4. **Add a score counter**: Display current snake length
5. **Add WASD controls**: Extend `handle_key_event_` for additional keys
6. **Pause functionality**: Add a pause state toggled by spacebar

### 🔴 Hard
7. **High score persistence**: Save/load high score to a file
8. **Two-player mode**: Two snakes, different controls
9. **AI snake**: Implement pathfinding to chase food

### 🟣 Expert
10. **Port to terminal**: Replace SDL rendering with `ncurses` — keep the same `SnakeContext` logic!

---

### 🏗️ Architecture Challenge: Modularize the God File
`snake.c` is a **god file** — it contains everything in one place: constants, data structures, game 
logic, input handling, rendering, and SDL lifecycle. This works for a demo, but doesn't scale.

**Your mission**: Refactor `snake.c` into a well-organized multi-file project.

#### Suggested File Structure
```
snake/
├── include/
│   ├── snake_game.h        # SnakeContext, SnakeCell, SnakeDirection, game logic API
│   ├── snake_constants.h   # All #define constants
│   └── snake_renderer.h    # Rendering function declarations
├── src/
│   ├── main.c              # SDL lifecycle (AppInit, AppQuit), minimal glue code
│   ├── snake_game.c        # Pure game logic (initialize, step, redir)
│   ├── snake_input.c       # Input handling (handle_key_event_, handle_hat_event_)
│   └── snake_renderer.c    # All SDL rendering code
└── Makefile
```

#### Step-by-Step Refactoring

**Step 1: Extract constants** → `snake_constants.h`
```c
#ifndef SNAKE_CONSTANTS_H
#define SNAKE_CONSTANTS_H

#define STEP_RATE_IN_MILLISECONDS  125
#define SNAKE_BLOCK_SIZE_IN_PIXELS 24
#define SNAKE_GAME_WIDTH  24U
#define SNAKE_GAME_HEIGHT 18U
// ... all other constants

#endif
```

**Step 2: Extract game logic** → `snake_game.h` / `snake_game.c`
- Move `SnakeCell`, `SnakeDirection`, `SnakeContext` to the header
- Move `snake_initialize`, `snake_step`, `snake_redir`, `snake_cell_at` to the source
- Keep `put_cell_at_`, `new_food_pos_`, `wrap_around_`, `are_cells_full_` as `static` in `snake_game.c`

**Step 3: Extract rendering** → `snake_renderer.h` / `snake_renderer.c`
```c
// snake_renderer.h
#ifndef SNAKE_RENDERER_H
#define SNAKE_RENDERER_H

#include <SDL3/SDL.h>
#include "snake_game.h"

void snake_render(SDL_Renderer *renderer, const SnakeContext *ctx);

#endif
```

**Step 4: Extract input handling** → `snake_input.c`
- Move `handle_key_event_` and `handle_hat_event_`
- These need access to `SnakeContext` and call `snake_redir`

**Step 5: Slim down main.c**
- Only SDL lifecycle: `SDL_AppInit`, `SDL_AppEvent`, `SDL_AppIterate`, `SDL_AppQuit`
- Calls into other modules for actual work

#### Why This Matters

| Single File (God File)           | Modular Design                          |
|----------------------------------|-----------------------------------------|
| Hard to find specific code       | Each file has one responsibility        |
| Can't reuse game logic           | `snake_game.c` works with any renderer  |
| Merge conflicts in teams         | Team members edit different files       |
| Difficult to test                | Can unit test game logic without SDL    |
| Everything recompiles on change  | Only changed modules recompile          |

#### Verification Checklist
- [ ] Game still compiles and runs identically
- [ ] No function defined in a header (only declarations)
- [ ] Each `.c` file includes only what it needs
- [ ] `snake_game.c` has **zero** SDL includes
- [ ] All `static` functions remain private to their module
- [ ] Include guards in every header

This challenge teaches you **the same skills used in the Tic-Tac-Toe project** — where we separated 
`board.c`, `tictactoe.c`, `ui_cli.c`, and `win_condition_calculator.c` into focused modules.

---

## Key Takeaways

| Principle                        | Snake Example                   | Tic-Tac-Toe Example             |
|----------------------------------|---------------------------------|---------------------------------|
| **Separate logic from display**  | `SnakeContext` has no SDL code  | `GameState` has no printf       |
| **Use enums for states**         | `SnakeCell`, `SnakeDirection`   | `CellValue`, `GameStatus`       |
| **Constants make code flexible** | Grid size is configurable       | `BOARD_DIM` scales the game     |
| **Static hides implementation**  | Helper functions are private    | Internal functions use `static` |
| **Always check allocations**     | `if (!as) return FAILURE`       | NULL checks throughout          |
| **Clean up resources**           | `SDL_free`, `SDL_DestroyWindow` | N/A (stack-allocated)           |

---

## What's Next?

You've now seen two complete C programs:
- **Tic-Tac-Toe**: Turn-based, text UI, AI with minimax
- **Snake**: Real-time, graphical UI, game loop

### Suggested Projects
1. **Tetris** — Combines Snake's real-time loop with more complex piece logic
2. **Pong** — Add simple physics and collision detection
3. **Roguelike** — Procedural generation, inventory systems
4. **Your own game** — Apply these patterns to your idea!

---

## Final Words
C is challenging because it gives you direct control over memory and hardware. That same power makes it fast, portable,
and foundational to operating systems, game engines, and embedded devices.

You've now been exposed to:
- How data is laid out in memory (arrays, structs, bit packing)
- How to manage that memory safely (malloc/free, NULL checks)
- How to organize code cleanly (headers, static, function pointers)
- How to build real programs (game loops, event handling, rendering)

**Keep building. Keep learning. You've got this.** 

---

## Self-Assessment Checklist
Before moving on, honestly assess your understanding:

| Concept                              | Can Explain | Can Implement | Need Review |
|--------------------------------------|:-----------:|:-------------:|:-----------:|
| Computed `#define` macros            |      ☐      |       ☐       |      ☐      |
| Bit masking basics                   |      ☐      |       ☐       |      ☐      |
| Enum design for easy conversion      |      ☐      |       ☐       |      ☐      |
| Struct separation (logic vs display) |      ☐      |       ☐       |      ☐      |
| `static` for encapsulation           |      ☐      |       ☐       |      ☐      |
| Output parameters with pointers      |      ☐      |       ☐       |      ☐      |
| Callback-based architecture          |      ☐      |       ☐       |      ☐      |
| Fixed timestep game loop             |      ☐      |       ☐       |      ☐      |
| Allocation + NULL checks             |      ☐      |       ☐       |      ☐      |
| Cleanup in reverse order             |      ☐      |       ☐       |      ☐      |

**If you have more than 2 items in "Need Review"**, revisit those specific lesson sections before attempting the challenges.

---

← Previous: [Lesson 9: Memory Management](LESSON_9_Memory_Management.md)

← Back to [Learning Guide Index](LEARNING_GUIDE.md)

