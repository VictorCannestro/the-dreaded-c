#include "unity.h"
#include "board.h"


void setUp(void) {}
void tearDown(void) {}


/* ===== board_init ===== */
void test_board_init_clears_all_cells(void) {
    CellValue board[BOARD_SIZE];
    board[0] = CELL_X;
    board[4] = CELL_O;

    board_init(board);

    for (int i = 0; i < BOARD_SIZE; i++) {
        TEST_ASSERT_EQUAL(CELL_EMPTY, board[i]);
    }
}


/* ===== board_is_empty ===== */
void test_board_is_empty_returns_true_for_empty_cell(void) {
    CellValue board[BOARD_SIZE];
    board_init(board);

    TEST_ASSERT_EQUAL(1, board_is_empty(board, 0));
    TEST_ASSERT_EQUAL(1, board_is_empty(board, 4));
    TEST_ASSERT_EQUAL(1, board_is_empty(board, 8));
}

void test_board_is_empty_returns_false_for_occupied_cell(void) {
    CellValue board[BOARD_SIZE];
    board_init(board);
    board[3] = CELL_X;
    board[7] = CELL_O;

    TEST_ASSERT_EQUAL(0, board_is_empty(board, 3));
    TEST_ASSERT_EQUAL(0, board_is_empty(board, 7));
}


/* ===== board_place ===== */
void test_board_place_writes_symbol_to_cell(void) {
    CellValue board[BOARD_SIZE];
    board_init(board);

    int result = board_place(board, 0, CELL_X);

    TEST_ASSERT_EQUAL(0, result);
    TEST_ASSERT_EQUAL(CELL_X, board[0]);
}

void test_board_place_returns_error_on_negative_position(void) {
    CellValue board[BOARD_SIZE];
    board_init(board);

    TEST_ASSERT_EQUAL(-1, board_place(board, -1, CELL_X));
}

void test_board_place_returns_error_on_out_of_range_position(void) {
    CellValue board[BOARD_SIZE];
    board_init(board);

    TEST_ASSERT_EQUAL(-1, board_place(board, BOARD_SIZE, CELL_X));
    TEST_ASSERT_EQUAL(-1, board_place(board, 100, CELL_X));
}

void test_board_place_returns_error_on_occupied_cell(void) {
    CellValue board[BOARD_SIZE];
    board_init(board);
    board_place(board, 4, CELL_X);

    int result = board_place(board, 4, CELL_O);

    TEST_ASSERT_EQUAL(-1, result);
    TEST_ASSERT_EQUAL(CELL_X, board[4]); /* original value preserved */
}

void test_board_place_does_not_modify_board_on_failure(void) {
    CellValue board[BOARD_SIZE];
    board_init(board);

    board_place(board, 9, CELL_X); /* out of range — board must be untouched */

    for (int i = 0; i < BOARD_SIZE; i++) {
        TEST_ASSERT_EQUAL(CELL_EMPTY, board[i]);
    }
}


/* ===== board_is_valid_move ===== */

void test_board_is_valid_move_true_for_empty_cell(void) {
    CellValue board[BOARD_SIZE];
    board_init(board);

    TEST_ASSERT_EQUAL(1, board_is_valid_move(board, 0));
    TEST_ASSERT_EQUAL(1, board_is_valid_move(board, 4));
    TEST_ASSERT_EQUAL(1, board_is_valid_move(board, 8));
}

void test_board_is_valid_move_false_for_occupied_cell(void) {
    CellValue board[BOARD_SIZE];
    board_init(board);
    board[2] = CELL_O;

    TEST_ASSERT_EQUAL(0, board_is_valid_move(board, 2));
}

void test_board_is_valid_move_false_for_negative_position(void) {
    CellValue board[BOARD_SIZE];
    board_init(board);

    TEST_ASSERT_EQUAL(0, board_is_valid_move(board, -1));
}

void test_board_is_valid_move_false_for_out_of_range_position(void) {
    CellValue board[BOARD_SIZE];
    board_init(board);

    TEST_ASSERT_EQUAL(0, board_is_valid_move(board, BOARD_SIZE));
    TEST_ASSERT_EQUAL(0, board_is_valid_move(board, 100));
}

