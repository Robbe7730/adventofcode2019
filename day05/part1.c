#include "../helpers/input_helper.h"
#include "../helpers/arraylist.h"

#include <stdlib.h>

int get_input() {
    return 1;
}

int get_value(arraylist* memory, int value, int mode) {
    int new_value;
    switch (mode) {
    case 0:
        arraylist_get(memory, value, &new_value);
        return new_value;
    case 1:
        return value;
    default:
        fprintf(stderr, "Invalid mode %d", mode);
        exit(EXIT_FAILURE);
    }
}

int main(int argv, char** argc) {
    if (argv != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argc[0]);
    }

    FILE* f = open_file(argc[1]);

    arraylist* memory = arraylist_new(sizeof(int));
    int num;
    bool succesful = true;
    while (succesful) {
        num = read_int(f, ',', &succesful);
        arraylist_add(memory, &num);
    }

    int ip = 0;

    while (1 == 1) {
        int full_opcode = 0;
        arraylist_get(memory, ip, &full_opcode);

        int first_position = 0;
        int second_position = 0;

        int first_value = 0;
        int second_value = 0;

        int position = 0;
        int value = 0;

        int opcode = full_opcode % 100;
        int mode_a = (full_opcode / 100) % 10;
        int mode_b = (full_opcode / 1000) % 10;
        int mode_c = (full_opcode / 10000) % 10;

        printf("opcode %d = %d modes a:%d b:%d c:%d \n", ip, opcode, mode_a, mode_b, mode_c);

        switch (opcode % 100) {
        case 1:
            puts("+");

            arraylist_get(memory, ip + 1, &first_position);
            arraylist_get(memory, ip + 2, &second_position);
            arraylist_get(memory, ip + 3, &position);

            first_value = get_value(memory, first_position, mode_a);
            second_value = get_value(memory, second_position, mode_b);

            int sum = first_value + second_value;

            printf("Setting %d to %d\n", position, sum);
            arraylist_set(memory, position, &sum);

            ip += 4;
            break;
        case 2:
            puts("*");

            arraylist_get(memory, ip + 1, &first_position);
            arraylist_get(memory, ip + 2, &second_position);
            arraylist_get(memory, ip + 3, &position);

            first_value = get_value(memory, first_position, mode_a);
            second_value = get_value(memory, second_position, mode_b);

            int product = first_value * second_value;

            printf("Setting %d to %d\n", position, product);
            arraylist_set(memory, position, &product);

            ip += 4;
            break;
        case 3:
            puts("input");

            arraylist_get(memory, ip + 1, &position);

            get_value(memory, position, mode_a);

            int input = get_input();

            arraylist_set(memory, position, &input);

            ip += 2;
            break;
        case 4:
            puts("print");

            arraylist_get(memory, ip + 1, &position);

            arraylist_get(memory, position, &value);

            printf("OUT: %d\n", value);

            ip += 2;
            break;
        case 99:
            puts("HALT");
            return 0;

        default:
            fprintf(stderr, "Unknown opcode %d\n", opcode);
            return 1;
        }
    }
}
