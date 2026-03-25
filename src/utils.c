#include "utils.h"
#include "constants.h"
#include <stdio.h>


int newline(void) {
    return printf("\n");
}

/*
 * Clear any remaining input from the input buffer
 */
void clear_input_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

char cell_value_to_marker(CellValue cell) {
    switch (cell) {
        case CELL_EMPTY:
            return EMPTY_MARKER;
        case CELL_X:
            return X_MARKER;
        case CELL_O:
            return O_MARKER;
        default:
            return EMPTY_MARKER;
    }
}

