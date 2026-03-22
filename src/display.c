#include "display.h"
#include <stdio.h>

void display_board(const GameState *state) {
    if (state == NULL) return;

    printf("\n");
    printf("   0 | 1 | 2\n");
    printf("  -----------\n");

    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            int pos = row * 3 + col;
            CellValue cell = state->board[pos];

            if (cell == CELL_EMPTY) {
                printf("   %d", pos);
            } else if (cell == CELL_X) {
                printf("   X");
            } else {
                printf("   O");
            }

            if (col < 2) printf(" |");
        }
        printf("\n");
        if (row < 2) printf("  -----------\n");
    }
    printf("\n");
}

void display_status(GameStatus status) {
    printf("\n");
    switch (status) {
        case GAME_X_WINS:
            printf("*** Player X WINS! ***\n");
            break;
        case GAME_O_WINS:
            printf("*** Player O WINS! ***\n");
            break;
        case GAME_DRAW:
            printf("*** GAME DRAW ***\n");
            break;
        case GAME_ONGOING:
            printf("Game is ongoing...\n");
            break;
    }
    printf("\n");
}
