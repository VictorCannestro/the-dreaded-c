#ifndef UI_INTERFACE_H
#define UI_INTERFACE_H

#include "constants.h"
#include "tictactoe.h"

/*
 * User Interface - Abstract interface for user interaction
 *
 * This allows swapping between CLI, GUI, or other implementations
 * without changing game logic.
 */
typedef struct UserInterface {
    /* Display the current game board */
    void (*display_board)(const GameState *state);

    /* Display the game status (win/draw/ongoing) */
    void (*display_status)(GameStatus status);

    /* Display a general message to the user */
    void (*display_message)(const char *message);

    /* Display a formatted message (printf-style) */
    void (*display_formatted)(const char *format, ...);

    /* Get a move from the human player (returns position 0-8) */
    int (*get_move)(GameState *game);

    /* Get a yes/no choice from the user (returns 1 for yes, 0 for no) */
    int (*get_yes_no)(const char *prompt);

    /* Get symbol choice (X or O) from the user */
    CellValue (*get_symbol_choice)(const char *prompt);

    /* Get difficulty choice from the user */
    Difficulty (*get_difficulty)(void);

    /* Display an error message */
    void (*display_error)(const char *message);
} UserInterface;

/*
 * Get the CLI implementation of the User Interface
 */
UserInterface *ui_get_cli_interface(void);

#endif /* UI_INTERFACE_H */
