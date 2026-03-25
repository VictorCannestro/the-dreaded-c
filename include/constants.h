#ifndef CONSTANTS_H
#define CONSTANTS_H

/* Board configuration - change BOARD_DIM to support larger boards (e.g., 4, 5) */
#define BOARD_DIM 3
#define BOARD_SIZE (BOARD_DIM * BOARD_DIM)
#define WIN_LENGTH BOARD_DIM  /* Number in a row needed to win */

extern const char X_MARKER;
extern const char O_MARKER;
extern const char EMPTY_MARKER;

typedef enum {
    CELL_EMPTY = 0,
    CELL_X = 1,
    CELL_O = 2
} CellValue;

typedef enum {
    ONGOING = 0,
    X_WINS = 1,
    O_WINS = 2,
    DRAW = 3
} GameStatus;

typedef enum {
    PLAYER_HUMAN = 0,
    PLAYER_COMPUTER = 1
} PlayerType;

typedef enum {
    DIFFICULTY_EASY = 0,
    DIFFICULTY_HARD = 1
} Difficulty;

#endif // CONSTANTS_H
