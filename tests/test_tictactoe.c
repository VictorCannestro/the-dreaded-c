#include "unity.h"
#include "tictactoe.h"

void setUp(void) {
    // This function is called before each test
}

void tearDown(void) {
    // This function is called after each test
}

// ===== Initialization Tests =====

void test_game_init_creates_empty_board(void) {
    GameState game;
    game_init(&game);

    for (int i = 0; i < 9; i++) {
        TEST_ASSERT_EQUAL(CELL_EMPTY, game.board[i]);
    }
    TEST_ASSERT_EQUAL(GAME_ONGOING, game.status);
    TEST_ASSERT_EQUAL(0, game.move_count);
}

void test_game_init_with_null_pointer(void) {
    // Should not crash
    game_init(NULL);
    TEST_PASS();
}

// ===== Move Validation Tests =====

void test_valid_move_on_empty_cell(void) {
    GameState game;
    game_init(&game);

    TEST_ASSERT_EQUAL(1, game_is_valid_move(&game, 0));
    TEST_ASSERT_EQUAL(1, game_is_valid_move(&game, 4));
    TEST_ASSERT_EQUAL(1, game_is_valid_move(&game, 8));
}

void test_invalid_move_out_of_range(void) {
    GameState game;
    game_init(&game);

    TEST_ASSERT_EQUAL(0, game_is_valid_move(&game, -1));
    TEST_ASSERT_EQUAL(0, game_is_valid_move(&game, 9));
    TEST_ASSERT_EQUAL(0, game_is_valid_move(&game, 100));
}

void test_invalid_move_on_occupied_cell(void) {
    GameState game;
    game_init(&game);
    
    game_make_move(&game, 0);  // X plays at 0
    TEST_ASSERT_EQUAL(0, game_is_valid_move(&game, 0));
}

// ===== Player Turn Tests =====

void test_x_plays_first(void) {
    GameState game;
    game_init(&game);

    CellValue player = game_get_current_player(&game);
    TEST_ASSERT_EQUAL(CELL_X, player);
}

void test_players_alternate(void) {
    GameState game;
    game_init(&game);

    TEST_ASSERT_EQUAL(CELL_X, game_get_current_player(&game));
    game_make_move(&game, 0);
    TEST_ASSERT_EQUAL(CELL_O, game_get_current_player(&game));
    game_make_move(&game, 1);
    TEST_ASSERT_EQUAL(CELL_X, game_get_current_player(&game));
}

// ===== Basic Move Tests =====

void test_make_move_success(void) {
    GameState game;
    game_init(&game);

    int result = game_make_move(&game, 0);
    TEST_ASSERT_EQUAL(0, result);
    TEST_ASSERT_EQUAL(CELL_X, game.board[0]);
    TEST_ASSERT_EQUAL(1, game.move_count);
}

void test_make_move_invalid_position(void) {
    GameState game;
    game_init(&game);

    int result = game_make_move(&game, 10);
    TEST_ASSERT_EQUAL(-1, result);
    TEST_ASSERT_EQUAL(0, game.move_count);
}

void test_make_move_occupied_cell(void) {
    GameState game;
    game_init(&game);

    game_make_move(&game, 0);  // X plays at 0
    int result = game_make_move(&game, 0);  // O tries to play at 0
    TEST_ASSERT_EQUAL(-1, result);
    TEST_ASSERT_EQUAL(1, game.move_count);
}

// ===== Winning Tests =====

void test_x_wins_top_row(void) {
    GameState game;
    game_init(&game);

    // X: 0, O: 3, X: 1, O: 4, X: 2
    game_make_move(&game, 0);  // X
    game_make_move(&game, 3);  // O
    game_make_move(&game, 1);  // X
    game_make_move(&game, 4);  // O
    game_make_move(&game, 2);  // X wins

    TEST_ASSERT_EQUAL(GAME_X_WINS, game.status);
}

void test_o_wins_middle_column(void) {
    GameState game;
    game_init(&game);

    // X: 0, O: 1, X: 2, O: 4, X: 5, O: 7
    game_make_move(&game, 0);  // X
    game_make_move(&game, 1);  // O
    game_make_move(&game, 2);  // X
    game_make_move(&game, 4);  // O
    game_make_move(&game, 5);  // X
    game_make_move(&game, 7);  // O wins

    TEST_ASSERT_EQUAL(GAME_O_WINS, game.status);
}

void test_x_wins_diagonal(void) {
    GameState game;
    game_init(&game);

    // X: 0, O: 1, X: 4, O: 2, X: 8
    game_make_move(&game, 0);  // X
    game_make_move(&game, 1);  // O
    game_make_move(&game, 4);  // X
    game_make_move(&game, 2);  // O
    game_make_move(&game, 8);  // X wins diagonal

    TEST_ASSERT_EQUAL(GAME_X_WINS, game.status);
}

void test_o_wins_anti_diagonal(void) {
    GameState game;
    game_init(&game);

    // X: 0, O: 2, X: 1, O: 4, X: 3, O: 6
    game_make_move(&game, 0);  // X
    game_make_move(&game, 2);  // O
    game_make_move(&game, 1);  // X
    game_make_move(&game, 4);  // O
    game_make_move(&game, 3);  // X
    game_make_move(&game, 6);  // O wins anti-diagonal

    TEST_ASSERT_EQUAL(GAME_O_WINS, game.status);
}

// ===== Draw Tests =====

void test_game_draw(void) {
    GameState game;
    game_init(&game);

    // Fill board without winner: X-O-X / X-O-O / O-X-X
    game_make_move(&game, 0);  // X
    game_make_move(&game, 1);  // O
    game_make_move(&game, 2);  // X
    game_make_move(&game, 3);  // O
    game_make_move(&game, 4);  // X
    game_make_move(&game, 5);  // O
    game_make_move(&game, 6);  // X
    game_make_move(&game, 7);  // O
    game_make_move(&game, 8);  // X

    TEST_ASSERT_EQUAL(GAME_DRAW, game.status);
    TEST_ASSERT_EQUAL(9, game.move_count);
}

// ===== Reset Tests =====

void test_game_reset(void) {
    GameState game;
    game_init(&game);

    game_make_move(&game, 0);
    game_make_move(&game, 1);

    game_reset(&game);

    TEST_ASSERT_EQUAL(GAME_ONGOING, game.status);
    TEST_ASSERT_EQUAL(0, game.move_count);
    TEST_ASSERT_EQUAL(CELL_EMPTY, game.board[0]);
    TEST_ASSERT_EQUAL(CELL_EMPTY, game.board[1]);
}

int main(void) {
    UNITY_BEGIN();

    // Initialization tests
    RUN_TEST(test_game_init_creates_empty_board);
    RUN_TEST(test_game_init_with_null_pointer);

    // Move validation tests
    RUN_TEST(test_valid_move_on_empty_cell);
    RUN_TEST(test_invalid_move_out_of_range);
    RUN_TEST(test_invalid_move_on_occupied_cell);

    // Player turn tests
    RUN_TEST(test_x_plays_first);
    RUN_TEST(test_players_alternate);

    // Basic move tests
    RUN_TEST(test_make_move_success);
    RUN_TEST(test_make_move_invalid_position);
    RUN_TEST(test_make_move_occupied_cell);

    // Winning tests
    RUN_TEST(test_x_wins_top_row);
    RUN_TEST(test_o_wins_middle_column);
    RUN_TEST(test_x_wins_diagonal);
    RUN_TEST(test_o_wins_anti_diagonal);

    // Draw tests
    RUN_TEST(test_game_draw);

    // Reset tests
    RUN_TEST(test_game_reset);

    return UNITY_END();
}
