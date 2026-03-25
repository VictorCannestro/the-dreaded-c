#include "win_condition_calculator.h"


/*
 * Check if there's a winner along a line defined by starting position
 * and step (delta). Returns the winning symbol or CELL_EMPTY.
 */
static CellValue check_line(const CellValue board[BOARD_SIZE], int start, int step) {
    CellValue first = board[start];
    if (first == CELL_EMPTY) {
        return CELL_EMPTY;
    }

    for (int i = 1; i < WIN_LENGTH; i++) {
        if (board[start + i * step] != first) {
            return CELL_EMPTY;
        }
    }
    return first;
}


CellValue wcc_check_winner(const CellValue board[BOARD_SIZE]) {
    CellValue winner;
    winner = wcc_check_rows(board);
    if (winner != CELL_EMPTY) {
        return winner;
    }
    winner = wcc_check_columns(board);
    if (winner != CELL_EMPTY) {
        return winner;
    }
    winner = wcc_check_main_diagonals(board);
    if (winner != CELL_EMPTY) {
        return winner;
    }
    winner = wcc_check_anti_diagonals(board);
    if (winner != CELL_EMPTY) {
        return winner;
    }
    return CELL_EMPTY;
}

CellValue wcc_check_rows(const CellValue board[BOARD_SIZE]) {
    for (int row = 0; row < BOARD_DIM; row++) {
        for (int col = 0; col <= BOARD_DIM - WIN_LENGTH; col++) {
            int start = row * BOARD_DIM + col;
            CellValue winner = check_line(board, start, 1);
            if (winner != CELL_EMPTY) {
                return winner;
            }
        }
    }
    return CELL_EMPTY;
}

CellValue wcc_check_columns(const CellValue board[BOARD_SIZE]) {
    for (int col = 0; col < BOARD_DIM; col++) {
        for (int row = 0; row <= BOARD_DIM - WIN_LENGTH; row++) {
            int start = row * BOARD_DIM + col;
            CellValue winner = check_line(board, start, BOARD_DIM);
            if (winner != CELL_EMPTY) {
                return winner;
            }
        }
    }
    return CELL_EMPTY;
}

CellValue wcc_check_main_diagonals(const CellValue board[BOARD_SIZE]) {
    for (int row = 0; row <= BOARD_DIM - WIN_LENGTH; row++) {
        for (int col = 0; col <= BOARD_DIM - WIN_LENGTH; col++) {
            int start = row * BOARD_DIM + col;
            CellValue winner = check_line(board, start, BOARD_DIM + 1);
            if (winner != CELL_EMPTY) {
                return winner;
            }
        }
    }
    return CELL_EMPTY;
}

CellValue wcc_check_anti_diagonals(const CellValue board[BOARD_SIZE]) {
    for (int row = 0; row <= BOARD_DIM - WIN_LENGTH; row++) {
        for (int col = WIN_LENGTH - 1; col < BOARD_DIM; col++) {
            int start = row * BOARD_DIM + col;
            CellValue winner = check_line(board, start, BOARD_DIM - 1);
            if (winner != CELL_EMPTY) {
                return winner;
            }
        }
    }
    return CELL_EMPTY;
}

int wcc_is_draw(const CellValue board[BOARD_SIZE], int move_count) {
    return (move_count == BOARD_SIZE) && (wcc_check_winner(board) == CELL_EMPTY);
}
