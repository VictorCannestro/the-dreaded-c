/*
 * Game Logic - Implementation
 *
 * v2 Improvement: Uses GameState struct instead of global variables.
 * All game state is encapsulated in the struct.
 */

#include "game.h"
#include "board.h"

void game_init(GameState *state) {
    board_init(state->board);
    state->current_player = CELL_X;  /* X always goes first */
    state->status = STATUS_ONGOING;
    state->x_player_type = PLAYER_HUMAN;
    state->o_player_type = PLAYER_COMPUTER;
}

int game_make_move(GameState *state, int position) {
    /* Can't make a move if game is over */
    if (state->status != STATUS_ONGOING) {
        return -1;
    }

    /* Try to place the piece */
    if (board_place(state->board, position, state->current_player) != 0) {
        return -1;
    }

    /* Check for winner */
    CellValue winner = board_check_winner(state->board);
    if (winner == CELL_X) {
        state->status = STATUS_X_WINS;
    } else if (winner == CELL_O) {
        state->status = STATUS_O_WINS;
    } else if (board_is_full(state->board)) {
        state->status = STATUS_DRAW;
    } else {
        /* Game continues - switch player */
        game_switch_player(state);
    }

    return 0;
}

CellValue game_get_current_player(const GameState *state) {
    return state->current_player;
}

void game_switch_player(GameState *state) {
    state->current_player = (state->current_player == CELL_X) ? CELL_O : CELL_X;
}

int game_is_over(const GameState *state) {
    return state->status != STATUS_ONGOING;
}

PlayerType game_get_player_type(const GameState *state, CellValue symbol) {
    if (symbol == CELL_X) {
        return state->x_player_type;
    }
    return state->o_player_type;
}

void game_set_players(GameState *state, PlayerType x_type, PlayerType o_type) {
    state->x_player_type = x_type;
    state->o_player_type = o_type;
}

