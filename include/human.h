#ifndef HUMAN_H
#define HUMAN_H

#include "tictactoe.h"

/*
 * Get the human player's symbol choice for the first game
 * Prompts user to choose between X and O
 * Returns CELL_X or CELL_O
 */
CellValue human_get_symbol_choice(void);

/*
 * Get the winner's symbol choice for the next game
 * Prompts the winning player to choose their symbol
 * Returns CELL_X or CELL_O
 */
CellValue human_get_winner_symbol_choice(CellValue last_winner);

/*
 * Get the human player's move
 * Validates the move against the game state
 * Returns position 0-8
 */
int human_get_move(GameState *game);

#endif // HUMAN_H
