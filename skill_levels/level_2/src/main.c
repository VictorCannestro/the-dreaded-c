/*
 * Tic-Tac-Toe - Main Entry Point
 *
 * v2 Improvement: main() is now a thin coordinator.
 * All logic lives in separate modules.
 */

#include "types.h"
#include "game.h"
#include "display.h"
#include "input.h"
#include "ai.h"

#include <stdio.h>

int main(void) {
    GameState state;
    CellValue human_symbol;
    CellValue current;
    PlayerType current_type;
    int move;

    printf("Welcome to Tic-Tac-Toe!\n");
    printf("========================\n\n");

    /* Get player's symbol choice */
    human_symbol = input_get_symbol_choice();

    /* Set up players */
    if (human_symbol == CELL_X) {
        printf("You are X, computer is O.\n\n");
        game_init(&state);
        game_set_players(&state, PLAYER_HUMAN, PLAYER_COMPUTER);
    } else {
        printf("You are O, computer is X.\n\n");
        game_init(&state);
        game_set_players(&state, PLAYER_COMPUTER, PLAYER_HUMAN);
    }

    /* Main game loop */
    do {
        while (!game_is_over(&state)) {
            display_board(&state);

            current = game_get_current_player(&state);
            current_type = game_get_player_type(&state, current);

            if (current_type == PLAYER_HUMAN) {
                printf("Your turn (%c)\n", (current == CELL_X) ? 'X' : 'O');
                move = input_get_move(&state);
            } else {
                printf("Computer's turn (%c)\n", (current == CELL_X) ? 'X' : 'O');
                move = ai_get_move(&state);
                printf("Computer plays position %d\n", move);
            }

            game_make_move(&state, move);
        }

        /* Show final state and result */
        display_board(&state);
        display_result(state.status);

        /* Play again? */
        if (!input_play_again()) {
            break;
        }

        /* Reset for new game (keeping same player assignments) */
        game_init(&state);
        if (human_symbol == CELL_O) {
            game_set_players(&state, PLAYER_COMPUTER, PLAYER_HUMAN);
        }

    } while (1);

    printf("\nThanks for playing!\n");
    return 0;
}

