#include "tictactoe.h"
#include "ui_interface.h"
#include "constants.h"
#include "utils.h"

#include <stdio.h>


static void play_game(GameState *game, UserInterface *ui) {
    while (!game_is_over(game)) {
        ui->display_board(game);

        CellValue current_player = game_get_current_player(game);
        PlayerType player_type = game_get_player_type(game, current_player);
        char player_symbol = cell_value_to_marker(current_player);

        if (player_type == PLAYER_HUMAN) {
            ui->display_formatted("Player %c's turn (Human)\n", player_symbol);
            int move = ui->get_move(game);
            game_make_move(game, move);
        } else {
            ui->display_formatted("Player %c's turn (Computer)\n", player_symbol);
            int move = game_get_computer_move(game);
            ui->display_formatted("Computer plays position %d\n", move);
            game_make_move(game, move);
        }
    }

    ui->display_board(game);
    ui->display_status(game->status);
}

int main(void) {
    GameState game;
    UserInterface *ui = ui_get_cli_interface();

    ui->display_message("Welcome to Tic-Tac-Toe!");
    ui->display_message("========================");
    ui->display_message("You will play against the computer.\n");

    game_init_session(&game);

    Difficulty difficulty = ui->get_difficulty();
    game_set_difficulty(&game, difficulty);
    ui->display_formatted(
        "Difficulty Level: %s\n\n",
        (difficulty == DIFFICULTY_EASY) ? "Easy" : "Hard"
    );

    CellValue human_symbol = ui->get_symbol_choice("Choose your symbol");
    game_set_human_symbol_choice(&game, human_symbol);

    char human_marker = cell_value_to_marker(human_symbol);
    char computer_marker = (human_symbol == CELL_X) ? O_MARKER : X_MARKER;
    ui->display_formatted(
        "You chose %c. Computer will play %c.\n\n",
        human_marker,
        computer_marker
    );

    do {
        play_game(&game, ui);

        if (!ui->get_yes_no("Play another game?")) {
            break;
        }

        if (game.last_winner != CELL_EMPTY) {
            char winner_marker = cell_value_to_marker(game.last_winner);
            char prompt[64];
            snprintf(
                prompt,
                sizeof(prompt),
                "Player %c (last winner), choose your symbol for the next game",
                winner_marker
            );

            CellValue winner_choice = ui->get_symbol_choice(prompt);
            game_set_winner_symbol_choice(&game, winner_choice);

            char next_human = cell_value_to_marker(winner_choice);
            char next_computer = (winner_choice == CELL_X) ? O_MARKER : X_MARKER;
            ui->display_formatted(
                "Next game: You play %c, Computer plays %c\n\n",
                next_human,
                next_computer
            );
        } else {
            ui->display_message("Draw! Keeping same symbols for next game.\n");
        }

        game_new_game(&game);

    } while (1);

    ui->display_message("\nThanks for playing Tic-Tac-Toe!");
    return 0;
}
