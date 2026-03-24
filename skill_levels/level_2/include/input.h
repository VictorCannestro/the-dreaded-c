/*
 * Input Module - Header
 *
 * v2 Improvement: All user input in one place.
 * Better error handling than v1.
 */

#ifndef INPUT_H
#define INPUT_H

#include "types.h"

/* Get the human player's move (with validation) */
int input_get_move(const GameState *state);

/* Get symbol choice (X or O) */
CellValue input_get_symbol_choice(void);

/* Ask if player wants to play again */
int input_play_again(void);

/* Clear any remaining input */
void input_clear_buffer(void);

#endif /* INPUT_H */

