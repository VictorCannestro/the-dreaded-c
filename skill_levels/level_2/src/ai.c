/*
 * AI Module - Implementation
 *
 * Still uses random moves (like v1), but isolated for easy improvement.
 * The final version will add minimax algorithm here.
 */

#include "ai.h"
#include "board.h"
#include <stdlib.h>
#include <time.h>

static int initialized = 0;

int ai_get_move(const GameState *state) {
    int empty_cells[BOARD_SIZE];
    int empty_count = 0;
    int i;

    /* Initialize random seed once */
    if (!initialized) {
        srand(time(NULL));
        initialized = 1;
    }

    /* Find all empty cells */
    for (i = 0; i < BOARD_SIZE; i++) {
        if (board_is_empty(state->board, i)) {
            empty_cells[empty_count++] = i;
        }
    }

    /* Should never be called with no moves available */
    if (empty_count == 0) {
        return -1;
    }

    /* Pick a random empty cell */
    return empty_cells[rand() % empty_count];
}

