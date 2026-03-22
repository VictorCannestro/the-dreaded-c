#include "tictactoe.h"
#include <string.h>

/*
 * Internal helper: Check for a winner
 * Returns CELL_X if X wins, CELL_O if O wins, CELL_EMPTY if no winner
 */
static CellValue check_winner(const CellValue board[9]) {
    // Winning combinations (row, col, diagonal)
    const int winning_combos[8][3] = {
        {0, 1, 2},  // Top row
        {3, 4, 5},  // Middle row
        {6, 7, 8},  // Bottom row
        {0, 3, 6},  // Left column
        {1, 4, 7},  // Middle column
        {2, 5, 8},  // Right column
        {0, 4, 8},  // Diagonal (top-left to bottom-right)
        {2, 4, 6}   // Diagonal (top-right to bottom-left)
    };

    for (int i = 0; i < 8; i++) {
        int a = winning_combos[i][0];
        int b = winning_combos[i][1];
        int c = winning_combos[i][2];

        if (board[a] != CELL_EMPTY &&
            board[a] == board[b] &&
            board[b] == board[c]) {
            return board[a];  // Return the winner
        }
    }

    return CELL_EMPTY;  // No winner
}

void game_init(GameState *state) {
    if (state == NULL) return;

    memset(state->board, CELL_EMPTY, 9 * sizeof(CellValue));
    state->status = GAME_ONGOING;
    state->move_count = 0;
}

int game_make_move(GameState *state, int position) {
    if (state == NULL || position < 0 || position > 8) {
        return -1;  // Invalid input
    }

    if (state->board[position] != CELL_EMPTY) {
        return -1;  // Position already occupied
    }

    if (state->status != GAME_ONGOING) {
        return -1;  // Game already finished
    }

    // Determine current player and make move
    CellValue player = game_get_current_player(state);
    state->board[position] = player;
    state->move_count++;

    // Check for winner
    CellValue winner = check_winner(state->board);
    if (winner == CELL_X) {
        state->status = GAME_X_WINS;
        return 0;
    } else if (winner == CELL_O) {
        state->status = GAME_O_WINS;
        return 0;
    }

    // Check for draw (board full)
    if (state->move_count == 9) {
        state->status = GAME_DRAW;
        return 0;
    }

    return 0;  // Move successful
}

GameStatus game_get_status(GameState *state) {
    if (state == NULL) return GAME_ONGOING;
    return state->status;
}

int game_is_valid_move(GameState *state, int position) {
    if (state == NULL || position < 0 || position > 8) {
        return 0;
    }

    if (state->board[position] != CELL_EMPTY) {
        return 0;
    }

    if (state->status != GAME_ONGOING) {
        return 0;
    }

    return 1;  // Valid move
}

CellValue game_get_current_player(GameState *state) {
    if (state == NULL) return CELL_X;

    // X goes first (even move count), O goes second (odd move count)
    if (state->move_count % 2 == 0) {
        return CELL_X;
    } else {
        return CELL_O;
    }
}

void game_reset(GameState *state) {
    if (state == NULL) return;
    game_init(state);
}
