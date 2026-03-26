# Lesson 6: Structs

A `struct` (structure) groups related data together into a single unit. Think of it as a custom type that bundles multiple variables.

## Syntax

```c
typedef struct {
    int x;
    int y;
} Point;

Point p;       // Declare a Point variable
p.x = 10;      // Access members with dot notation
p.y = 20;
```

## Structs in This Project

**Player** - Who is playing (human or computer)?
```c
typedef struct {
    PlayerType type;    // PLAYER_HUMAN or PLAYER_COMPUTER
    CellValue symbol;   // CELL_X or CELL_O
} Player;
```

**GameState** - Everything about the current game:
```c
typedef struct {
    CellValue board[BOARD_SIZE]; // The NxN board (configurable size)
    GameStatus status;           // ONGOING, X_WINS, O_WINS, or DRAW
    int move_count;              // How many moves made
    Player players[2];           // players[0]=X, players[1]=O
    int game_count;              // Which game number in session
    CellValue last_winner;       // Who won the previous game
    Difficulty difficulty;       // DIFFICULTY_EASY or DIFFICULTY_HARD
} GameState;
```

## Accessing Struct Members

**Direct access** (when you have the struct itself):
```c
GameState game;
game.move_count = 0;
game.board[4] = CELL_X;
```

**Pointer access** (when you have a pointer to the struct):
```c
GameState *state = &game;
state->move_count = 0;      // Arrow notation for pointers
state->board[4] = CELL_X;

// Equivalent but more verbose:
(*state).move_count = 0;
```

## Nested Structs

Structs can contain other structs:
```c
// GameState contains an array of Player structs
state->players[0].type = PLAYER_HUMAN;
state->players[0].symbol = CELL_X;
state->players[1].type = PLAYER_COMPUTER;
state->players[1].symbol = CELL_O;
```

## Why Use Structs?

1. **Organization** - Related data stays together
2. **Pass to functions easily** - One parameter instead of many
3. **Return multiple values** - Return a struct with all results
4. **Self-documenting** - Field names explain what data means

See [include/tictactoe.h](../../include/tictactoe.h) for struct definitions.

---

## Next Steps

Continue to [Lesson 7: Header Files](LESSON_7_Header_Files.md) →

← Previous: [Lesson 5: Arrays](LESSON_5_Arrays.md)

← Back to [Learning Guide Index](LEARNING_GUIDE.md)

