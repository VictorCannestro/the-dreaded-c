#ifndef AI_EASY_H
#define AI_EASY_H

#include "tictactoe.h"

/*
 * Get the computer player's next move using a random strategy.
 * Picks uniformly from all remaining empty cells.
 * Returns a position 0-8, or 4 (center) if no valid moves are found.
 */
int ai_easy_get_computer_move(GameState *state);

#endif /* AI_EASY_H */

