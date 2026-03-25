/*
 * ui_gui_example.c - Example GUI Implementation (Not Compiled)
 *
 * This file demonstrates how to implement a GUI frontend using the
 * UIInterface abstraction. It uses SDL2 as an example, but the same
 * pattern works with any GUI framework (GTK, Qt, ncurses, etc.)
 *
 * To use this:
 * 1. Install SDL2: brew install sdl2 sdl2_ttf
 * 2. Add this file to your build
 * 3. Link with -lSDL2 -lSDL2_ttf
 * 4. Call ui_get_gui_interface() instead of ui_get_cli_interface()
 */

#ifdef USE_GUI  /* Only compile when GUI is enabled */

#include "ui_interface.h"
#include "tictactoe.h"
#include "utils.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>

/* ------------------------------------------------------------------ */
/* GUI State                                                           */
/* ------------------------------------------------------------------ */

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static TTF_Font *font = NULL;
static int pending_move = -1;
static int waiting_for_input = 0;

#define WINDOW_SIZE 600
#define CELL_SIZE (WINDOW_SIZE / 3)

/* ------------------------------------------------------------------ */
/* GUI Initialization                                                  */
/* ------------------------------------------------------------------ */

static int gui_init(void) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL Init failed: %s\n", SDL_GetError());
        return -1;
    }

    if (TTF_Init() < 0) {
        fprintf(stderr, "TTF Init failed: %s\n", TTF_GetError());
        return -1;
    }

    window = SDL_CreateWindow(
        "Tic-Tac-Toe",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_SIZE, WINDOW_SIZE,
        SDL_WINDOW_SHOWN
    );

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    font = TTF_OpenFont("/System/Library/Fonts/Helvetica.ttc", 120);

    return 0;
}

/* ------------------------------------------------------------------ */
/* Display Functions                                                   */
/* ------------------------------------------------------------------ */

static void gui_display_board(const GameState *state) {
    if (state == NULL || renderer == NULL) return;

    /* Clear screen - white background */
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    /* Draw grid lines - black */
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    /* Vertical lines */
    SDL_RenderDrawLine(renderer, CELL_SIZE, 0, CELL_SIZE, WINDOW_SIZE);
    SDL_RenderDrawLine(renderer, CELL_SIZE * 2, 0, CELL_SIZE * 2, WINDOW_SIZE);

    /* Horizontal lines */
    SDL_RenderDrawLine(renderer, 0, CELL_SIZE, WINDOW_SIZE, CELL_SIZE);
    SDL_RenderDrawLine(renderer, 0, CELL_SIZE * 2, WINDOW_SIZE, CELL_SIZE * 2);

    /* Draw X's and O's */
    SDL_Color textColor = {0, 0, 0, 255};

    for (int i = 0; i < 9; i++) {
        char marker = cell_value_to_marker(state->board[i]);
        if (marker == ' ') continue;

        char text[2] = {marker, '\0'};
        SDL_Surface *surface = TTF_RenderText_Solid(font, text, textColor);
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

        int row = i / 3;
        int col = i % 3;

        SDL_Rect dstRect = {
            col * CELL_SIZE + (CELL_SIZE - surface->w) / 2,
            row * CELL_SIZE + (CELL_SIZE - surface->h) / 2,
            surface->w,
            surface->h
        };

        SDL_RenderCopy(renderer, texture, NULL, &dstRect);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }

    SDL_RenderPresent(renderer);
}

static void gui_display_status(GameStatus status) {
    /* Could show a dialog or overlay with the game result */
    const char *message = NULL;
    switch (status) {
        case X_WINS: message = "Player X Wins!"; break;
        case O_WINS: message = "Player O Wins!"; break;
        case DRAW:   message = "It's a Draw!"; break;
        default: return;
    }

    /* Simple approach: show message box */
    SDL_ShowSimpleMessageBox(
        SDL_MESSAGEBOX_INFORMATION,
        "Game Over",
        message,
        window
    );
}

static void gui_display_message(const char *message) {
    /* Could render to a status bar area or show briefly */
    printf("[GUI] %s\n", message);  /* Fallback to console */
}

static void gui_display_formatted(const char *format, ...) {
    /* Similar to display_message - render or log */
    va_list args;
    va_start(args, format);
    printf("[GUI] ");
    vprintf(format, args);
    va_end(args);
}

static void gui_display_error(const char *message) {
    SDL_ShowSimpleMessageBox(
        SDL_MESSAGEBOX_ERROR,
        "Error",
        message,
        window
    );
}

/* ------------------------------------------------------------------ */
/* Input Functions                                                     */
/* ------------------------------------------------------------------ */

static int gui_get_move(GameState *game) {
    pending_move = -1;
    waiting_for_input = 1;

    while (waiting_for_input) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                exit(0);
            }

            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x = event.button.x;
                int y = event.button.y;

                int col = x / CELL_SIZE;
                int row = y / CELL_SIZE;
                int position = row * 3 + col;

                if (game_is_valid_move(game, position)) {
                    pending_move = position;
                    waiting_for_input = 0;
                }
            }
        }
        SDL_Delay(16);  /* ~60 FPS */
    }

    return pending_move;
}

static int gui_get_yes_no(const char *prompt) {
    const SDL_MessageBoxButtonData buttons[] = {
        { SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 0, "No" },
        { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "Yes" },
    };

    const SDL_MessageBoxData messageboxdata = {
        SDL_MESSAGEBOX_INFORMATION,
        window,
        "Tic-Tac-Toe",
        prompt,
        SDL_arraysize(buttons),
        buttons,
        NULL
    };

    int buttonid;
    SDL_ShowMessageBox(&messageboxdata, &buttonid);
    return buttonid;
}

static CellValue gui_get_symbol_choice(const char *prompt) {
    const SDL_MessageBoxButtonData buttons[] = {
        { 0, CELL_O, "O" },
        { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, CELL_X, "X" },
    };

    const SDL_MessageBoxData messageboxdata = {
        SDL_MESSAGEBOX_INFORMATION,
        window,
        prompt,
        "Choose your symbol:",
        SDL_arraysize(buttons),
        buttons,
        NULL
    };

    int buttonid;
    SDL_ShowMessageBox(&messageboxdata, &buttonid);
    return (CellValue)buttonid;
}

static Difficulty gui_get_difficulty(void) {
    const SDL_MessageBoxButtonData buttons[] = {
        { 0, DIFFICULTY_EASY, "Easy" },
        { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, DIFFICULTY_HARD, "Hard" },
    };

    const SDL_MessageBoxData messageboxdata = {
        SDL_MESSAGEBOX_INFORMATION,
        window,
        "Difficulty",
        "Choose difficulty level:",
        SDL_arraysize(buttons),
        buttons,
        NULL
    };

    int buttonid;
    SDL_ShowMessageBox(&messageboxdata, &buttonid);
    return (Difficulty)buttonid;
}

/* ------------------------------------------------------------------ */
/* Interface Instance                                                  */
/* ------------------------------------------------------------------ */

static UserInterface gui_interface = {
    .display_board     = gui_display_board,
    .display_status    = gui_display_status,
    .display_message   = gui_display_message,
    .display_formatted = gui_display_formatted,
    .get_move          = gui_get_move,
    .get_yes_no        = gui_get_yes_no,
    .get_symbol_choice = gui_get_symbol_choice,
    .get_difficulty    = gui_get_difficulty,
    .display_error     = gui_display_error
};

UserInterface *ui_get_gui_interface(void) {
    if (window == NULL) {
        gui_init();
    }
    return &gui_interface;
}

#endif /* USE_GUI */

