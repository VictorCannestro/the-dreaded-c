/*
 * Input Module - Implementation
 *
 * v2 Improvement: Better input validation.
 * Uses iteration instead of recursion to avoid stack overflow.
 */

#include "input.h"
#include "board.h"
#include <stdio.h>
#include <ctype.h>

void input_clear_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        /* Discard characters */
    }
}

/*
 * v2 Improvement: Loop instead of recursion.
 * In v1, getHumanMove() called itself recursively on bad input.
 * This could overflow the stack with enough bad inputs.
 */
int input_get_move(const GameState *state) {
    int position;

    while (1) {
        printf("Enter position (0-8): ");

        if (scanf("%d", &position) != 1) {
            printf("Invalid input! Please enter a number.\n");
            input_clear_buffer();
            continue;
        }

        if (!board_is_valid_move(state->board, position)) {
            if (position < 0 || position >= BOARD_SIZE) {
                printf("Invalid position! Enter 0-8.\n");
            } else {
                printf("Position %d is already taken!\n", position);
            }
            continue;
        }

        return position;
    }
}

CellValue input_get_symbol_choice(void) {
    char choice;

    while (1) {
        printf("Choose your symbol (X or O): ");

        if (scanf(" %c", &choice) != 1) {
            input_clear_buffer();
            continue;
        }

        choice = toupper(choice);

        if (choice == 'X') {
            return CELL_X;
        } else if (choice == 'O') {
            return CELL_O;
        } else {
            printf("Invalid choice! Enter X or O.\n");
            input_clear_buffer();
        }
    }
}

int input_play_again(void) {
    char choice;

    while (1) {
        printf("Play again? (y/n): ");

        if (scanf(" %c", &choice) != 1) {
            input_clear_buffer();
            continue;
        }

        choice = tolower(choice);

        if (choice == 'y') {
            return 1;
        } else if (choice == 'n') {
            return 0;
        } else {
            printf("Please enter y or n.\n");
            input_clear_buffer();
        }
    }
}

