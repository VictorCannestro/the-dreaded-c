#ifndef TICTACTOE_H
#define TICTACTOE_H

#include <stdint.h>
#include "constants.h"


typedef struct {
    PlayerType type;  
    CellValue symbol;   
} Player;

typedef struct {
    CellValue board[9];    // 3x3 board, row-major order
    GameStatus status;
    int move_count;
    Player players[2];     // players[0] is X, players[1] is O
    int game_count;        
    CellValue last_winner; 
} GameState;

/*
 * Initialize a new game session (first game)
 * Sets up players and initial game state
 */
void game_init_session(GameState *state);

/*
 * Start a new game within the session
 * Resets board but keeps player assignments
 */
void game_new_game(GameState *state);

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
 * Get the current player (whose turn it is)
 * Returns CELL_X or CELL_O
 */
CellValue game_get_current_player(GameState *state);

/*
 * Get the player type for a given symbol
 */
PlayerType game_get_player_type(GameState *state, CellValue symbol);

/*
 * Set player types (human vs computer)
 */
void game_set_player_types(GameState *state, PlayerType x_player, PlayerType o_player);

/*
 * Set player symbol choice for the first game
 */
void game_set_human_symbol_choice(GameState *state, CellValue human_symbol);

/*
 * Get computer move (AI logic)
 * Returns position 0-8
 */
int game_get_computer_move(GameState *state);

/*
 * Check if game is over
 */
int game_is_over(GameState *state);

#endif // TICTACTOE_H
