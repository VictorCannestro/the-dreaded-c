/*
 * Tic-Tac-Toe - Types and Constants
 *
 * v2 Improvement: Using enums instead of magic numbers/chars
 * This makes the code self-documenting and catches errors at compile time.
 */

#ifndef TYPES_H
#define TYPES_H

/* Board dimensions */
#define BOARD_SIZE 9
#define BOARD_SIDE 3

/* Cell values - instead of char '-', 'X', 'O' */
typedef enum {
    CELL_EMPTY = 0,
    CELL_X = 1,
    CELL_O = 2
} CellValue;

/* Game status - instead of int 0, 1, 2, 3 */
typedef enum {
    STATUS_ONGOING = 0,
    STATUS_X_WINS = 1,
    STATUS_O_WINS = 2,
    STATUS_DRAW = 3
} GameStatus;

/* Player types */
typedef enum {
    PLAYER_HUMAN = 0,
    PLAYER_COMPUTER = 1
} PlayerType;

/* Game state - instead of global variables */
typedef struct {
    CellValue board[BOARD_SIZE];
    CellValue current_player;
    GameStatus status;
    PlayerType x_player_type;  /* Who plays X? */
    PlayerType o_player_type;  /* Who plays O? */
} GameState;

/* Display markers */
extern const char MARKER_X;
extern const char MARKER_O;
extern const char MARKER_EMPTY;

#endif /* TYPES_H */

