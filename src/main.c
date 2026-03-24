#include "tictactoe.h"
#include "display.h"
#include "constants.h"
#include "human.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>


static void play_game(GameState *game) {
    while (!game_is_over(game)) {
        display_board(game);

        CellValue current_player = game_get_current_player(game);
        PlayerType player_type = game_get_player_type(game, current_player);
        char player_symbol = (current_player == CELL_X) ? X_MARKER : O_MARKER;

        printf("Player %c's turn", player_symbol);

        int move;
        if (player_type == PLAYER_HUMAN) {
            printf(" (Human)\n");
            move = human_get_move(game);
        } else {
            printf(" (Computer)\n");
            move = game_get_computer_move(game);
            printf("Computer plays position %d\n", move);
        }

        game_make_move(game, move);
    }

    display_board(game);
    display_status(game->status);
}

static int ask_play_again(void) {
    char choice;
    while (1) {
        printf("Play another game? (y/n): ");
        if (scanf(" %c", &choice) != 1) {
            clear_input_buffer();
            continue;
        }

        choice = tolower(choice);
        if (choice == 'y') {
            return 1;
        } 
        if (choice == 'n') {
            return 0;
        } else {
            printf("Please enter y or n.\n");
            clear_input_buffer();
        }
    }
}

int main(void) {
    GameState game;

    printf("Welcome to Tic-Tac-Toe!\n");
    printf("========================\n");
    printf("You will play against the computer.\n\n");

    game_init_session(&game);

    CellValue human_symbol = human_get_symbol_choice();
    game_set_human_symbol_choice(&game, human_symbol);

    printf("You chose %c. ", (human_symbol == CELL_X) ? X_MARKER : O_MARKER);
    printf("Computer will play %c.\n\n", (human_symbol == CELL_X) ? O_MARKER : X_MARKER);

    do {
        play_game(&game);

        if (!ask_play_again()) {
            break;
        }

        // Start new game
        if (game.last_winner != CELL_EMPTY) {
            // Winner chooses symbol for next game
            CellValue winner_choice = human_get_winner_symbol_choice(game.last_winner);
            // Reset game_count to allow symbol choice update
            game.game_count = 0;
            game_set_human_symbol_choice(&game, winner_choice);
            printf("Next game: You play %c, Computer plays %c\n\n",
                   (winner_choice == CELL_X) ? X_MARKER : O_MARKER,
                   (winner_choice == CELL_X) ? O_MARKER : X_MARKER);
            game.game_count = 1;  // Restore count before game_new_game increments
        } else {
            printf("Draw! Keeping same symbols for next game.\n\n");
        }

        game_new_game(&game);

    } while (1);

    printf("\nThanks for playing Tic-Tac-Toe!\n");
    return 0;
}
