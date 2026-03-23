#include "display.h"
#include <stdio.h>


int newline() {
    return printf("\n");
}

void display_board(const GameState *state) {
    if (state == NULL) {
        return;
    }
    char board_template[] = "   |   |   \n %c | %c | %c \n___|___|___\n   |   |   \n %c | %c | %c \n___|___|___\n   |   |   \n %c | %c | %c \n   |   |   ";
    newline();
    printf(
        board_template, 
        cell_value_to_marker(state, 0), cell_value_to_marker(state, 1), cell_value_to_marker(state, 2),
        cell_value_to_marker(state, 3), cell_value_to_marker(state, 4), cell_value_to_marker(state, 5),
        cell_value_to_marker(state, 6), cell_value_to_marker(state, 7), cell_value_to_marker(state, 8)
    );
    newline();
}

char cell_value_to_marker(const GameState *state, int pos) {
    CellValue cell = state->board[pos];
    switch (cell) {
        case CELL_EMPTY:
            return '-';
        case CELL_X:
            return 'X';
        case CELL_O:
            return 'O';
        default:
            return '-';
    }
}

void display_status(GameStatus status) {
    newline();
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
    newline();
}
