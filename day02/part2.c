#include "../helpers/input_helper.h"
#include "../helpers/arraylist.h"

#include <stdlib.h>

int find_solution(arraylist* memory, int expected, int noun, int verb);

int main(int argv, char** argc) {
    if (argv != 2) {
        // fprintf(stderr, "Usage: %s <filename>\n", argc[0]);
    }

    FILE* f = open_file(argc[1]);

    bool ignored;

    arraylist* memory = arraylist_new(sizeof(int));
    int num;
    while ((num = read_int(f, ',', &ignored)) != -1) {
        arraylist_add(memory, &num);
    }

    for (int noun = 0; noun < 100; noun++) {
        for (int verb = 0; verb < 100; verb++) {
            int a = find_solution(arraylist_copy(memory), 19690720, noun, verb);
            if (a != 0) {
                printf("GOTTIM %d\n", a);
                exit(EXIT_SUCCESS);
            } else {
                printf("NOT IT\n");
            }
        }
    }
}

int find_solution(arraylist* memory, int expected, int noun, int verb) {
    int ip = 0;

    arraylist_set(memory, 1, &noun);
    arraylist_set(memory, 2, &verb);

    while (1 == 1) {
        int opcode = 0;
        arraylist_get(memory, ip, &opcode);
        // printf("opcode %d = %d\n", ip, opcode);
        int first_position = 0;
        int second_position = 0;
        int first_value = 0;
        int second_value = 0;
        int position = 0;
        int result;
        switch (opcode) {
        case 1:
            //puts("+");

            arraylist_get(memory, ip + 1, &first_position);
            arraylist_get(memory, ip + 2, &second_position);
            arraylist_get(memory, ip + 3, &position);

            arraylist_get(memory, first_position, &first_value);
            arraylist_get(memory, second_position, &second_value);

            int sum = first_value + second_value;

            // printf("Setting %d to %d\n", position, sum);
            arraylist_set(memory, position, &sum);
            break;
        case 2:
            //puts("*");

            arraylist_get(memory, ip + 1, &first_position);
            arraylist_get(memory, ip + 2, &second_position);
            arraylist_get(memory, ip + 3, &position);

            arraylist_get(memory, first_position, &first_value);
            arraylist_get(memory, second_position, &second_value);

            int product = first_value * second_value;

            // printf("Setting %d to %d\n", position, product);
            arraylist_set(memory, position, &product);
            break;
        case 99:
            //puts("HALT");
            result = 0;
            arraylist_get(memory, 0, &result);
            // printf("result = %d\n", result);
            if (result == expected) {
                return 100*noun + verb;
            }
            return 0;

        default:
            // fprintf(stderr, "Unknown opcode %d\n", opcode);
            return 0;
        }
        ip += 4;
    }
}