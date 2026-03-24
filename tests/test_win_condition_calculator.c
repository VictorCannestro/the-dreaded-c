#include "unity.h"
#include "win_condition_calculator.h"


void setUp(void) {}
void tearDown(void) {}


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

