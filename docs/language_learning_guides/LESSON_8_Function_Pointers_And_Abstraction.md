# Lesson 8: Function Pointers and Abstraction
Function pointers allow you to store and call functions dynamically. This enables powerful patterns like callbacks, 
plugin systems, and **interface abstraction**.

## Basic Function Pointer Syntax
```c
// A regular function
int add(int a, int b) {
    return a + b;
}

// A function pointer variable
int (*operation)(int, int);   // Points to a function taking 2 ints, returning int

// Assign and call
operation = add;              // Point to the add function
int result = operation(3, 4); // Call through pointer: result = 7
```

## Why Function Pointers Matter
They let you **swap behavior without changing code**. This is crucial for:
- Swapping CLI for GUI
- Testing with mock functions
- Plugin architectures
- Strategy patterns

## The UserInterface Pattern in This Project
We use function pointers to abstract the UI layer:

**Step 1: Define the interface (what functions must exist)**
```c
// ui_interface.h
typedef struct UserInterface {
    void (*display_board)(const GameState *state);
    void (*display_status)(GameStatus status);
    void (*display_message)(const char *message);
    int  (*get_move)(GameState *game);
    int  (*get_yes_no)(const char *prompt);
    CellValue (*get_symbol_choice)(const char *prompt);
    Difficulty (*get_difficulty)(void);
} UserInterface;

// Factory function declaration
UserInterface *ui_get_cli_interface(void);
```

**Step 2: Implement the interface (CLI version)**
```c
// ui_cli.c
static void cli_display_board(const GameState *state) {
    // printf the board to terminal...
}

static int cli_get_move(GameState *game) {
    // scanf from user...
}

// Create the interface struct with function pointers
static UserInterface cli_interface = {
    .display_board = cli_display_board,
    .display_status = cli_display_status,
    .display_message = cli_display_message,
    .get_move = cli_get_move,
    .get_yes_no = cli_get_yes_no,
    .get_symbol_choice = cli_get_symbol_choice,
    .get_difficulty = cli_get_difficulty,
};

UserInterface *ui_get_cli_interface(void) {
    return &cli_interface;
}
```

**Step 3: Use the interface (game logic doesn't know about CLI)**
```c
// main.c
int main(void) {
    GameState game;
    UserInterface *ui = ui_get_cli_interface();  // Could be GUI!
    
    ui->display_message("Welcome to Tic-Tac-Toe!");
    
    while (!game_is_over(&game)) {
        ui->display_board(&game);
        int move = ui->get_move(&game);  // Works for CLI or GUI
        game_make_move(&game, move);
    }
    
    ui->display_status(game.status);
    return 0;
}
```

## Adding a GUI Implementation
To add a GUI, you just implement the same interface:

```c
// ui_gui.c (example with SDL2)
static void gui_display_board(const GameState *state) {
    // SDL_RenderDrawLine, SDL_RenderCopy, etc.
}

static int gui_get_move(GameState *game) {
    // SDL_PollEvent for mouse clicks
}

static UserInterface gui_interface = {
    .display_board = gui_display_board,
    .get_move = gui_get_move,
    // ... same fields, different implementations
};

UserInterface *ui_get_gui_interface(void) {
    return &gui_interface;
}
```

**To switch from CLI to GUI:** Change ONE line in main.c:
```c
// UserInterface *ui = ui_get_cli_interface();
UserInterface *ui = ui_get_gui_interface();  // That's it!
```

## Benefits of This Pattern

| Without Abstraction              | With UserInterface                      |
|----------------------------------|-----------------------------------------|
| `printf` scattered throughout    | All I/O in one place                    |
| Changing UI = rewrite game logic | Changing UI = new implementation file   |
| Can't test without terminal      | Can mock the interface for tests        |
| Hard to add web/mobile UI        | Each platform implements same interface |

## Algorithm Deep Dive: `cli_display_board`

For a detailed breakdown of how `cli_display_board` renders the game board dynamically for any board size, see:

📄 **[CLI Display Board Algorithm](../algorithms/CLI_DISPLAY_BOARD.md)**

Topics covered:
- Visual output examples (3x3 and 4x4 boards)
- String constants for board display (`BOARD_CELL_PADDING`, `BOARD_CELL_SEPARATOR`, etc.)
- Step-by-step code breakdown
- 2D to 1D index conversion formula
- Key design decisions and rationale
- Complexity analysis
- Extension ideas (colors, Unicode, labels)

See [include/ui_interface.h](../../include/ui_interface.h) and [src/ui_cli.c](../../src/ui_cli.c).

---

## Next Steps

Continue to [Lesson 9: Memory Management](LESSON_9_Memory_Management.md) →

← Previous: [Lesson 7: Header Files](LESSON_7_Header_Files.md)

← Back to [Learning Guide Index](LEARNING_GUIDE.md)

