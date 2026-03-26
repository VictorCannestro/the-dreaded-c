#include "ui_interface.h"
#include "tictactoe.h"
#include "constants.h"
#include "utils.h"

#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>


/* ------------------------------------------------------------------ */
/* String Constants                                                    */
/* ------------------------------------------------------------------ */
static const char *MSG_X_WINS           = "*** Player X WINS! ***\n";
static const char *MSG_O_WINS           = "*** Player O WINS! ***\n";
static const char *MSG_DRAW             = "*** GAME DRAW ***\n";
static const char *MSG_ONGOING          = "Game is ongoing...\n";

static const char *PROMPT_POSITION      = "Enter position (0-%d): ";
static const char *PROMPT_YES_NO_CHOICE = " (y/n): ";
static const char *PROMPT_SYMBOL_CHOICE = " (X or O): ";
static const char *PROMPT_DIFFICULTY    = "Choose difficulty - (e)asy or (h)ard: ";

static const char *ERR_PREFIX           = "Error: %s\n";
static const char *ERR_INVALID_NUMBER   = "Invalid input. Please enter a number between 0 and %d.\n";
static const char *ERR_INVALID_MOVE     = "Invalid move! Position %d is already occupied or out of range.\n";
static const char *ERR_INVALID_YES_NO   = "Please enter y or n.\n";
static const char *ERR_INVALID_SYMBOL   = "Invalid choice. Please enter X or O.\n";
static const char *ERR_INVALID_DIFF     = "Please enter e or h.\n";
static const char *ERR_INVALID_INPUT_XO = "Invalid input. Please enter X or O.\n";

static const char *BOARD_CELL_PADDING   = "   ";
static const char *BOARD_CELL_SEPARATOR = "|";
static const char *BOARD_CELL_FORMAT    = " %c ";
static const char *BOARD_ROW_DIVIDER    = "___";


/* ------------------------------------------------------------------ */
/* Display Functions                                                   */
/* ------------------------------------------------------------------ */
static void cli_display_board(const GameState *state) {
    if (state == NULL) {
        return;
    }
    newline();
    
    for (int row = 0; row < BOARD_DIM; row++) {
        /* Top padding row */
        for (int col = 0; col < BOARD_DIM; col++) {
            printf("%s", BOARD_CELL_PADDING);
            if (col < BOARD_DIM - 1) {
                printf("%s", BOARD_CELL_SEPARATOR);
            }
        }
        newline();

        /* Cell values row */
        for (int col = 0; col < BOARD_DIM; col++) {
            int pos = row * BOARD_DIM + col;
            printf(BOARD_CELL_FORMAT, cell_value_to_marker(state->board[pos]));
            if (col < BOARD_DIM - 1) {
                printf("%s", BOARD_CELL_SEPARATOR);
            }
        }
        newline();

        /* Bottom border row (or spacing for last row) */
        for (int col = 0; col < BOARD_DIM; col++) {
            if (row < BOARD_DIM - 1) {
                printf("%s", BOARD_ROW_DIVIDER);
            } else {
                printf("%s", BOARD_CELL_PADDING);
            }
            if (col < BOARD_DIM - 1) {
                printf("%s", BOARD_CELL_SEPARATOR);
            }
        }
        newline();
    }
    newline();
}

static void cli_display_status(GameStatus status) {
    newline();
    switch (status) {
        case X_WINS:
            printf("%s", MSG_X_WINS);
            break;
        case O_WINS:
            printf("%s", MSG_O_WINS);
            break;
        case DRAW:
            printf("%s", MSG_DRAW);
            break;
        case ONGOING:
            printf("%s", MSG_ONGOING);
            break;
    }
    newline();
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
    printf(ERR_PREFIX, message);
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
        printf(PROMPT_POSITION, BOARD_SIZE - 1);
        if (scanf("%d", &position) != 1) {
            cli_clear_input_buffer();
            printf(ERR_INVALID_NUMBER, BOARD_SIZE - 1);
            continue;
        }

        if (game_is_valid_move(game, position)) {
            return position;
        } else {
            printf(ERR_INVALID_MOVE, position);
        }
    }
}

static int cli_get_yes_no(const char *prompt) {
    char choice;
    while (1) {
        printf("%s%s", prompt, PROMPT_YES_NO_CHOICE);
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
        printf("%s", ERR_INVALID_YES_NO);
        cli_clear_input_buffer();
    }
}

static CellValue cli_get_symbol_choice(const char *prompt) {
    char choice;
    while (1) {
        printf("%s%s", prompt, PROMPT_SYMBOL_CHOICE);
        if (scanf(" %c", &choice) != 1) {
            cli_clear_input_buffer();
            printf("%s", ERR_INVALID_INPUT_XO);
            continue;
        }

        choice = toupper(choice);
        if (choice == X_MARKER) {
            return CELL_X;
        } else if (choice == O_MARKER) {
            return CELL_O;
        } else {
            printf("%s", ERR_INVALID_SYMBOL);
            cli_clear_input_buffer();
        }
    }
}

static Difficulty cli_get_difficulty(void) {
    char choice;
    while (1) {
        printf("%s", PROMPT_DIFFICULTY);
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
        printf("%s", ERR_INVALID_DIFF);
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

