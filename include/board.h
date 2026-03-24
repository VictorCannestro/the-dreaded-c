#ifndef BOARD_H
#define BOARD_H

#include "constants.h"
#include "win_condition_calculator.h"


/*
 * Clear all cells to CELL_EMPTY.
 */
void board_init(CellValue board[BOARD_SIZE]);

/*
 * Place symbol at position.
 * Returns  0 on success,
 *         -1 if position is out of range or already occupied.
 */
int board_place(CellValue board[BOARD_SIZE], int position, CellValue symbol);

/*
 * Returns 1 if the cell at position is CELL_EMPTY, 0 otherwise.
 * Does not perform bounds checking — position must be 0-8.
 */
int board_is_empty(const CellValue board[BOARD_SIZE], int position);

/*
 * Returns 1 if the position is in range and the cell is empty,
 *         0 otherwise.
 */
int board_is_valid_move(const CellValue board[BOARD_SIZE], int position);

#endif /* BOARD_H */

