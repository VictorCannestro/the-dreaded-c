# Lesson 2: Enums

An `enum` (enumeration) creates a set of named integer constants. Instead of remembering that `1` means X and `2` means O, you use meaningful names.

## Syntax

```c
typedef enum {
    CELL_EMPTY = 0,   // Explicit values (optional but recommended)
    CELL_X = 1,
    CELL_O = 2
} CellValue;          // Type name

CellValue cell = CELL_X;  // Usage
```

## Why Enums Are Essential

**❌ Without enums (confusing):**
```c
if (board[0] == 1) { ... }    // What is 1? Who knows!
board[4] = 2;                 // Setting to... something?
```

**✅ With enums (self-documenting):**
```c
if (board[0] == CELL_X) { ... }    // Checking for X
board[4] = CELL_O;                 // Placing O at center
```

## All Enums in This Project

**Cell values** (what's in each board position):
```c
typedef enum {
    CELL_EMPTY = 0,
    CELL_X = 1,
    CELL_O = 2
} CellValue;
```

**Game status** (is the game over?):
```c
typedef enum {
    ONGOING = 0,
    X_WINS = 1,
    O_WINS = 2,
    DRAW = 3
} GameStatus;
```

**Player type** (human or computer?):
```c
typedef enum {
    PLAYER_HUMAN = 0,
    PLAYER_COMPUTER = 1
} PlayerType;
```

**Difficulty level**:
```c
typedef enum {
    DIFFICULTY_EASY = 0,
    DIFFICULTY_HARD = 1
} Difficulty;
```

## Clever Enum Trick: Using Values as Indices

```c
// CELL_X = 1, CELL_O = 2
// players[0] = X's info, players[1] = O's info

PlayerType game_get_player_type(GameState *state, CellValue symbol) {
    return state->players[symbol - 1].type;  // CELL_X-1=0, CELL_O-1=1
}
```

See [include/constants.h](../../include/constants.h) for all enum definitions.

---

## Next Steps

Continue to [Lesson 3: Pointers](LESSON_3_Pointers.md) →

← Previous: [Lesson 1: Basic Types](LESSON_1_Basic_Types.md)

← Back to [Learning Guide Index](LEARNING_GUIDE.md)

