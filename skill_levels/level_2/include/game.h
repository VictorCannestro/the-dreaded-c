/*
 * Game Logic - Header
 *
 * v2 Improvement: Game coordination in its own module.
 * Uses GameState struct instead of global variables.
 */

#ifndef GAME_H
#define GAME_H

#include "types.h"

/* Initialize a new game */
void game_init(GameState *state);

/* Make a move and update game status */
int game_make_move(GameState *state, int position);

/* Get the current player */
CellValue game_get_current_player(const GameState *state);

/* Switch to the next player */
void game_switch_player(GameState *state);

/* Check if game is over */
int game_is_over(const GameState *state);

/* Get who plays a symbol (human or computer) */
PlayerType game_get_player_type(const GameState *state, CellValue symbol);

/* Set who plays X and O */
void game_set_players(GameState *state, PlayerType x_type, PlayerType o_type);

#endif /* GAME_H */

