#ifndef WIN_CONDITION_CALCULATOR_H
#define WIN_CONDITION_CALCULATOR_H

#include "constants.h"

/*
 * Check all rows for WIN_LENGTH symbols in a row.
 * Returns the winning CellValue, or CELL_EMPTY if none.
 */
CellValue wcc_check_rows(const CellValue board[BOARD_SIZE]);

/*
 * Check all columns for WIN_LENGTH symbols in a row.
 * Returns the winning CellValue, or CELL_EMPTY if none.
 */
CellValue wcc_check_columns(const CellValue board[BOARD_SIZE]);

/*
 * Check main diagonals (top-left to bottom-right) for WIN_LENGTH symbols.
 * Returns the winning CellValue, or CELL_EMPTY if none.
 */
CellValue wcc_check_main_diagonals(const CellValue board[BOARD_SIZE]);

/*
 * Check anti-diagonals (top-right to bottom-left) for WIN_LENGTH symbols.
 * Returns the winning CellValue, or CELL_EMPTY if none.
 */
CellValue wcc_check_anti_diagonals(const CellValue board[BOARD_SIZE]);

/*
 * Scan the board for WIN_LENGTH symbols in a row.
 * Supports any board dimension defined by BOARD_DIM.
 * Returns the winning CellValue, or CELL_EMPTY if there is none.
 */
CellValue wcc_check_winner(const CellValue board[BOARD_SIZE]);

/*
 * Returns 1 if the board is a draw (no winner and all cells filled),
 * 0 otherwise.
 */
int wcc_is_draw(const CellValue board[BOARD_SIZE], int move_count);

#endif /* WIN_CONDITION_CALCULATOR_H */

