#include "ai.h"
#include "board.h"
#include "win_condition_calculator.h"

#include <assert.h>
#include <stdlib.h>


/*
 * Minimax algorithm for optimal AI play
 * Returns evaluation score for the board position
 */
static int minimax(CellValue board[BOARD_SIZE], int depth, int is_maximizing) {
    CellValue winner = wcc_check_winner(board);
    if (winner != CELL_EMPTY) {
        return winner == CELL_X ? 10 - depth : -10 + depth;
    }

    /* Count empty cells to detect a draw */
    int moves_left = 0;
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (board_is_empty(board, i)) {
            moves_left++;
        }
    }
    if (moves_left == 0) return 0;

    if (is_maximizing) {
        int best_score = -1000;
        for (int i = 0; i < BOARD_SIZE; i++) {
            if (board_is_empty(board, i)) {
                board[i] = CELL_X;
                int score = minimax(board, depth + 1, 0);
                board[i] = CELL_EMPTY;
                if (score > best_score) best_score = score;
            }
        }
        return best_score;
    } else {
        int best_score = 1000;
        for (int i = 0; i < BOARD_SIZE; i++) {
            if (board_is_empty(board, i)) {
                board[i] = CELL_O;
                int score = minimax(board, depth + 1, 1);
                board[i] = CELL_EMPTY;
                if (score < best_score) best_score = score;
            }
        }
        return best_score;
    }
}

int ai_get_computer_move(GameState *state) {
    assert(state != NULL);

    CellValue ai_symbol = (state->players[0].type == PLAYER_COMPUTER) ? CELL_X : CELL_O;
    int is_maximizing = (ai_symbol == CELL_X);

    int best_move  = -1;
    int best_score = is_maximizing ? -1000 : 1000;

    for (int i = 0; i < BOARD_SIZE; i++) {
        if (board_is_empty(state->board, i)) {
            state->board[i] = ai_symbol;
            int score = minimax(state->board, 0, !is_maximizing);
            state->board[i] = CELL_EMPTY;

            if ((is_maximizing  && score > best_score) ||
                (!is_maximizing && score < best_score)) {
                best_score = score;
                best_move  = i;
            }
        }
    }

    assert(best_move != -1); /* should never be called on a full board */
    return best_move;
}
