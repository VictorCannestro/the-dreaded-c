#include "unity.h"
#include "win_condition_calculator.h"


void setUp(void) {}
void tearDown(void) {}


/* ===== wcc_check_rows ===== */
void test_wcc_check_rows_empty_board_returns_no_winner(void) {
    CellValue board[BOARD_SIZE] = {
        CELL_EMPTY, CELL_EMPTY, CELL_EMPTY,
        CELL_EMPTY, CELL_EMPTY, CELL_EMPTY,
        CELL_EMPTY, CELL_EMPTY, CELL_EMPTY
    };
    TEST_ASSERT_EQUAL(CELL_EMPTY, wcc_check_rows(board));
}

void test_wcc_check_rows_x_wins_top_row(void) {
    CellValue board[BOARD_SIZE] = {
        CELL_X, CELL_X, CELL_X,
        CELL_O, CELL_O, CELL_EMPTY,
        CELL_EMPTY, CELL_EMPTY, CELL_EMPTY
    };
    TEST_ASSERT_EQUAL(CELL_X, wcc_check_rows(board));
}

void test_wcc_check_rows_x_wins_middle_row(void) {
    CellValue board[BOARD_SIZE] = {
        CELL_O, CELL_EMPTY, CELL_O,
        CELL_X, CELL_X, CELL_X,
        CELL_EMPTY, CELL_EMPTY, CELL_EMPTY
    };
    TEST_ASSERT_EQUAL(CELL_X, wcc_check_rows(board));
}

void test_wcc_check_rows_o_wins_bottom_row(void) {
    CellValue board[BOARD_SIZE] = {
        CELL_X, CELL_X, CELL_EMPTY,
        CELL_EMPTY, CELL_EMPTY, CELL_X,
        CELL_O, CELL_O, CELL_O
    };
    TEST_ASSERT_EQUAL(CELL_O, wcc_check_rows(board));
}

void test_wcc_check_rows_no_winner_partial_row(void) {
    CellValue board[BOARD_SIZE] = {
        CELL_X, CELL_X, CELL_EMPTY,
        CELL_EMPTY, CELL_EMPTY, CELL_EMPTY,
        CELL_EMPTY, CELL_EMPTY, CELL_EMPTY
    };
    TEST_ASSERT_EQUAL(CELL_EMPTY, wcc_check_rows(board));
}

void test_wcc_check_rows_no_winner_column_win(void) {
    /* Column win should not be detected by wcc_check_rows */
    CellValue board[BOARD_SIZE] = {
        CELL_X, CELL_O, CELL_EMPTY,
        CELL_X, CELL_O, CELL_EMPTY,
        CELL_X, CELL_EMPTY, CELL_EMPTY
    };
    TEST_ASSERT_EQUAL(CELL_EMPTY, wcc_check_rows(board));
}


/* ===== wcc_check_columns ===== */
void test_wcc_check_columns_empty_board_returns_no_winner(void) {
    CellValue board[BOARD_SIZE] = {
        CELL_EMPTY, CELL_EMPTY, CELL_EMPTY,
        CELL_EMPTY, CELL_EMPTY, CELL_EMPTY,
        CELL_EMPTY, CELL_EMPTY, CELL_EMPTY
    };
    TEST_ASSERT_EQUAL(CELL_EMPTY, wcc_check_columns(board));
}

void test_wcc_check_columns_o_wins_left_column(void) {
    CellValue board[BOARD_SIZE] = {
        CELL_O, CELL_X, CELL_X,
        CELL_O, CELL_X, CELL_EMPTY,
        CELL_O, CELL_EMPTY, CELL_EMPTY
    };
    TEST_ASSERT_EQUAL(CELL_O, wcc_check_columns(board));
}

void test_wcc_check_columns_x_wins_middle_column(void) {
    CellValue board[BOARD_SIZE] = {
        CELL_O, CELL_X, CELL_EMPTY,
        CELL_O, CELL_X, CELL_EMPTY,
        CELL_EMPTY, CELL_X, CELL_O
    };
    TEST_ASSERT_EQUAL(CELL_X, wcc_check_columns(board));
}

void test_wcc_check_columns_o_wins_right_column(void) {
    CellValue board[BOARD_SIZE] = {
        CELL_X, CELL_X, CELL_O,
        CELL_EMPTY, CELL_X, CELL_O,
        CELL_EMPTY, CELL_EMPTY, CELL_O
    };
    TEST_ASSERT_EQUAL(CELL_O, wcc_check_columns(board));
}

void test_wcc_check_columns_no_winner_partial_column(void) {
    CellValue board[BOARD_SIZE] = {
        CELL_X, CELL_EMPTY, CELL_EMPTY,
        CELL_X, CELL_EMPTY, CELL_EMPTY,
        CELL_EMPTY, CELL_EMPTY, CELL_EMPTY
    };
    TEST_ASSERT_EQUAL(CELL_EMPTY, wcc_check_columns(board));
}

void test_wcc_check_columns_no_winner_row_win(void) {
    /* Row win should not be detected by wcc_check_columns */
    CellValue board[BOARD_SIZE] = {
        CELL_X, CELL_X, CELL_X,
        CELL_O, CELL_O, CELL_EMPTY,
        CELL_EMPTY, CELL_EMPTY, CELL_EMPTY
    };
    TEST_ASSERT_EQUAL(CELL_EMPTY, wcc_check_columns(board));
}


/* ===== wcc_check_main_diagonals ===== */
void test_wcc_check_main_diagonals_empty_board_returns_no_winner(void) {
    CellValue board[BOARD_SIZE] = {
        CELL_EMPTY, CELL_EMPTY, CELL_EMPTY,
        CELL_EMPTY, CELL_EMPTY, CELL_EMPTY,
        CELL_EMPTY, CELL_EMPTY, CELL_EMPTY
    };
    TEST_ASSERT_EQUAL(CELL_EMPTY, wcc_check_main_diagonals(board));
}

void test_wcc_check_main_diagonals_x_wins(void) {
    CellValue board[BOARD_SIZE] = {
        CELL_X, CELL_O, CELL_EMPTY,
        CELL_O, CELL_X, CELL_EMPTY,
        CELL_EMPTY, CELL_EMPTY, CELL_X
    };
    TEST_ASSERT_EQUAL(CELL_X, wcc_check_main_diagonals(board));
}

void test_wcc_check_main_diagonals_o_wins(void) {
    CellValue board[BOARD_SIZE] = {
        CELL_O, CELL_X, CELL_X,
        CELL_X, CELL_O, CELL_EMPTY,
        CELL_EMPTY, CELL_X, CELL_O
    };
    TEST_ASSERT_EQUAL(CELL_O, wcc_check_main_diagonals(board));
}

void test_wcc_check_main_diagonals_no_winner_partial(void) {
    CellValue board[BOARD_SIZE] = {
        CELL_X, CELL_EMPTY, CELL_EMPTY,
        CELL_EMPTY, CELL_X, CELL_EMPTY,
        CELL_EMPTY, CELL_EMPTY, CELL_EMPTY
    };
    TEST_ASSERT_EQUAL(CELL_EMPTY, wcc_check_main_diagonals(board));
}

void test_wcc_check_main_diagonals_no_winner_anti_diagonal_win(void) {
    /* Anti-diagonal win should not be detected by wcc_check_main_diagonals */
    CellValue board[BOARD_SIZE] = {
        CELL_EMPTY, CELL_EMPTY, CELL_X,
        CELL_EMPTY, CELL_X, CELL_EMPTY,
        CELL_X, CELL_EMPTY, CELL_EMPTY
    };
    TEST_ASSERT_EQUAL(CELL_EMPTY, wcc_check_main_diagonals(board));
}


/* ===== wcc_check_anti_diagonals ===== */
void test_wcc_check_anti_diagonals_empty_board_returns_no_winner(void) {
    CellValue board[BOARD_SIZE] = {
        CELL_EMPTY, CELL_EMPTY, CELL_EMPTY,
        CELL_EMPTY, CELL_EMPTY, CELL_EMPTY,
        CELL_EMPTY, CELL_EMPTY, CELL_EMPTY
    };
    TEST_ASSERT_EQUAL(CELL_EMPTY, wcc_check_anti_diagonals(board));
}

void test_wcc_check_anti_diagonals_o_wins(void) {
    CellValue board[BOARD_SIZE] = {
        CELL_X, CELL_X, CELL_O,
        CELL_EMPTY, CELL_O, CELL_EMPTY,
        CELL_O, CELL_EMPTY, CELL_X
    };
    TEST_ASSERT_EQUAL(CELL_O, wcc_check_anti_diagonals(board));
}

void test_wcc_check_anti_diagonals_x_wins(void) {
    CellValue board[BOARD_SIZE] = {
        CELL_O, CELL_O, CELL_X,
        CELL_EMPTY, CELL_X, CELL_EMPTY,
        CELL_X, CELL_EMPTY, CELL_O
    };
    TEST_ASSERT_EQUAL(CELL_X, wcc_check_anti_diagonals(board));
}

void test_wcc_check_anti_diagonals_no_winner_partial(void) {
    CellValue board[BOARD_SIZE] = {
        CELL_EMPTY, CELL_EMPTY, CELL_X,
        CELL_EMPTY, CELL_X, CELL_EMPTY,
        CELL_EMPTY, CELL_EMPTY, CELL_EMPTY
    };
    TEST_ASSERT_EQUAL(CELL_EMPTY, wcc_check_anti_diagonals(board));
}

void test_wcc_check_anti_diagonals_no_winner_main_diagonal_win(void) {
    /* Main diagonal win should not be detected by wcc_check_anti_diagonals */
    CellValue board[BOARD_SIZE] = {
        CELL_X, CELL_EMPTY, CELL_EMPTY,
        CELL_EMPTY, CELL_X, CELL_EMPTY,
        CELL_EMPTY, CELL_EMPTY, CELL_X
    };
    TEST_ASSERT_EQUAL(CELL_EMPTY, wcc_check_anti_diagonals(board));
}


/* ===== wcc_check_winner ===== */
void test_wcc_check_winner_empty_board_returns_no_winner(void) {
    CellValue board[BOARD_SIZE] = {
        CELL_EMPTY, CELL_EMPTY, CELL_EMPTY,
        CELL_EMPTY, CELL_EMPTY, CELL_EMPTY,
        CELL_EMPTY, CELL_EMPTY, CELL_EMPTY
    };
    TEST_ASSERT_EQUAL(CELL_EMPTY, wcc_check_winner(board));
}

void test_wcc_check_winner_x_wins_top_row(void) {
    CellValue board[BOARD_SIZE] = {
        CELL_X, CELL_X, CELL_X,
        CELL_O, CELL_O, CELL_EMPTY,
        CELL_EMPTY, CELL_EMPTY, CELL_EMPTY
    };
    TEST_ASSERT_EQUAL(CELL_X, wcc_check_winner(board));
}

void test_wcc_check_winner_x_wins_middle_row(void) {
    CellValue board[BOARD_SIZE] = {
        CELL_O, CELL_EMPTY, CELL_O,
        CELL_X, CELL_X, CELL_X,
        CELL_EMPTY, CELL_EMPTY, CELL_EMPTY
    };
    TEST_ASSERT_EQUAL(CELL_X, wcc_check_winner(board));
}

void test_wcc_check_winner_x_wins_bottom_row(void) {
    CellValue board[BOARD_SIZE] = {
        CELL_O, CELL_O, CELL_EMPTY,
        CELL_EMPTY, CELL_EMPTY, CELL_O,
        CELL_X, CELL_X, CELL_X
    };
    TEST_ASSERT_EQUAL(CELL_X, wcc_check_winner(board));
}

void test_wcc_check_winner_o_wins_left_column(void) {
    CellValue board[BOARD_SIZE] = {
        CELL_O, CELL_X, CELL_X,
        CELL_O, CELL_X, CELL_EMPTY,
        CELL_O, CELL_EMPTY, CELL_EMPTY
    };
    TEST_ASSERT_EQUAL(CELL_O, wcc_check_winner(board));
}

void test_wcc_check_winner_o_wins_middle_column(void) {
    CellValue board[BOARD_SIZE] = {
        CELL_X, CELL_O, CELL_EMPTY,
        CELL_X, CELL_O, CELL_EMPTY,
        CELL_EMPTY, CELL_O, CELL_X
    };
    TEST_ASSERT_EQUAL(CELL_O, wcc_check_winner(board));
}

void test_wcc_check_winner_o_wins_right_column(void) {
    CellValue board[BOARD_SIZE] = {
        CELL_X, CELL_X, CELL_O,
        CELL_EMPTY, CELL_X, CELL_O,
        CELL_EMPTY, CELL_EMPTY, CELL_O
    };
    TEST_ASSERT_EQUAL(CELL_O, wcc_check_winner(board));
}

void test_wcc_check_winner_x_wins_main_diagonal(void) {
    CellValue board[BOARD_SIZE] = {
        CELL_X, CELL_O, CELL_EMPTY,
        CELL_O, CELL_X, CELL_EMPTY,
        CELL_EMPTY, CELL_EMPTY, CELL_X
    };
    TEST_ASSERT_EQUAL(CELL_X, wcc_check_winner(board));
}

void test_wcc_check_winner_o_wins_anti_diagonal(void) {
    CellValue board[BOARD_SIZE] = {
        CELL_X, CELL_X, CELL_O,
        CELL_EMPTY, CELL_O, CELL_EMPTY,
        CELL_O, CELL_EMPTY, CELL_X
    };
    TEST_ASSERT_EQUAL(CELL_O, wcc_check_winner(board));
}

void test_wcc_check_winner_partial_board_no_winner(void) {
    /* Three moves in, no winner yet */
    CellValue board[BOARD_SIZE] = {
        CELL_X, CELL_O, CELL_EMPTY,
        CELL_EMPTY, CELL_X, CELL_EMPTY,
        CELL_EMPTY, CELL_EMPTY, CELL_EMPTY
    };
    TEST_ASSERT_EQUAL(CELL_EMPTY, wcc_check_winner(board));
}


/* ===== wcc_is_draw ===== */
void test_wcc_is_draw_full_board_no_winner(void) {
    /* X O X / O X O / O X O — no three in a row, all cells filled */
    CellValue board[BOARD_SIZE] = {
        CELL_X, CELL_O, CELL_X,
        CELL_O, CELL_X, CELL_O,
        CELL_O, CELL_X, CELL_O
    };
    TEST_ASSERT_EQUAL(1, wcc_is_draw(board, BOARD_SIZE));
}

void test_wcc_is_draw_full_board_with_winner_is_not_draw(void) {
    CellValue board[BOARD_SIZE] = {
        CELL_X, CELL_X, CELL_X,
        CELL_O, CELL_O, CELL_EMPTY,
        CELL_EMPTY, CELL_EMPTY, CELL_EMPTY
    };
    TEST_ASSERT_EQUAL(0, wcc_is_draw(board, BOARD_SIZE));
}

void test_wcc_is_draw_partial_board_is_not_draw(void) {
    CellValue board[BOARD_SIZE] = {
        CELL_X, CELL_O, CELL_EMPTY,
        CELL_EMPTY, CELL_EMPTY, CELL_EMPTY,
        CELL_EMPTY, CELL_EMPTY, CELL_EMPTY
    };
    TEST_ASSERT_EQUAL(0, wcc_is_draw(board, 2));
}

void test_wcc_is_draw_empty_board_is_not_draw(void) {
    CellValue board[BOARD_SIZE] = {
        CELL_EMPTY, CELL_EMPTY, CELL_EMPTY,
        CELL_EMPTY, CELL_EMPTY, CELL_EMPTY,
        CELL_EMPTY, CELL_EMPTY, CELL_EMPTY
    };
    TEST_ASSERT_EQUAL(0, wcc_is_draw(board, 0));
}

