/*
 * Display Module - Header
 *
 * v2 Improvement: All output in one place.
 * Separates "what to display" from "game logic".
 */

#ifndef DISPLAY_H
#define DISPLAY_H

#include "types.h"

/* Display the current board state */
void display_board(const GameState *state);

/* Display game result */
void display_result(GameStatus status);

/* Convert cell value to display character */
char cell_to_char(CellValue cell);

#endif /* DISPLAY_H */

