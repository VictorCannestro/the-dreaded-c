#include "human.h"
#include "constants.h"
#include "utils.h"

#include <stdio.h>
#include <ctype.h>


CellValue human_get_symbol_choice(void) {
    char choice;
    while (1) {
        printf("Choose your symbol (X or O): ");
        if (scanf(" %c", &choice) != 1) {
            clear_input_buffer();
            printf("Invalid input. Please enter X or O.\n");
            continue;
        }

        choice = toupper(choice);
        if (choice == X_MARKER) {
            return CELL_X;
        } else if (choice == O_MARKER) {
            return CELL_O;
        } else {
            printf("Invalid choice. Please enter X or O.\n");
            clear_input_buffer();
        }
    }
}

CellValue human_get_winner_symbol_choice(CellValue last_winner) {
    char choice;
    char winner_char = (last_winner == CELL_X) ? X_MARKER : O_MARKER;

    while (1) {
        printf("Player %c (last winner), choose your symbol for the next game (X or O): ", winner_char);
        if (scanf(" %c", &choice) != 1) {
            clear_input_buffer();
            printf("Invalid input. Please enter X or O.\n");
            continue;
        }

        choice = toupper(choice);
        if (choice == X_MARKER) {
            return CELL_X;
        } else if (choice == O_MARKER) {
            return CELL_O;
        } else {
            printf("Invalid choice. Please enter X or O.\n");
            clear_input_buffer();
        }
    }
}

int human_get_move(GameState *game) {
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
