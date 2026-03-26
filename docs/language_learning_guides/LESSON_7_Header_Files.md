# Lesson 7: Header Files

C code is split into two types of files:
- **Header files (`.h`)** - Declarations ("what exists")
- **Source files (`.c`)** - Definitions ("how it works")

## Why Two File Types?

Imagine you want to use `game_make_move()` from another file. You need to know:
- What parameters does it take?
- What does it return?

You **don't** need to know how it's implemented. That's what headers provide.

## Header File Example (`.h`)

```c
// tictactoe.h - Public interface

#ifndef TICTACTOE_H      // Include guard (prevents double-inclusion)
#define TICTACTOE_H

#include "constants.h"   // Types we depend on

// Type definitions
typedef struct {
    CellValue board[9];
    GameStatus status;
    // ...
} GameState;

// Function declarations (no bodies!)
void game_init_session(GameState *state);
int game_make_move(GameState *state, int position);
int game_is_over(GameState *state);

#endif // TICTACTOE_H
```

## Source File Example (`.c`)

```c
// tictactoe.c - Implementation

#include "tictactoe.h"    // Our own header first
#include "board.h"        // Other project headers
#include "ai.h"

#include <stdlib.h>       // Standard library headers
#include <time.h>

// Function definitions (with bodies!)
void game_init_session(GameState *state) {
    if (state == NULL) return;
    board_init(state->board);
    state->status = ONGOING;
    // ...
}

int game_make_move(GameState *state, int position) {
    // Full implementation here
}
```

## Include Guards

```c
#ifndef TICTACTOE_H   // "If TICTACTOE_H is NOT defined..."
#define TICTACTOE_H   // "...define it now"

// Header content here

#endif                // "End of the #ifndef block"
```

**Purpose:** Prevent the same header from being included twice in one compilation, which would cause "redefinition" errors.

**Common mistake:** Copy-pasting a header and forgetting to change the guard name:
```c
// board.h - WRONG!
#ifndef TICTACTOE_H   // ❌ Should be BOARD_H
#define TICTACTOE_H
```

## Include Order Convention

```c
// In tictactoe.c:
#include "tictactoe.h"        // 1. Own header first
#include "board.h"            // 2. Other project headers
#include "win_condition_calculator.h"

#include <assert.h>           // 3. Standard library headers
#include <stdlib.h>
#include <time.h>
```

**Why own header first?** Catches missing includes in the header itself.

## What Goes Where?

| In Header (`.h`) | In Source (`.c`) |
|------------------|------------------|
| Type definitions (`typedef`, `struct`) | Function implementations |
| Function declarations | `static` helper functions |
| `#define` constants | Local variables |
| `extern` variable declarations | `#include` for implementation needs |

See [include/](../../include/) for all header files.

---

## Next Steps

Continue to [Lesson 8: Function Pointers and Abstraction](LESSON_8_Function_Pointers_And_Abstraction.md) →

← Previous: [Lesson 6: Structs](LESSON_6_Structs.md)

← Back to [Learning Guide Index](LEARNING_GUIDE.md)

