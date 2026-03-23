#ifndef DISPLAY_H
#define DISPLAY_H

#include "tictactoe.h"


int newline();

void display_board(const GameState *state);

char cell_value_to_marker(const GameState *state, int pos);

void display_status(GameStatus status);

#endif // DISPLAY_H
