#ifndef CONSTANTS_H
#define CONSTANTS_H


extern const char X_MARKER;
extern const char O_MARKER;
extern const char EMPTY_MARKER;

typedef enum {
    CELL_EMPTY = 0,
    CELL_X = 1,
    CELL_O = 2
} CellValue;

typedef enum {
    ONGOING = 0,
    X_WINS = 1,
    O_WINS = 2,
    DRAW = 3
} GameStatus;

typedef enum {
    PLAYER_HUMAN = 0,
    PLAYER_COMPUTER = 1
} PlayerType;

#endif // CONSTANTS_H
