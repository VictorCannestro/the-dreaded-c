#include "tictactoe.h"
#include "board.h"
#include "win_condition_calculator.h"
#include "ai.h"
#include "ai_easy.h"

#include <assert.h>
#include <stdlib.h>
#include <time.h>


/*
 * Internal helper: map a winning symbol to the corresponding GameStatus
 * and record it on state.  Checks for draw when there is no winner.
 */
static void update_status(GameState *state) {
    CellValue winner = wcc_check_winner(state->board);

    if (winner == CELL_X) {
        state->status = X_WINS;
        state->last_winner = CELL_X;
    } else if (winner == CELL_O) {
        state->status = O_WINS;
        state->last_winner = CELL_O;
    } else if (wcc_is_draw(state->board, state->move_count)) {
        state->status = DRAW;
    }
}


/* ------------------------------------------------------------------ */
/* Public API                                                           */
/* ------------------------------------------------------------------ */
void game_init_session(GameState *state) {
    if (state == NULL) {
        return;
    }

    board_init(state->board);
    state->status = ONGOING;
    state->move_count = 0;
    state->game_count = 0;
    state->last_winner = CELL_EMPTY;
    state->difficulty = DIFFICULTY_HARD;

    /* Default: human plays X, computer plays O */
    state->players[0].type = PLAYER_HUMAN;
    state->players[0].symbol = CELL_X;
    state->players[1].type = PLAYER_COMPUTER;
    state->players[1].symbol = CELL_O;

    srand((unsigned int)time(NULL));
}

void game_new_game(GameState *state) {
    if (state == NULL) {
        return;
    }

    board_init(state->board);
    state->status = ONGOING;
    state->move_count = 0;
    state->game_count++;
}

int game_make_move(GameState *state, int position) {
    if (state == NULL || state->status != ONGOING) {
        return -1;
    }

    CellValue current = game_get_current_player(state);
    if (board_place(state->board, position, current) != 0) {
        return -1;
    }

    state->move_count++;
    update_status(state);
    return 0;
}

GameStatus game_get_status(GameState *state) {
    if (state == NULL) {
        return ONGOING;
    }
    return state->status;
}

int game_is_valid_move(GameState *state, int position) {
    if (state == NULL || state->status != ONGOING) {
        return 0;
    }
    return board_is_valid_move(state->board, position);
}

/* X moves on even turns (0, 2, 4 …), O moves on odd turns. */
CellValue game_get_current_player(GameState *state) {
    if (state == NULL) {
        return CELL_X;
    }
    return (state->move_count % 2 == 0) ? CELL_X : CELL_O;
}

/*
 * players[0] owns CELL_X (index 1 - 1 = 0)
 * players[1] owns CELL_O (index 2 - 1 = 1)
 */
PlayerType game_get_player_type(GameState *state, CellValue symbol) {
    if (state == NULL || symbol == CELL_EMPTY) {
        return PLAYER_HUMAN;
    }
    return state->players[symbol - 1].type;
}

void game_set_player_types(GameState *state, PlayerType x_player, PlayerType o_player) {
    if (state == NULL) {
        return;
    }
    state->players[0].type = x_player;
    state->players[1].type = o_player;
}

void game_set_human_symbol_choice(GameState *state, CellValue human_symbol) {
    if (state == NULL || state->game_count > 0) {
        return;
    }
    if (human_symbol == CELL_X) {
        state->players[0].type = PLAYER_HUMAN;
        state->players[1].type = PLAYER_COMPUTER;
    } else {
        state->players[0].type = PLAYER_COMPUTER;
        state->players[1].type = PLAYER_HUMAN;
    }
}

void game_set_winner_symbol_choice(GameState *state, CellValue winner_symbol) {
    if (state == NULL) {
        return;
    }
    if (winner_symbol == CELL_X) {
        state->players[0].type = PLAYER_HUMAN;
        state->players[1].type = PLAYER_COMPUTER;
    } else {
        state->players[0].type = PLAYER_COMPUTER;
        state->players[1].type = PLAYER_HUMAN;
    }
}

void game_set_difficulty(GameState *state, Difficulty difficulty) {
    if (state == NULL) {
        return;
    }
    state->difficulty = difficulty;
}

int game_get_computer_move(GameState *state) {
    assert(state != NULL);
    if (state->difficulty == DIFFICULTY_EASY) {
        return ai_easy_get_computer_move(state);
    }
    return ai_get_computer_move(state);
}

int game_is_over(GameState *state) {
    if (state == NULL) {
        return 0;
    }
    return state->status != ONGOING;
}
