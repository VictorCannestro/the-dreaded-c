#include "unity.h"
#include "tictactoe.h"


void setUp(void) {
    // This function is called before each test
}

void tearDown(void) {
    // This function is called after each test
}

// ===== Initialization Tests =====
void test_game_init_session_creates_empty_board(void) {
    GameState game;
    game_init_session(&game);

    for (int i = 0; i < 9; i++) {
        TEST_ASSERT_EQUAL(CELL_EMPTY, game.board[i]);
    }
    TEST_ASSERT_EQUAL(GAME_ONGOING, game.status);
    TEST_ASSERT_EQUAL(0, game.move_count);
    TEST_ASSERT_EQUAL(0, game.game_count);
    TEST_ASSERT_EQUAL(CELL_EMPTY, game.last_winner);
    TEST_ASSERT_EQUAL(PLAYER_HUMAN, game.players[0].type);    // X player
    TEST_ASSERT_EQUAL(PLAYER_COMPUTER, game.players[1].type); // O player
}

void test_game_new_game_resets_board(void) {
    GameState game;
    game_init_session(&game);

    game_make_move(&game, 0);
    game_make_move(&game, 1);

    game_new_game(&game);

    for (int i = 0; i < 9; i++) {
        TEST_ASSERT_EQUAL(CELL_EMPTY, game.board[i]);
    }
    TEST_ASSERT_EQUAL(GAME_ONGOING, game.status);
    TEST_ASSERT_EQUAL(0, game.move_count);
    TEST_ASSERT_EQUAL(1, game.game_count); // Game count should increment
}

void test_game_init_with_null_pointer(void) {
    // Should not crash
    game_init_session(NULL);
    TEST_PASS();
}


// ===== Move Validation Tests =====
void test_valid_move_on_empty_cell(void) {
    GameState game;
    game_init_session(&game);

    TEST_ASSERT_EQUAL(1, game_is_valid_move(&game, 0));
    TEST_ASSERT_EQUAL(1, game_is_valid_move(&game, 4));
    TEST_ASSERT_EQUAL(1, game_is_valid_move(&game, 8));
}

void test_invalid_move_out_of_range(void) {
    GameState game;
    game_init_session(&game);

    TEST_ASSERT_EQUAL(0, game_is_valid_move(&game, -1));
    TEST_ASSERT_EQUAL(0, game_is_valid_move(&game, 9));
    TEST_ASSERT_EQUAL(0, game_is_valid_move(&game, 100));
}

void test_invalid_move_on_occupied_cell(void) {
    GameState game;
    game_init_session(&game);

    game_make_move(&game, 0);  // X plays at 0
    TEST_ASSERT_EQUAL(0, game_is_valid_move(&game, 0));
}


// ===== Player Turn Tests =====
void test_x_plays_first(void) {
    GameState game;
    game_init_session(&game);

    CellValue player = game_get_current_player(&game);
    TEST_ASSERT_EQUAL(CELL_X, player);
}

void test_players_alternate(void) {
    GameState game;
    game_init_session(&game);

    TEST_ASSERT_EQUAL(CELL_X, game_get_current_player(&game));
    game_make_move(&game, 0);
    TEST_ASSERT_EQUAL(CELL_O, game_get_current_player(&game));
    game_make_move(&game, 1);
    TEST_ASSERT_EQUAL(CELL_X, game_get_current_player(&game));
}


// ===== Player Type Tests =====
void test_default_player_types(void) {
    GameState game;
    game_init_session(&game);

    TEST_ASSERT_EQUAL(PLAYER_HUMAN, game_get_player_type(&game, CELL_X));
    TEST_ASSERT_EQUAL(PLAYER_COMPUTER, game_get_player_type(&game, CELL_O));
}

void test_set_player_types(void) {
    GameState game;
    game_init_session(&game);

    game_set_player_types(&game, PLAYER_COMPUTER, PLAYER_HUMAN);

    TEST_ASSERT_EQUAL(PLAYER_COMPUTER, game_get_player_type(&game, CELL_X));
    TEST_ASSERT_EQUAL(PLAYER_HUMAN, game_get_player_type(&game, CELL_O));
}

void test_human_symbol_choice(void) {
    GameState game;
    game_init_session(&game);

    game_set_human_symbol_choice(&game, CELL_O);

    TEST_ASSERT_EQUAL(PLAYER_COMPUTER, game_get_player_type(&game, CELL_X));
    TEST_ASSERT_EQUAL(PLAYER_HUMAN, game_get_player_type(&game, CELL_O));
}


// ===== Basic Move Tests =====
void test_make_move_success(void) {
    GameState game;
    game_init_session(&game);

    int result = game_make_move(&game, 0);
    TEST_ASSERT_EQUAL(0, result);
    TEST_ASSERT_EQUAL(CELL_X, game.board[0]);
    TEST_ASSERT_EQUAL(1, game.move_count);
}

void test_make_move_invalid_position(void) {
    GameState game;
    game_init_session(&game);

    int result = game_make_move(&game, 10);
    TEST_ASSERT_EQUAL(-1, result);
    TEST_ASSERT_EQUAL(0, game.move_count);
}

void test_make_move_occupied_cell(void) {
    GameState game;
    game_init_session(&game);

    game_make_move(&game, 0);  // X plays at 0
    int result = game_make_move(&game, 0);  // O tries to play at 0
    TEST_ASSERT_EQUAL(-1, result);
    TEST_ASSERT_EQUAL(1, game.move_count);
}


// ===== Winning Tests =====
void test_x_wins_top_row(void) {
    GameState game;
    game_init_session(&game);

    // X: 0, O: 3, X: 1, O: 4, X: 2
    game_make_move(&game, 0);  // X
    game_make_move(&game, 3);  // O
    game_make_move(&game, 1);  // X
    game_make_move(&game, 4);  // O
    game_make_move(&game, 2);  // X wins

    TEST_ASSERT_EQUAL(GAME_X_WINS, game.status);
    TEST_ASSERT_EQUAL(CELL_X, game.last_winner);
}

void test_o_wins_middle_column(void) {
    GameState game;
    game_init_session(&game);

    // X: 0, O: 1, X: 2, O: 4, X: 5, O: 7
    game_make_move(&game, 0);  // X
    game_make_move(&game, 1);  // O
    game_make_move(&game, 2);  // X
    game_make_move(&game, 4);  // O
    game_make_move(&game, 5);  // X
    game_make_move(&game, 7);  // O wins

    TEST_ASSERT_EQUAL(GAME_O_WINS, game.status);
    TEST_ASSERT_EQUAL(CELL_O, game.last_winner);
}

void test_x_wins_diagonal(void) {
    GameState game;
    game_init_session(&game);

    // X: 0, O: 1, X: 4, O: 2, X: 8
    game_make_move(&game, 0);  // X
    game_make_move(&game, 1);  // O
    game_make_move(&game, 4);  // X
    game_make_move(&game, 2);  // O
    game_make_move(&game, 8);  // X wins diagonal

    TEST_ASSERT_EQUAL(GAME_X_WINS, game.status);
    TEST_ASSERT_EQUAL(CELL_X, game.last_winner);
}

void test_o_wins_anti_diagonal(void) {
    GameState game;
    game_init_session(&game);

    // X: 0, O: 2, X: 1, O: 4, X: 3, O: 6
    game_make_move(&game, 0);  // X
    game_make_move(&game, 2);  // O
    game_make_move(&game, 1);  // X
    game_make_move(&game, 4);  // O
    game_make_move(&game, 3);  // X
    game_make_move(&game, 6);  // O wins anti-diagonal

    TEST_ASSERT_EQUAL(GAME_O_WINS, game.status);
    TEST_ASSERT_EQUAL(CELL_O, game.last_winner);
}


// ===== Draw Tests =====
void test_game_draw(void) {
    GameState game;
    game_init_session(&game);

    // Fill board without winner: X-X-O / O-O-X / X-O-X (this creates a draw)
    game_make_move(&game, 0);  // X
    game_make_move(&game, 3);  // O
    game_make_move(&game, 1);  // X
    game_make_move(&game, 4);  // O
    game_make_move(&game, 5);  // X
    game_make_move(&game, 2);  // O
    game_make_move(&game, 6);  // X
    game_make_move(&game, 8);  // O
    game_make_move(&game, 7);  // X

    TEST_ASSERT_EQUAL(GAME_DRAW, game.status);
    TEST_ASSERT_EQUAL(9, game.move_count);
}


// ===== Reset Tests =====
void test_game_reset(void) {
    GameState game;
    game_init_session(&game);

    game_make_move(&game, 0);
    game_make_move(&game, 1);
    game_reset(&game);

    TEST_ASSERT_EQUAL(GAME_ONGOING, game.status);
    TEST_ASSERT_EQUAL(0, game.move_count);
    TEST_ASSERT_EQUAL(CELL_EMPTY, game.board[0]);
    TEST_ASSERT_EQUAL(CELL_EMPTY, game.board[1]);
}


// ===== Computer AI Tests =====
void test_computer_move_returns_valid_position(void) {
    GameState game;
    game_init_session(&game);

    int move = game_get_computer_move(&game);
    TEST_ASSERT_TRUE(move >= 0 && move <= 8);
    TEST_ASSERT_EQUAL(CELL_EMPTY, game.board[move]);
}

void test_computer_move_blocks_win(void) {
    GameState game;
    game_init_session(&game);

    // Set up a situation where human (X) is about to win
    game_make_move(&game, 0);  // X
    game_make_move(&game, 3);  // O
    game_make_move(&game, 1);  // X - now X has 0 and 1, needs 2 to win

    // Computer (O) should block by playing position 2
    int move = game_get_computer_move(&game);
    TEST_ASSERT_EQUAL(2, move);
}



