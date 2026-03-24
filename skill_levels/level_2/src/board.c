/*
 * Board Management - Implementation
 *
 * v2 Improvement: Win checking uses a loop over winning combinations
 * instead of repeating the same code 8 times.
 */

#include "board.h"

/* All winning combinations */
static const int WIN_COMBOS[8][3] = {
    {0, 1, 2},  /* Top row */
    {3, 4, 5},  /* Middle row */
    {6, 7, 8},  /* Bottom row */
    {0, 3, 6},  /* Left column */
    {1, 4, 7},  /* Middle column */
    {2, 5, 8},  /* Right column */
    {0, 4, 8},  /* Main diagonal */
    {2, 4, 6}   /* Anti-diagonal */
};

void board_init(CellValue board[BOARD_SIZE]) {
    int i;
    for (i = 0; i < BOARD_SIZE; i++) {
        board[i] = CELL_EMPTY;
    }
}

int board_is_empty(const CellValue board[BOARD_SIZE], int position) {
    return board[position] == CELL_EMPTY;
}

int board_place(CellValue board[BOARD_SIZE], int position, CellValue symbol) {
    /* Validate position */
    if (position < 0 || position >= BOARD_SIZE) {
        return -1;
    }

    /* Check if already occupied */
    if (!board_is_empty(board, position)) {
        return -1;
    }

    board[position] = symbol;
    return 0;
}

int board_is_valid_move(const CellValue board[BOARD_SIZE], int position) {
    if (position < 0 || position >= BOARD_SIZE) {
        return 0;
    }
    return board_is_empty(board, position);
}

/*
 * v2 Improvement: Loop over winning combinations instead of
 * copy-pasting the same check 8 times.
 *
 * Before (v1): 24 lines of repeated if statements
 * After (v2):  10 lines with a loop
 */
CellValue board_check_winner(const CellValue board[BOARD_SIZE]) {
    int i;
    for (i = 0; i < 8; i++) {
        int a = WIN_COMBOS[i][0];
        int b = WIN_COMBOS[i][1];
        int c = WIN_COMBOS[i][2];

        if (board[a] != CELL_EMPTY &&
            board[a] == board[b] &&
            board[b] == board[c]) {
            return board[a];  /* Return the winner (CELL_X or CELL_O) */
        }
    }
    return CELL_EMPTY;  /* No winner */
}

int board_is_full(const CellValue board[BOARD_SIZE]) {
    int i;
    for (i = 0; i < BOARD_SIZE; i++) {
        if (board[i] == CELL_EMPTY) {
            return 0;
        }
    }
    return 1;
}

