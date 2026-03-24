#include "ai_easy.h"
#include "board.h"
#include "win_condition_calculator.h"

#include <assert.h>
#include <stdlib.h>


int ai_easy_get_computer_move(GameState *state) {
    assert(state != NULL);

    int empty[BOARD_SIZE];
    int count = 0;

    for (int i = 0; i < BOARD_SIZE; i++) {
        if (board_is_empty(state->board, i)) {
            empty[count++] = i;
        }
    }

    assert(count > 0); /* should never be called on a full board */
    return empty[rand() % count];
}

