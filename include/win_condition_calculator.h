#ifndef WIN_CONDITION_CALCULATOR_H
#define WIN_CONDITION_CALCULATOR_H

#include "constants.h"

#define BOARD_SIZE 9
#define NUM_WIN_COMBOS 8

/*
 * Scan the board for a three-in-a-row.
 * Returns the winning CellValue, or CELL_EMPTY if there is none.
 */
CellValue wcc_check_winner(const CellValue board[BOARD_SIZE]);

/*
 * Returns 1 if the board is a draw (no winner and all cells filled),
 * 0 otherwise.
 */
int wcc_is_draw(const CellValue board[BOARD_SIZE], int move_count);

#endif /* WIN_CONDITION_CALCULATOR_H */

