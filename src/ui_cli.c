#include "ui_interface.h"
#include "tictactoe.h"
#include "constants.h"
#include "utils.h"

#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>


/* ------------------------------------------------------------------ */
/* Display Functions                                                   */
/* ------------------------------------------------------------------ */
static void cli_display_board(const GameState *state) {
    if (state == NULL) {
        return;
    }

    printf("\n");

    for (int row = 0; row < BOARD_DIM; row++) {
        /* Top padding row */
        for (int col = 0; col < BOARD_DIM; col++) {
            printf("   ");
            if (col < BOARD_DIM - 1) {
                printf("|");
            }
        }
        printf("\n");

        /* Cell values row */
        for (int col = 0; col < BOARD_DIM; col++) {
            int pos = row * BOARD_DIM + col;
            printf(" %c ", cell_value_to_marker(state->board[pos]));
            if (col < BOARD_DIM - 1) {
                printf("|");
            }
        }
        printf("\n");

        /* Bottom border row (or spacing for last row) */
        for (int col = 0; col < BOARD_DIM; col++) {
            if (row < BOARD_DIM - 1) {
                printf("___");
            } else {
                printf("   ");
            }
            if (col < BOARD_DIM - 1) {
                printf("|");
            }
        }
        printf("\n");
    }

    printf("\n");
}

static void cli_display_status(GameStatus status) {
    printf("\n");
    switch (status) {
        case X_WINS:
            printf("*** Player X WINS! ***\n");
            break;
        case O_WINS:
            printf("*** Player O WINS! ***\n");
            break;
        case DRAW:
            printf("*** GAME DRAW ***\n");
            break;
        case ONGOING:
            printf("Game is ongoing...\n");
            break;
    }
    printf("\n");
}

static void cli_display_message(const char *message) {
    printf("%s\n", message);
}

static void cli_display_formatted(const char *format, ...) {
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
}

static void cli_display_error(const char *message) {
    printf("Error: %s\n", message);
}


/* ------------------------------------------------------------------ */
/* Input Functions                                                     */
/* ------------------------------------------------------------------ */
static void cli_clear_input_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

static int cli_get_move(GameState *game) {
    int position;
    while (1) {
        printf("Enter position (0-%d): ", BOARD_SIZE - 1);
        if (scanf("%d", &position) != 1) {
            cli_clear_input_buffer();
            printf("Invalid input. Please enter a number between 0 and %d.\n", BOARD_SIZE - 1);
            continue;
        }

        if (game_is_valid_move(game, position)) {
            return position;
        } else {
            printf("Invalid move! Position %d is already occupied or out of range.\n", position);
        }
    }
}

static int cli_get_yes_no(const char *prompt) {
    char choice;
    while (1) {
        printf("%s (y/n): ", prompt);
        if (scanf(" %c", &choice) != 1) {
            cli_clear_input_buffer();
            continue;
        }

        choice = tolower(choice);
        if (choice == 'y') {
            return 1;
        }
        if (choice == 'n') {
            return 0;
        }
        printf("Please enter y or n.\n");
        cli_clear_input_buffer();
    }
}

static CellValue cli_get_symbol_choice(const char *prompt) {
    char choice;
    while (1) {
        printf("%s (X or O): ", prompt);
        if (scanf(" %c", &choice) != 1) {
            cli_clear_input_buffer();
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
            cli_clear_input_buffer();
        }
    }
}

static Difficulty cli_get_difficulty(void) {
    char choice;
    while (1) {
        printf("Choose difficulty - (e)asy or (h)ard: ");
        if (scanf(" %c", &choice) != 1) {
            cli_clear_input_buffer();
            continue;
        }

        choice = tolower(choice);
        if (choice == 'e') {
            return DIFFICULTY_EASY;
        }
        if (choice == 'h') {
            return DIFFICULTY_HARD;
        }
        printf("Please enter e or h.\n");
        cli_clear_input_buffer();
    }
}


/* ------------------------------------------------------------------ */
/* Interface Instance                                                  */
/* ------------------------------------------------------------------ */
static UserInterface cli_interface = {
    .display_board     = cli_display_board,
    .display_status    = cli_display_status,
    .display_message   = cli_display_message,
    .display_formatted = cli_display_formatted,
    .get_move          = cli_get_move,
    .get_yes_no        = cli_get_yes_no,
    .get_symbol_choice = cli_get_symbol_choice,
    .get_difficulty    = cli_get_difficulty,
    .display_error     = cli_display_error
};

UserInterface *ui_get_cli_interface(void) {
    return &cli_interface;
}

