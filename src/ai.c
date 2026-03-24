#include "ai.h"


/*
 * Check for a winner on the board
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

/*
 * Minimax algorithm for optimal AI play
 * Returns evaluation score for the board position
 */
static int minimax(CellValue board[9], int depth, int is_maximizing) {
    CellValue winner = check_winner(board);
    if (winner != CELL_EMPTY) {
        return winner == CELL_X ? 10 - depth : -10 + depth;
    }

    // Check for draw
    int moves_left = 0;
    for (int i = 0; i < 9; i++) {
        if (board[i] == CELL_EMPTY) moves_left++;
    }
    if (moves_left == 0) return 0;

    if (is_maximizing) {
        int best_score = -1000;
        for (int i = 0; i < 9; i++) {
            if (board[i] == CELL_EMPTY) {
                board[i] = CELL_X;
                int score = minimax(board, depth + 1, 0);
                board[i] = CELL_EMPTY;
                if (score > best_score) best_score = score;
            }
        }
        return best_score;
    } else {
        int best_score = 1000;
        for (int i = 0; i < 9; i++) {
            if (board[i] == CELL_EMPTY) {
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
    if (state == NULL) {
        return 4; // Default to center
    }

    CellValue ai_symbol = (state->players[0].type == PLAYER_COMPUTER) ? CELL_X : CELL_O;
    int is_maximizing = (ai_symbol == CELL_X);

    // Find the best move using minimax
    int best_move = -1;
    int best_score = is_maximizing ? -1000 : 1000;

    for (int i = 0; i < 9; i++) {
        if (state->board[i] == CELL_EMPTY) {
            state->board[i] = ai_symbol;
            int score = minimax(state->board, 0, !is_maximizing);
            state->board[i] = CELL_EMPTY;

            if ((is_maximizing && score > best_score) ||
                (!is_maximizing && score < best_score)) {
                best_score = score;
                best_move = i;
            }
        }
    }

    return best_move != -1 ? best_move : 4; // Default to center if no move found
}