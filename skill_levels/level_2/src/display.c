/*
 * Display Module - Implementation
 *
 * v2 Improvement: All display logic isolated.
 * Makes it easy to change output format later.
 */

#include "display.h"
#include <stdio.h>

char cell_to_char(CellValue cell) {
    switch (cell) {
        case CELL_X:     return MARKER_X;
        case CELL_O:     return MARKER_O;
        case CELL_EMPTY: return MARKER_EMPTY;
        default:         return '?';
    }
}

void display_board(const GameState *state) {
    printf("\n");
    printf(" %c | %c | %c \n",
           cell_to_char(state->board[0]),
           cell_to_char(state->board[1]),
           cell_to_char(state->board[2]));
    printf("---+---+---\n");
    printf(" %c | %c | %c \n",
           cell_to_char(state->board[3]),
           cell_to_char(state->board[4]),
           cell_to_char(state->board[5]));
    printf("---+---+---\n");
    printf(" %c | %c | %c \n",
           cell_to_char(state->board[6]),
           cell_to_char(state->board[7]),
           cell_to_char(state->board[8]));
    printf("\n");
}

void display_result(GameStatus status) {
    printf("\n");
    switch (status) {
        case STATUS_X_WINS:
            printf("*** X WINS! ***\n");
            break;
        case STATUS_O_WINS:
            printf("*** O WINS! ***\n");
            break;
        case STATUS_DRAW:
            printf("*** DRAW! ***\n");
            break;
        default:
            printf("Game in progress...\n");
            break;
    }
    printf("\n");
}

