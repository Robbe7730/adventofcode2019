#include "../helpers/input_helper.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#define IMAGE_WIDTH 25
#define IMAGE_HEIGHT 6

bool process_layer(FILE* f, int* min_zero_hold, int* num_one_hold, int* num_two_hold) {
    char curr_char;
    int num_zero = 0;
    int num_one = 0;
    int num_two = 0;
    for (int i = 0; i < IMAGE_HEIGHT; i++) {
        for (int j = 0; j < IMAGE_WIDTH; j++) {
            curr_char = getc(f);
            if (curr_char == EOF) {
                return false;
            }
            if (curr_char == '0') {
                num_zero++;
            } else if (curr_char == '1') {
                num_one++;
            } else if (curr_char == '2') {
                num_two++;
            }
        }
    }
    if (num_zero < *min_zero_hold) {
        *min_zero_hold = num_zero;
        *num_one_hold = num_one;
        *num_two_hold = num_two;
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

    while (process_layer(f, &min_zero, &num_one, &num_two)) {}

    printf("result = 1:%d * 2:%d = %d\n", num_one, num_two, num_one*num_two);
    
}