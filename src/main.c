#include "tictactoe.h"
#include "display.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    GameState game;
    game_init(&game);

    printf("Welcome to Tic-Tac-Toe!\n");
    printf("========================\n");

    while (game.status == GAME_ONGOING) {
        display_board(&game);

        CellValue current_player = game_get_current_player(&game);
        char player_symbol = (current_player == CELL_X) ? 'X' : 'O';

        printf("Player %c's turn. Enter position (0-8): ", player_symbol);
        int position;
        if (scanf("%d", &position) != 1) {
            // Clear invalid input from stdin
            while (getchar() != '\n');
            printf("Invalid input. Please enter a number between 0 and 8.\n");
            continue;
        }

        if (!game_is_valid_move(&game, position)) {
            printf("Invalid move! Position %d is already occupied or out of range.\n", position);
            continue;
        }

        game_make_move(&game, position);
    }

    display_board(&game);
    display_status(game.status);

    return 0;
}
