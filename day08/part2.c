#include "../helpers/input_helper.h"
#include "../helpers/memory.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#define IMAGE_WIDTH 25
#define IMAGE_HEIGHT 6

bool process_layer(FILE* f, char** drawing) {
    char curr_char;
    
    for (int i = 0; i < IMAGE_HEIGHT * IMAGE_WIDTH; i++) {
        curr_char = getc(f);
        if (curr_char == EOF) {
            return false;
        }
        if (curr_char == '0') {
            if ((*drawing)[i] == 0) {
                (*drawing)[i] = ' ';
            }
        } else if (curr_char == '1') {
            if ((*drawing)[i] == 0) {
                (*drawing)[i] = '#';
            }
        } else if (curr_char == '2') {}
    }

    return true;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    FILE* f = open_file(argv[1]);

    bool running = true;

    int min_zero = INT_MAX;
    int num_one = 0;
    int num_two = 0;

    char* drawing = checked_calloc(IMAGE_HEIGHT * IMAGE_WIDTH,sizeof(int));

    while (process_layer(f, &drawing)) {}

    for (int i = 0; i < IMAGE_HEIGHT; i++) {
        for (int j = 0; j < IMAGE_WIDTH; j++) {
            printf("%c", drawing[i*IMAGE_WIDTH + j]);
        }
        printf("\n");
    }

    
}