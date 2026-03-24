#include "utils.h"
#include <stdio.h>


int newline() {
    return printf("\n");
}

/*
 * Clear any remaining input from the input buffer
 */
void clear_input_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}