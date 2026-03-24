#include "tictactoe.h"
#include "ai.h"

#include <string.h>
#include <stdlib.h>
#include <time.h>


/*
 * Internal helper: Check for a winner
 * Returns CELL_X if X wins, CELL_O if O wins, CELL_EMPTY if no winner
 */
static CellValue check_winner(const CellValue board[9]) {
    // Winning combinations (row, col, diagonal)
    const int winning_combos[8][3] = {
        {0, 1, 2},  // Top row
        {3, 4, 5},  // Middle row
        {6, 7, 8},  // Bottom row
        {0, 3, 6},  // Left column
        {1, 4, 7},  // Middle column
        {2, 5, 8},  // Right column
        {0, 4, 8},  // Diagonal (top-left to bottom-right)
        {2, 4, 6}   // Diagonal (top-right to bottom-left)
    };

    for (int i = 0; i < 8; i++) {
        int a = winning_combos[i][0];
        int b = winning_combos[i][1];
        int c = winning_combos[i][2];

        if (board[a] != CELL_EMPTY &&
            board[a] == board[b] &&
            board[b] == board[c]) {
            return board[a];  // Return the winner
        }
    }

    return CELL_EMPTY;  // No winner
}

void game_init_session(GameState *state) {
    if (state == NULL) {
        return;
    }

    // Initialize with default values
    memset(state->board, CELL_EMPTY, 9 * sizeof(CellValue));
    state->status = ONGOING;
    state->move_count = 0;
    state->game_count = 0;
    state->last_winner = CELL_EMPTY;

    // Default: human plays X, computer plays O
    state->players[0].type = PLAYER_HUMAN;    
    state->players[0].symbol = CELL_X;
    state->players[1].type = PLAYER_COMPUTER; 
    state->players[1].symbol = CELL_O;

    // Seed random number generator for AI
    srand((unsigned)(time(NULL)));
}

void game_new_game(GameState *state) {
    if (state == NULL) {
        return;
    }

    // Reset board and game state
    memset(state->board, CELL_EMPTY, 9 * sizeof(CellValue));
    state->status = ONGOING;
    state->move_count = 0;
    state->game_count++;
}

int game_make_move(GameState *state, int position) {
    if (state == NULL || position < 0 || position > 8) {
        return -1;  // Invalid input
    }
    if (state->board[position] != CELL_EMPTY) {
        return -1;  // Position already occupied
    }
    if (state->status != ONGOING) {
        return -1;  // Game already finished
    }

    CellValue player = game_get_current_player(state);
    state->board[position] = player;
    state->move_count++;

    CellValue winner = check_winner(state->board);
    if (winner == CELL_X) {
        state->status = X_WINS;
        state->last_winner = CELL_X;
        return 0;
    } else if (winner == CELL_O) {
        state->status = O_WINS;
        state->last_winner = CELL_O;
        return 0;
    }
    if (state->move_count == 9) {
        state->status = DRAW;
        return 0;
    }
    return 0;  // Move successful
}

GameStatus game_get_status(GameState *state) {
    if (state == NULL) {
        return ONGOING;
    }
    return state->status;
}

int game_is_valid_move(GameState *state, int position) {
    if (state == NULL || position < 0 || position > 8) {
        return 0;
    }
    if (state->board[position] != CELL_EMPTY) {
        return 0;
    }
    if (state->status != ONGOING) {
        return 0;
    }
    return 1;  // Valid move
}

CellValue game_get_current_player(GameState *state) {
    if (state == NULL) {
        return CELL_X;
    }

    // X goes first (even move count), O goes second (odd move count)
    if (state->move_count % 2 == 0) {
        return CELL_X;
    } else {
        return CELL_O;
    }
}

PlayerType game_get_player_type(GameState *state, CellValue symbol) {
    if (state == NULL) {
        return PLAYER_HUMAN;
    }
    if (symbol == CELL_X) {
        return state->players[0].type;
    } else if (symbol == CELL_O) {
        return state->players[1].type;
    }
    return PLAYER_HUMAN;
}

void game_set_player_types(GameState *state, PlayerType x_player, PlayerType o_player) {
    if (state == NULL) {
        return;
    }
    state->players[0].type = x_player;
    state->players[1].type = o_player;
}

void game_set_human_symbol_choice(GameState *state, CellValue human_symbol) {
    if (state == NULL || state->game_count > 0) return;

    // For first game, human chooses their symbol
    if (human_symbol == CELL_X) {
        state->players[0].type = PLAYER_HUMAN;    // X is human
        state->players[1].type = PLAYER_COMPUTER; // O is computer
    } else  {
        state->players[0].type = PLAYER_COMPUTER; // X is computer
        state->players[1].type = PLAYER_HUMAN;    // O is human
    }
}

int game_get_computer_move(GameState *state) {
    return ai_get_computer_move(state);
}

int game_is_over(GameState *state) {
    if (state == NULL) {
        return 0;
    }
    return state->status != ONGOING;
}
