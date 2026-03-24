/*
 * AI Module - Header
 *
 * v2 Improvement: AI logic separated from game logic.
 * Still random moves, but isolated for future improvement.
 */

#ifndef AI_H
#define AI_H

#include "types.h"

/* Get the computer's move (random selection) */
int ai_get_move(const GameState *state);

#endif /* AI_H */

