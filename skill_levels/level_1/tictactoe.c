/*
 * Tic-Tac-Toe - Naive Implementation (v1)
 *
 * This version represents how a new C programmer might write this game.
 * It works, but has many issues that will be addressed in later versions:
 *
 * Code Smells Present:
 * - Everything in one file (no separation of concerns)
 * - Magic numbers instead of constants/enums
 * - Global variables for state
 * - Long functions with multiple responsibilities
 * - Minimal error handling
 * - No input validation
 * - Duplicated code
 * - No tests (hard to test this structure)
 * - Poor naming conventions
 * - No documentation
 * - Mixed I/O with logic
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Global state - hard to test, maintain, or reason about */
char board[9] = {'-', '-', '-', '-', '-', '-', '-', '-', '-'};
char currentPlayer = 'X';
int gameOver = 0;
int winner = 0;  // 0 = no winner, 1 = X wins, 2 = O wins, 3 = draw

/* Print the board */
void printBoard() {
    printf("\n");
    printf(" %c | %c | %c \n", board[0], board[1], board[2]);
    printf("---+---+---\n");
    printf(" %c | %c | %c \n", board[3], board[4], board[5]);
    printf("---+---+---\n");
    printf(" %c | %c | %c \n", board[6], board[7], board[8]);
    printf("\n");
}

/* Check for winner - lots of repeated logic */
int checkWinner() {
    // Check rows
    if (board[0] != '-' && board[0] == board[1] && board[1] == board[2]) {
        if (board[0] == 'X') return 1;
        else return 2;
    }
    if (board[3] != '-' && board[3] == board[4] && board[4] == board[5]) {
        if (board[3] == 'X') return 1;
        else return 2;
    }
    if (board[6] != '-' && board[6] == board[7] && board[7] == board[8]) {
        if (board[6] == 'X') return 1;
        else return 2;
    }

    // Check columns
    if (board[0] != '-' && board[0] == board[3] && board[3] == board[6]) {
        if (board[0] == 'X') return 1;
        else return 2;
    }
    if (board[1] != '-' && board[1] == board[4] && board[4] == board[7]) {
        if (board[1] == 'X') return 1;
        else return 2;
    }
    if (board[2] != '-' && board[2] == board[5] && board[5] == board[8]) {
        if (board[2] == 'X') return 1;
        else return 2;
    }

    // Check diagonals
    if (board[0] != '-' && board[0] == board[4] && board[4] == board[8]) {
        if (board[0] == 'X') return 1;
        else return 2;
    }
    if (board[2] != '-' && board[2] == board[4] && board[4] == board[6]) {
        if (board[2] == 'X') return 1;
        else return 2;
    }

    // Check for draw
    int i;
    int emptyCount = 0;
    for (i = 0; i < 9; i++) {
        if (board[i] == '-') emptyCount++;
    }
    if (emptyCount == 0) return 3;  // Draw

    return 0;  // Game ongoing
}

/* Get computer move - simple random (no AI) */
int getComputerMove() {
    int move;
    int attempts = 0;

    /* Keep trying random positions until we find an empty one */
    /* Note: This could potentially loop forever if board is full,
       but we check for game over before calling this */
    while (1) {
        move = rand() % 9;
        if (board[move] == '-') {
            return move;
        }
        attempts++;
        if (attempts > 100) {
            /* Safety valve - should never hit this */
            printf("Error: couldn't find valid move!\n");
            return -1;
        }
    }
}

/* Get human move - minimal validation */
int getHumanMove() {
    int move;
    printf("Enter position (0-8): ");
    scanf("%d", &move);

    /* Basic validation - but could crash if input isn't a number */
    if (move < 0 || move > 8) {
        printf("Invalid position!\n");
        return getHumanMove();  // Recursive - could overflow stack
    }
    if (board[move] != '-') {
        printf("Position already taken!\n");
        return getHumanMove();  // Recursive again
    }
    return move;
}

/* Make a move */
void makeMove(int position, char player) {
    board[position] = player;
}

/* Switch players */
void switchPlayer() {
    if (currentPlayer == 'X') {
        currentPlayer = 'O';
    } else {
        currentPlayer = 'X';
    }
}

/* Reset the board for a new game */
void resetBoard() {
    int i;
    for (i = 0; i < 9; i++) {
        board[i] = '-';
    }
    currentPlayer = 'X';
    gameOver = 0;
    winner = 0;
}

/* Main game loop - does too many things */
int main() {
    char humanSymbol;
    char playAgain;
    int isHumanX;
    int move;

    srand(time(NULL));

    printf("Welcome to Tic-Tac-Toe!\n");
    printf("========================\n\n");

    /* Get player choice - minimal validation */
    printf("Do you want to be X or O? ");
    scanf(" %c", &humanSymbol);

    if (humanSymbol == 'x') humanSymbol = 'X';
    if (humanSymbol == 'o') humanSymbol = 'O';

    if (humanSymbol != 'X' && humanSymbol != 'O') {
        printf("Invalid choice, defaulting to X\n");
        humanSymbol = 'X';
    }

    isHumanX = (humanSymbol == 'X');
    printf("You are %c, computer is %c\n\n",
           humanSymbol, (humanSymbol == 'X') ? 'O' : 'X');

    /* Game loop */
    while (1) {
        while (!gameOver) {
            printBoard();

            /* Determine whose turn */
            if ((currentPlayer == 'X' && isHumanX) ||
                (currentPlayer == 'O' && !isHumanX)) {
                /* Human turn */
                printf("Your turn (%c)\n", currentPlayer);
                move = getHumanMove();
            } else {
                /* Computer turn */
                printf("Computer's turn (%c)\n", currentPlayer);
                move = getComputerMove();
                if (move == -1) {
                    printf("Computer error!\n");
                    return 1;
                }
                printf("Computer plays position %d\n", move);
            }

            makeMove(move, currentPlayer);

            /* Check for game end */
            winner = checkWinner();
            if (winner != 0) {
                gameOver = 1;
            } else {
                switchPlayer();
            }
        }

        /* Game over */
        printBoard();
        if (winner == 1) {
            printf("X wins!\n");
            if (isHumanX) printf("Congratulations!\n");
            else printf("Computer wins!\n");
        } else if (winner == 2) {
            printf("O wins!\n");
            if (!isHumanX) printf("Congratulations!\n");
            else printf("Computer wins!\n");
        } else {
            printf("It's a draw!\n");
        }

        /* Play again? */
        printf("\nPlay again? (y/n): ");
        scanf(" %c", &playAgain);

        if (playAgain == 'y' || playAgain == 'Y') {
            resetBoard();
            /* Note: doesn't let winner choose symbol like requirements say */
        } else {
            break;
        }
    }

    printf("\nThanks for playing!\n");
    return 0;
}

