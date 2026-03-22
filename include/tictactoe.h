#ifndef TICTACTOE_H
#define TICTACTOE_H

#include <stdint.h>

/*
 * Tic-Tac-Toe Game Engine
 * A simple library to manage tic-tac-toe game logic
 */

typedef enum {
    CELL_EMPTY = 0,
    CELL_X = 1,
    CELL_O = 2
} CellValue;

typedef enum {
    GAME_ONGOING = 0,
    GAME_X_WINS = 1,
    GAME_O_WINS = 2,
    GAME_DRAW = 3
} GameStatus;

typedef struct {
    CellValue board[9];  // 3x3 board, row-major order
    GameStatus status;
    int move_count;
} GameState;

/*
 * Initialize a new game
 */
void game_init(GameState *state);

/*
 * Make a move on the board
 * Returns 0 on success, -1 if position is invalid or already occupied
 */
int game_make_move(GameState *state, int position);

/*
 * Get the current status of the game
 */
GameStatus game_get_status(GameState *state);

/*
 * Check if a move is valid
 * Returns 1 if valid, 0 if invalid
 */
int game_is_valid_move(GameState *state, int position);

/*
 * Get whose turn it is (based on move count)
 * Returns CELL_X or CELL_O
 */
CellValue game_get_current_player(GameState *state);

/*
 * Reset the board for a new game
 */
void game_reset(GameState *state);

#endif // TICTACTOE_H
