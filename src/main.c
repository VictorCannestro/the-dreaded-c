#include "tictactoe.h"
#include "display.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>


static void clear_input_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

static CellValue get_human_symbol_choice(void) {
    char choice;
    while (1) {
        printf("Choose your symbol (X or O): ");
        if (scanf(" %c", &choice) != 1) {
            clear_input_buffer();
            printf("Invalid input. Please enter X or O.\n");
            continue;
        }

        choice = toupper(choice);
        if (choice == 'X') {
            return CELL_X;
        } else if (choice == 'O') {
            return CELL_O;
        } else {
            printf("Invalid choice. Please enter X or O.\n");
            clear_input_buffer();
        }
    }
}

static CellValue get_winner_symbol_choice(CellValue last_winner) {
    char choice;
    char winner_char = (last_winner == CELL_X) ? 'X' : 'O';

    while (1) {
        printf("Player %c (last winner), choose your symbol for the next game (X or O): ", winner_char);
        if (scanf(" %c", &choice) != 1) {
            clear_input_buffer();
            printf("Invalid input. Please enter X or O.\n");
            continue;
        }

        choice = toupper(choice);
        if (choice == 'X') {
            return CELL_X;
        } else if (choice == 'O') {
            return CELL_O;
        } else {
            printf("Invalid choice. Please enter X or O.\n");
            clear_input_buffer();
        }
    }
}

static int get_human_move(GameState *game) {
    int position;
    while (1) {
        printf("Enter position (0-8): ");
        if (scanf("%d", &position) != 1) {
            clear_input_buffer();
            printf("Invalid input. Please enter a number between 0 and 8.\n");
            continue;
        }

        if (game_is_valid_move(game, position)) {
            return position;
        } else {
            printf("Invalid move! Position %d is already occupied or out of range.\n", position);
        }
    }
}

static void play_game(GameState *game) {
    while (!game_is_over(game)) {
        display_board(game);

        CellValue current_player = game_get_current_player(game);
        PlayerType player_type = game_get_player_type(game, current_player);
        char player_symbol = (current_player == CELL_X) ? 'X' : 'O';

        printf("Player %c's turn", player_symbol);

        int move;
        if (player_type == PLAYER_HUMAN) {
            printf(" (Human)\n");
            move = get_human_move(game);
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
        } else if (choice == 'n') {
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

    // Initialize the game session
    game_init_session(&game);

    // First game: human chooses symbol
    CellValue human_symbol = get_human_symbol_choice();
    game_set_human_symbol_choice(&game, human_symbol);

    printf("You chose %c. ", (human_symbol == CELL_X) ? 'X' : 'O');
    printf("Computer will play %c.\n\n", (human_symbol == CELL_X) ? 'O' : 'X');

    // Initialize first game board
    game_new_game(&game);

    do {
        play_game(&game);

        if (!ask_play_again()) {
            break;
        }

        // Start new game
        if (game.last_winner != CELL_EMPTY) {
            // Winner chooses symbol for next game
            CellValue winner_choice = get_winner_symbol_choice(game.last_winner);
            // Reset game_count to allow symbol choice update
            game.game_count = 0;
            game_set_human_symbol_choice(&game, winner_choice);
            printf("Next game: You play %c, Computer plays %c\n\n",
                   (winner_choice == CELL_X) ? 'X' : 'O',
                   (winner_choice == CELL_X) ? 'O' : 'X');
            game.game_count = 1;  // Restore count before game_new_game increments
        } else {
            printf("Draw! Keeping same symbols for next game.\n\n");
        }

        game_new_game(&game);

    } while (1);

    printf("\nThanks for playing Tic-Tac-Toe!\n");
    return 0;
}
