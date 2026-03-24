#include "win_condition_calculator.h"


static const int WINNING_COMBOS[NUM_WIN_COMBOS][3] = {
    {0, 1, 2},  /* top row       */
    {3, 4, 5},  /* middle row    */
    {6, 7, 8},  /* bottom row    */
    {0, 3, 6},  /* left column   */
    {1, 4, 7},  /* middle column */
    {2, 5, 8},  /* right column  */
    {0, 4, 8},  /* main diagonal */
    {2, 4, 6}   /* anti-diagonal */
};

CellValue wcc_check_winner(const CellValue board[BOARD_SIZE]) {
    for (int i = 0; i < NUM_WIN_COMBOS; i++) {
        int a = WINNING_COMBOS[i][0];
        int b = WINNING_COMBOS[i][1];
        int c = WINNING_COMBOS[i][2];

        if (board[a] != CELL_EMPTY && board[a] == board[b] && board[b] == board[c]) {
            return board[a];
        }
    }
    return CELL_EMPTY;
}

int wcc_is_draw(const CellValue board[BOARD_SIZE], int move_count) {
    return (move_count == BOARD_SIZE) && (wcc_check_winner(board) == CELL_EMPTY);
}

