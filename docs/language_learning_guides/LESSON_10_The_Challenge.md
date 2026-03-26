# Lesson 10: The Challenge — Building Snake 🐍

Congratulations! You've completed all the foundational lessons. Now it's time to put everything together by studying 
and understanding a **real-world C program**: the SDL Snake game from the official SDL library.

This lesson analyzes `snake.c` — a complete, optimized game in ~300 lines of C. You'll see how all the concepts from 
Lessons 1-9 combine in production code.

---

## The Challenge
Your mission: **Study this code until you can explain every line.** Then, attempt the extension challenges at the end.

---

## Full Source Reference
📄 **Source**: [SDL Snake Demo](https://github.com/libsdl-org/SDL/blob/main/examples/demo/01-snake/snake.c)

---

## Concept Mapping: What You'll Recognize

| Lesson | Concept           | Snake.c Example                               |
|--------|-------------------|-----------------------------------------------|
| 1      | Basic Types       | `char`, `int`, `unsigned`, `float` throughout |
| 2      | Enums             | `SnakeCell`, `SnakeDirection`                 |
| 3      | Pointers          | `SnakeContext *ctx`, `SDL_Window *window`     |
| 4      | Functions         | `static` helper functions, clear naming       |
| 5      | Arrays            | `cells[]` array, 2D→1D conversion             |
| 6      | Structs           | `SnakeContext`, `AppState`                    |
| 7      | Header Files      | `#include <SDL3/SDL.h>`                       |
| 8      | Function Pointers | SDL callback architecture                     |
| 9      | Memory Management | `SDL_calloc`, `SDL_free`                      |

---

## Part 1: Constants and Macros (Lesson 1 & 5)

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

---

## Part 2: Bit Packing — Advanced Array Technique (Lesson 5)
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

---

## Part 3: Enums — Dual Purpose (Lesson 2)

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
    Player players[2];  // Nested struct array
    // ...
} GameState;
```

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

---

## Part 6: Pointer Patterns (Lesson 3)
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

#### Bonus: Add a Makefile
```makefile
CC = gcc
CFLAGS = -Wall -Wextra -Iinclude $(shell sdl3-config --cflags)
LDFLAGS = $(shell sdl3-config --libs)

SRCS = src/main.c src/snake_game.c src/snake_input.c src/snake_renderer.c
OBJS = $(SRCS:.c=.o)

snake: $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(OBJS) snake
```

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

← Previous: [Lesson 9: Memory Management](LESSON_9_Memory_Management.md)

← Back to [Learning Guide Index](LEARNING_GUIDE.md)

