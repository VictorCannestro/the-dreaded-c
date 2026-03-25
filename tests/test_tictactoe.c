#include "unity.h"
#include "tictactoe.h"
#include "board.h"
#include "ai.h"
#include "ai_easy.h"
#include "win_condition_calculator.h"


void setUp(void) {}
void tearDown(void) {}


/* ===== game_init_session ===== */
void test_game_init_session_creates_empty_board(void) {
    GameState game;
    game_init_session(&game);

    for (int i = 0; i < BOARD_SIZE; i++) {
        TEST_ASSERT_EQUAL(CELL_EMPTY, game.board[i]);
    }
}

void test_game_init_session_sets_default_state(void) {
    GameState game;
    game_init_session(&game);

    TEST_ASSERT_EQUAL(ONGOING, game.status);
    TEST_ASSERT_EQUAL(0, game.move_count);
    TEST_ASSERT_EQUAL(0, game.game_count);
    TEST_ASSERT_EQUAL(CELL_EMPTY, game.last_winner);
}

void test_game_init_session_sets_default_players(void) {
    GameState game;
    game_init_session(&game);

    TEST_ASSERT_EQUAL(PLAYER_HUMAN,    game.players[0].type);
    TEST_ASSERT_EQUAL(CELL_X,          game.players[0].symbol);
    TEST_ASSERT_EQUAL(PLAYER_COMPUTER, game.players[1].type);
    TEST_ASSERT_EQUAL(CELL_O,          game.players[1].symbol);
}

void test_game_init_session_sets_default_difficulty(void) {
    GameState game;
    game_init_session(&game);

    TEST_ASSERT_EQUAL(DIFFICULTY_HARD, game.difficulty);
}

void test_game_init_session_null_does_not_crash(void) {
    game_init_session(NULL);
    TEST_PASS();
}


/* ===== game_new_game ===== */
void test_game_new_game_resets_board(void) {
    GameState game;
    game_init_session(&game);
    game_make_move(&game, 0);
    game_make_move(&game, 1);

    game_new_game(&game);

    for (int i = 0; i < BOARD_SIZE; i++) {
        TEST_ASSERT_EQUAL(CELL_EMPTY, game.board[i]);
    }
}

void test_game_new_game_resets_status_and_move_count(void) {
    GameState game;
    game_init_session(&game);
    game_make_move(&game, 0);

    game_new_game(&game);

    TEST_ASSERT_EQUAL(ONGOING, game.status);
    TEST_ASSERT_EQUAL(0, game.move_count);
}

void test_game_new_game_increments_game_count(void) {
    GameState game;
    game_init_session(&game);

    game_new_game(&game);
    TEST_ASSERT_EQUAL(1, game.game_count);

    game_new_game(&game);
    TEST_ASSERT_EQUAL(2, game.game_count);
}

void test_game_new_game_preserves_player_assignments(void) {
    GameState game;
    game_init_session(&game);
    game_set_player_types(&game, PLAYER_COMPUTER, PLAYER_HUMAN);

    game_new_game(&game);

    TEST_ASSERT_EQUAL(PLAYER_COMPUTER, game_get_player_type(&game, CELL_X));
    TEST_ASSERT_EQUAL(PLAYER_HUMAN,    game_get_player_type(&game, CELL_O));
}

void test_game_new_game_preserves_difficulty(void) {
    GameState game;
    game_init_session(&game);
    game_set_difficulty(&game, DIFFICULTY_EASY);

    game_new_game(&game);

    TEST_ASSERT_EQUAL(DIFFICULTY_EASY, game.difficulty);
}

void test_game_new_game_null_does_not_crash(void) {
    game_new_game(NULL);
    TEST_PASS();
}


/* ===== game_make_move ===== */
void test_game_make_move_places_x_first(void) {
    GameState game;
    game_init_session(&game);

    int result = game_make_move(&game, 0);

    TEST_ASSERT_EQUAL(0, result);
    TEST_ASSERT_EQUAL(CELL_X, game.board[0]);
    TEST_ASSERT_EQUAL(1, game.move_count);
}

void test_game_make_move_alternates_symbols(void) {
    GameState game;
    game_init_session(&game);

    game_make_move(&game, 0);
    game_make_move(&game, 1);

    TEST_ASSERT_EQUAL(CELL_X, game.board[0]);
    TEST_ASSERT_EQUAL(CELL_O, game.board[1]);
}

void test_game_make_move_returns_error_for_out_of_range(void) {
    GameState game;
    game_init_session(&game);

    TEST_ASSERT_EQUAL(-1, game_make_move(&game, -1));
    TEST_ASSERT_EQUAL(-1, game_make_move(&game, BOARD_SIZE));
    TEST_ASSERT_EQUAL(0, game.move_count);
}

void test_game_make_move_returns_error_for_occupied_cell(void) {
    GameState game;
    game_init_session(&game);
    game_make_move(&game, 0);

    int result = game_make_move(&game, 0);

    TEST_ASSERT_EQUAL(-1, result);
    TEST_ASSERT_EQUAL(1, game.move_count);
}

void test_game_make_move_rejected_after_game_over(void) {
    GameState game;
    game_init_session(&game);
    /* X wins top row */
    game_make_move(&game, 0);
    game_make_move(&game, 3);
    game_make_move(&game, 1);
    game_make_move(&game, 4);
    game_make_move(&game, 2);

    int result = game_make_move(&game, 5);

    TEST_ASSERT_EQUAL(-1, result);
    TEST_ASSERT_EQUAL(5, game.move_count);
}

void test_game_make_move_null_returns_error(void) {
    TEST_ASSERT_EQUAL(-1, game_make_move(NULL, 0));
}


/* ===== game_get_status ===== */
void test_game_get_status_ongoing_at_start(void) {
    GameState game;
    game_init_session(&game);

    TEST_ASSERT_EQUAL(ONGOING, game_get_status(&game));
}

void test_game_get_status_null_returns_ongoing(void) {
    TEST_ASSERT_EQUAL(ONGOING, game_get_status(NULL));
}


/* ===== game_is_valid_move ===== */

void test_game_is_valid_move_true_for_empty_cell(void) {
    GameState game;
    game_init_session(&game);

    TEST_ASSERT_EQUAL(1, game_is_valid_move(&game, 0));
    TEST_ASSERT_EQUAL(1, game_is_valid_move(&game, 4));
    TEST_ASSERT_EQUAL(1, game_is_valid_move(&game, 8));
}

void test_game_is_valid_move_false_for_occupied_cell(void) {
    GameState game;
    game_init_session(&game);
    game_make_move(&game, 0);

    TEST_ASSERT_EQUAL(0, game_is_valid_move(&game, 0));
}

void test_game_is_valid_move_false_out_of_range(void) {
    GameState game;
    game_init_session(&game);

    TEST_ASSERT_EQUAL(0, game_is_valid_move(&game, -1));
    TEST_ASSERT_EQUAL(0, game_is_valid_move(&game, BOARD_SIZE));
    TEST_ASSERT_EQUAL(0, game_is_valid_move(&game, 100));
}

void test_game_is_valid_move_false_when_game_over(void) {
    GameState game;
    game_init_session(&game);
    game_make_move(&game, 0);
    game_make_move(&game, 3);
    game_make_move(&game, 1);
    game_make_move(&game, 4);
    game_make_move(&game, 2); /* X wins */

    TEST_ASSERT_EQUAL(0, game_is_valid_move(&game, 5));
}


/* ===== game_get_current_player ===== */
void test_game_get_current_player_x_goes_first(void) {
    GameState game;
    game_init_session(&game);

    TEST_ASSERT_EQUAL(CELL_X, game_get_current_player(&game));
}

void test_game_get_current_player_alternates(void) {
    GameState game;
    game_init_session(&game);

    game_make_move(&game, 0);
    TEST_ASSERT_EQUAL(CELL_O, game_get_current_player(&game));

    game_make_move(&game, 1);
    TEST_ASSERT_EQUAL(CELL_X, game_get_current_player(&game));
}

void test_game_get_current_player_null_returns_x(void) {
    TEST_ASSERT_EQUAL(CELL_X, game_get_current_player(NULL));
}


/* ===== game_get_player_type ===== */
void test_game_get_player_type_default(void) {
    GameState game;
    game_init_session(&game);

    TEST_ASSERT_EQUAL(PLAYER_HUMAN,    game_get_player_type(&game, CELL_X));
    TEST_ASSERT_EQUAL(PLAYER_COMPUTER, game_get_player_type(&game, CELL_O));
}

void test_game_get_player_type_null_returns_human(void) {
    TEST_ASSERT_EQUAL(PLAYER_HUMAN, game_get_player_type(NULL, CELL_X));
}

void test_game_get_player_type_empty_symbol_returns_human(void) {
    GameState game;
    game_init_session(&game);

    TEST_ASSERT_EQUAL(PLAYER_HUMAN, game_get_player_type(&game, CELL_EMPTY));
}


/* ===== game_set_player_types ===== */
void test_game_set_player_types_swaps_assignments(void) {
    GameState game;
    game_init_session(&game);

    game_set_player_types(&game, PLAYER_COMPUTER, PLAYER_HUMAN);

    TEST_ASSERT_EQUAL(PLAYER_COMPUTER, game_get_player_type(&game, CELL_X));
    TEST_ASSERT_EQUAL(PLAYER_HUMAN,    game_get_player_type(&game, CELL_O));
}

void test_game_set_player_types_null_does_not_crash(void) {
    game_set_player_types(NULL, PLAYER_HUMAN, PLAYER_COMPUTER);
    TEST_PASS();
}


/* ===== game_set_human_symbol_choice ===== */

void test_game_set_human_symbol_choice_human_takes_o(void) {
    GameState game;
    game_init_session(&game);

    game_set_human_symbol_choice(&game, CELL_O);

    TEST_ASSERT_EQUAL(PLAYER_COMPUTER, game_get_player_type(&game, CELL_X));
    TEST_ASSERT_EQUAL(PLAYER_HUMAN,    game_get_player_type(&game, CELL_O));
}

void test_game_set_human_symbol_choice_human_takes_x(void) {
    GameState game;
    game_init_session(&game);
    /* Flip to O first, then back to X */
    game_set_human_symbol_choice(&game, CELL_O);
    game_set_human_symbol_choice(&game, CELL_X); /* should be ignored, game_count still 0 */

    /* game_count is still 0 so the second call runs */
    TEST_ASSERT_EQUAL(PLAYER_HUMAN,    game_get_player_type(&game, CELL_X));
    TEST_ASSERT_EQUAL(PLAYER_COMPUTER, game_get_player_type(&game, CELL_O));
}

void test_game_set_human_symbol_choice_ignored_after_first_game(void) {
    GameState game;
    game_init_session(&game);
    game_new_game(&game); /* game_count becomes 1 */

    game_set_human_symbol_choice(&game, CELL_O); /* must be ignored */

    /* Default assignment must be unchanged */
    TEST_ASSERT_EQUAL(PLAYER_HUMAN,    game_get_player_type(&game, CELL_X));
    TEST_ASSERT_EQUAL(PLAYER_COMPUTER, game_get_player_type(&game, CELL_O));
}


/* ===== game_set_winner_symbol_choice ===== */
void test_game_set_winner_symbol_choice_winner_takes_o(void) {
    GameState game;
    game_init_session(&game);
    game_new_game(&game); /* game_count = 1 */

    game_set_winner_symbol_choice(&game, CELL_O);

    TEST_ASSERT_EQUAL(PLAYER_COMPUTER, game_get_player_type(&game, CELL_X));
    TEST_ASSERT_EQUAL(PLAYER_HUMAN,    game_get_player_type(&game, CELL_O));
}

void test_game_set_winner_symbol_choice_winner_takes_x(void) {
    GameState game;
    game_init_session(&game);
    game_new_game(&game);

    game_set_winner_symbol_choice(&game, CELL_X);

    TEST_ASSERT_EQUAL(PLAYER_HUMAN,    game_get_player_type(&game, CELL_X));
    TEST_ASSERT_EQUAL(PLAYER_COMPUTER, game_get_player_type(&game, CELL_O));
}

void test_game_set_winner_symbol_choice_null_does_not_crash(void) {
    game_set_winner_symbol_choice(NULL, CELL_X);
    TEST_PASS();
}


/* ===== game_set_difficulty ===== */
void test_game_set_difficulty_easy(void) {
    GameState game;
    game_init_session(&game);

    game_set_difficulty(&game, DIFFICULTY_EASY);

    TEST_ASSERT_EQUAL(DIFFICULTY_EASY, game.difficulty);
}

void test_game_set_difficulty_hard(void) {
    GameState game;
    game_init_session(&game);
    game_set_difficulty(&game, DIFFICULTY_EASY);

    game_set_difficulty(&game, DIFFICULTY_HARD);

    TEST_ASSERT_EQUAL(DIFFICULTY_HARD, game.difficulty);
}

void test_game_set_difficulty_null_does_not_crash(void) {
    game_set_difficulty(NULL, DIFFICULTY_EASY);
    TEST_PASS();
}


/* ===== game_is_over ===== */
void test_game_is_over_false_at_start(void) {
    GameState game;
    game_init_session(&game);

    TEST_ASSERT_EQUAL(0, game_is_over(&game));
}

void test_game_is_over_true_after_x_wins(void) {
    GameState game;
    game_init_session(&game);
    game_make_move(&game, 0);
    game_make_move(&game, 3);
    game_make_move(&game, 1);
    game_make_move(&game, 4);
    game_make_move(&game, 2);

    TEST_ASSERT_EQUAL(1, game_is_over(&game));
}

void test_game_is_over_true_after_draw(void) {
    GameState game;
    game_init_session(&game);
    game_make_move(&game, 0);
    game_make_move(&game, 3);
    game_make_move(&game, 1);
    game_make_move(&game, 4);
    game_make_move(&game, 5);
    game_make_move(&game, 2);
    game_make_move(&game, 6);
    game_make_move(&game, 8);
    game_make_move(&game, 7);

    TEST_ASSERT_EQUAL(1, game_is_over(&game));
}

void test_game_is_over_null_returns_false(void) {
    TEST_ASSERT_EQUAL(0, game_is_over(NULL));
}


/* ===== Win conditions — all 8 lines ===== */
void test_x_wins_top_row(void) {
    GameState game;
    game_init_session(&game);
    game_make_move(&game, 0); game_make_move(&game, 3);
    game_make_move(&game, 1); game_make_move(&game, 4);
    game_make_move(&game, 2);

    TEST_ASSERT_EQUAL(X_WINS, game.status);
    TEST_ASSERT_EQUAL(CELL_X, game.last_winner);
}

void test_x_wins_middle_row(void) {
    GameState game;
    game_init_session(&game);
    game_make_move(&game, 3); game_make_move(&game, 0);
    game_make_move(&game, 4); game_make_move(&game, 1);
    game_make_move(&game, 5);

    TEST_ASSERT_EQUAL(X_WINS, game.status);
    TEST_ASSERT_EQUAL(CELL_X, game.last_winner);
}

void test_x_wins_bottom_row(void) {
    GameState game;
    game_init_session(&game);
    game_make_move(&game, 6); game_make_move(&game, 0);
    game_make_move(&game, 7); game_make_move(&game, 1);
    game_make_move(&game, 8);

    TEST_ASSERT_EQUAL(X_WINS, game.status);
    TEST_ASSERT_EQUAL(CELL_X, game.last_winner);
}

void test_o_wins_left_column(void) {
    GameState game;
    game_init_session(&game);
    game_make_move(&game, 1); game_make_move(&game, 0);
    game_make_move(&game, 2); game_make_move(&game, 3);
    game_make_move(&game, 4); game_make_move(&game, 6);

    TEST_ASSERT_EQUAL(O_WINS, game.status);
    TEST_ASSERT_EQUAL(CELL_O, game.last_winner);
}

void test_o_wins_middle_column(void) {
    GameState game;
    game_init_session(&game);
    game_make_move(&game, 0); game_make_move(&game, 1);
    game_make_move(&game, 2); game_make_move(&game, 4);
    game_make_move(&game, 5); game_make_move(&game, 7);

    TEST_ASSERT_EQUAL(O_WINS, game.status);
    TEST_ASSERT_EQUAL(CELL_O, game.last_winner);
}

void test_o_wins_right_column(void) {
    GameState game;
    game_init_session(&game);
    game_make_move(&game, 0); game_make_move(&game, 2);
    game_make_move(&game, 1); game_make_move(&game, 5);
    game_make_move(&game, 3); game_make_move(&game, 8);

    TEST_ASSERT_EQUAL(O_WINS, game.status);
    TEST_ASSERT_EQUAL(CELL_O, game.last_winner);
}

void test_x_wins_main_diagonal(void) {
    GameState game;
    game_init_session(&game);
    game_make_move(&game, 0); game_make_move(&game, 1);
    game_make_move(&game, 4); game_make_move(&game, 2);
    game_make_move(&game, 8);

    TEST_ASSERT_EQUAL(X_WINS, game.status);
    TEST_ASSERT_EQUAL(CELL_X, game.last_winner);
}

void test_o_wins_anti_diagonal(void) {
    GameState game;
    game_init_session(&game);
    game_make_move(&game, 0); game_make_move(&game, 2);
    game_make_move(&game, 1); game_make_move(&game, 4);
    game_make_move(&game, 3); game_make_move(&game, 6);

    TEST_ASSERT_EQUAL(O_WINS, game.status);
    TEST_ASSERT_EQUAL(CELL_O, game.last_winner);
}


/* ===== Draw ===== */

void test_game_draw_fills_board(void) {
    GameState game;
    game_init_session(&game);
    /* X O X / O O X / X X O — draw */
    game_make_move(&game, 0); game_make_move(&game, 3);
    game_make_move(&game, 1); game_make_move(&game, 4);
    game_make_move(&game, 5); game_make_move(&game, 2);
    game_make_move(&game, 6); game_make_move(&game, 8);
    game_make_move(&game, 7);

    TEST_ASSERT_EQUAL(DRAW, game.status);
    TEST_ASSERT_EQUAL(BOARD_SIZE, game.move_count);
}


/* ===== Hard AI ===== */
void test_hard_ai_returns_valid_position(void) {
    GameState game;
    game_init_session(&game);

    int move = game_get_computer_move(&game);

    TEST_ASSERT_TRUE(move >= 0 && move < BOARD_SIZE);
    TEST_ASSERT_EQUAL(CELL_EMPTY, game.board[move]);
}

void test_hard_ai_blocks_immediate_win(void) {
    GameState game;
    game_init_session(&game);
    /* X has 0 and 1, needs 2 to win — O must block at 2 */
    game_make_move(&game, 0);
    game_make_move(&game, 3);
    game_make_move(&game, 1);

    int move = game_get_computer_move(&game);

    TEST_ASSERT_EQUAL(2, move);
}

void test_hard_ai_takes_winning_move(void) {
    GameState game;
    game_init_session(&game);
    game_set_player_types(&game, PLAYER_COMPUTER, PLAYER_HUMAN);
    /* X (computer) has 0 and 1, needs 2 to win */
    game.board[0] = CELL_X;
    game.board[1] = CELL_X;
    game.board[3] = CELL_O;
    game.board[4] = CELL_O;
    game.move_count = 4;

    int move = game_get_computer_move(&game);

    TEST_ASSERT_EQUAL(2, move);
}


/* ===== Easy AI ===== */
void test_easy_ai_returns_valid_position(void) {
    GameState game;
    game_init_session(&game);
    game_set_difficulty(&game, DIFFICULTY_EASY);

    int move = game_get_computer_move(&game);

    TEST_ASSERT_TRUE(move >= 0 && move < BOARD_SIZE);
    TEST_ASSERT_EQUAL(CELL_EMPTY, game.board[move]);
}

void test_easy_ai_returns_position_on_nearly_full_board(void) {
    GameState game;
    game_init_session(&game);
    game_set_difficulty(&game, DIFFICULTY_EASY);
    /* Leave only position 7 empty */
    game.board[0] = CELL_X; game.board[1] = CELL_O;
    game.board[2] = CELL_X; game.board[3] = CELL_O;
    game.board[4] = CELL_X; game.board[5] = CELL_O;
    game.board[6] = CELL_X; game.board[8] = CELL_O;
    game.move_count = 8;

    int move = game_get_computer_move(&game);

    TEST_ASSERT_EQUAL(7, move);
}
