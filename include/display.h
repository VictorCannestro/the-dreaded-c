#ifndef DISPLAY_H
#define DISPLAY_H

#include "tictactoe.h"

/*
 * Display the board in a user-friendly format
 */
void display_board(const GameState *state);

int newline();

/*
 * Display the game status (win, draw, ongoing)
 */
void display_status(GameStatus status);

#endif // DISPLAY_H
