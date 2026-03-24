#ifndef AI_H
#define AI_H

#include "tictactoe.h"

/*
 * Get the computer player's next move using minimax algorithm
 * Returns position 0-8, or 4 (center) if no valid moves found
 */
int ai_get_computer_move(GameState *state);

#endif // AI_H
