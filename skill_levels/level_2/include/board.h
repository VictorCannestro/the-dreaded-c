/*
 * Board Management - Header
 *
 * v2 Improvement: Separate module for board operations.
 * The board is now part of GameState, not a global variable.
 */

#ifndef BOARD_H
#define BOARD_H

#include "types.h"

/* Initialize all cells to CELL_EMPTY */
void board_init(CellValue board[BOARD_SIZE]);

/* Check if a cell is empty */
int board_is_empty(const CellValue board[BOARD_SIZE], int position);

/* Place a symbol at a position (returns 0 on success, -1 on failure) */
int board_place(CellValue board[BOARD_SIZE], int position, CellValue symbol);

/* Check if a move is valid (in range and empty) */
int board_is_valid_move(const CellValue board[BOARD_SIZE], int position);

/* Check for winner (returns CELL_EMPTY if no winner) */
CellValue board_check_winner(const CellValue board[BOARD_SIZE]);

/* Check if board is full */
int board_is_full(const CellValue board[BOARD_SIZE]);

#endif /* BOARD_H */

