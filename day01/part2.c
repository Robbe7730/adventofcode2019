#include "../helpers/input_helper.h"
#include <stdlib.h>

int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    FILE* f = open_file(argv[1]);
    
    bool ignored;

    int num;
    int sum = 0;

    while ((num = read_int(f, '\n', &ignored)) != READ_INT_NULL) {
        printf("got: %d\n", num);
        while ((num = (num / 3) - 2) > 0) {
            sum += num;
        }
    }

    printf("Result: %d\n", sum);

    return 0;
}
