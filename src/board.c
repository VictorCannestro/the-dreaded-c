#include "board.h"

#include <string.h>


void board_init(CellValue board[BOARD_SIZE]) {
    memset(board, CELL_EMPTY, BOARD_SIZE * sizeof(CellValue));
}

int board_is_empty(const CellValue board[BOARD_SIZE], int position) {
    return board[position] == CELL_EMPTY;
}

int board_place(CellValue board[BOARD_SIZE], int position, CellValue symbol) {
    if (position < 0 || position >= BOARD_SIZE) {
        return -1;
    }
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

