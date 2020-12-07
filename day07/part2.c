#include "../helpers/input_helper.h"
#include "../helpers/arraylist.h"
#include "../helpers/memory.h"

#include <stdlib.h>
#include <string.h>

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

    int sequence[5] = {-1,-1,-1,-1,-1};
    int* max_sequence = checked_calloc(5,sizeof(int));

    int max_value = 0;

    dfs_sequence(sequence, 0b00000, 0, memory, &max_sequence, &max_value);

    printf("[");
    for (int i = 0; i <= 4; i++) {
        printf("%d, ", max_sequence[i]);
    }
    printf("] -> %d\n", max_value);
}

void dfs_sequence(int sequence[5], int sequence_bitmask, int depth, arraylist* memory, int** max_sequence, int* max_score) {
    if (depth == 5) {
        int last_output = 0;
        int last_actual_output;
        int finished = 0;
        int* ips = checked_calloc(5, sizeof(int));
        arraylist** memories = checked_calloc(5, sizeof(arraylist));

        for (int i = 0; i <= 4; i++) {
            memories[i] = arraylist_copy(memory);
            int* inputs = checked_calloc(2, sizeof(int));
            inputs[0] = sequence[i];
            inputs[1] = last_output;
            printf("%d ", i);
            last_output = amplifier_output(&(memories[i]), inputs, &(ips[i]));
            printf(" results in %d\n", last_output);
            if (last_output == -1337) {
                finished++;
            } else {
                last_actual_output = last_output;
            }
        }
        while (finished == 0) {
            for (int i = 0; i <= 4; i++) {
                int* inputs = checked_calloc(1, sizeof(int));
                inputs[0] = last_output;
                printf("%d ", i);
                last_output = amplifier_output(&(memories[i]), inputs, &(ips[i]));
                printf(" results in %d\n", last_output);
                if (last_output == -1337) {
                    finished++;
                } else {
                    last_actual_output = last_output;
                }
            }
        }
        puts("Gave up");
        if (last_actual_output > *max_score) {
            memcpy(*max_sequence, sequence, 5 * sizeof(int));
            *max_score = last_actual_output;
        }
    }
    for (int a = 9; a >= 5; a--) {
        if ((sequence_bitmask & (1 << a)) == 0) {
            sequence[depth] = a;
            dfs_sequence(sequence, sequence_bitmask | (1<<a), depth+1, memory, max_sequence, max_score);
        }
    }
}

int amplifier_output(arraylist** memory, int* inputs, int* ip) {
    printf("starting at IP = %d with inputs %d, %d", *ip, inputs[0], inputs[1]);
    int input_pointer = 0;

    while (1 == 1) {
        int full_opcode = 0;
        arraylist_get(*memory, *ip, &full_opcode);

        int first_position = 0;
        int second_position = 0;
        int third_position = 0;

        int first_value = 0;
        int second_value = 0;
        int third_value = 0;

        int position = 0;
        int value = 0;

        int opcode = full_opcode % 100;
        int mode_a = (full_opcode / 100) % 10;
        int mode_b = (full_opcode / 1000) % 10;
        int mode_c = (full_opcode / 10000) % 10;

        // printf("opcode %d = %d modes a:%d b:%d c:%d \n", *ip, opcode, mode_a, mode_b, mode_c);

        switch (opcode % 100) {
        case 1:
            // puts("+");

            arraylist_get(*memory, *ip + 1, &first_position);
            arraylist_get(*memory, *ip + 2, &second_position);
            arraylist_get(*memory, *ip + 3, &position);

            first_value = get_value(*memory, first_position, mode_a);
            second_value = get_value(*memory, second_position, mode_b);

            int sum = first_value + second_value;

            // printf("Setting %d to %d\n", position, sum);
            arraylist_set(*memory, position, &sum);

            *ip += 4;
            break;
        case 2:
            // puts("*");

            arraylist_get(*memory, *ip + 1, &first_position);
            arraylist_get(*memory, *ip + 2, &second_position);
            arraylist_get(*memory, *ip + 3, &position);

            first_value = get_value(*memory, first_position, mode_a);
            second_value = get_value(*memory, second_position, mode_b);

            int product = first_value * second_value;

            // printf("Setting %d to %d\n", position, product);
            arraylist_set(*memory, position, &product);

            *ip += 4;
            break;
        case 3:
            // puts("input");

            arraylist_get(*memory, *ip + 1, &position);

            get_value(*memory, position, mode_a);

            int input = inputs[input_pointer];
            input_pointer++;

            arraylist_set(*memory, position, &input);

            *ip += 2;
            break;
        case 4:
            // puts("print");

            arraylist_get(*memory, *ip + 1, &position);

            arraylist_get(*memory, position, &value);

            *ip += 2;

            return value;
            break;
        case 5:
            arraylist_get(*memory, *ip + 1, &first_position);
            arraylist_get(*memory, *ip + 2, &second_position);

            first_value = get_value(*memory, first_position, mode_a);
            second_value = get_value(*memory, second_position, mode_b);

            // printf("jump-if-true %d, %d\n", first_value, second_value);

            if (first_value != 0) {
                *ip = second_value;
            } else {
                *ip += 3;
            }

            break;
        case 6:

            arraylist_get(*memory, *ip + 1, &first_position);
            arraylist_get(*memory, *ip + 2, &second_position);

            first_value = get_value(*memory, first_position, mode_a);
            second_value = get_value(*memory, second_position, mode_b);

            // printf("jump-if-false %d, %d\n", first_value, second_value);

            if (first_value == 0) {
                *ip = second_value;
            } else {
                *ip += 3;
            }
            
            break;
        case 7:
            arraylist_get(*memory, *ip + 1, &first_position);
            arraylist_get(*memory, *ip + 2, &second_position);
            arraylist_get(*memory, *ip + 3, &third_position);

            first_value = get_value(*memory, first_position, mode_a);
            second_value = get_value(*memory, second_position, mode_b);

            // printf("less than %d, %d, %d\n", first_value, second_value, third_position);

            if (first_value < second_value) {
                value = 1;
            } else {
                value = 0;
            }

            arraylist_set(*memory, third_position, &value);
            
            *ip += 4;
            break;
        case 8:
            // puts("equals");

            arraylist_get(*memory, *ip + 1, &first_position);
            arraylist_get(*memory, *ip + 2, &second_position);
            arraylist_get(*memory, *ip + 3, &third_position);

            first_value = get_value(*memory, first_position, mode_a);
            second_value = get_value(*memory, second_position, mode_b);

            // printf("equals %d, %d, %d\n", first_value, second_value, third_position);

            if (first_value == second_value) {
                value = 1;
            } else {
                value = 0;
            }

            arraylist_set(*memory, third_position, &value);
            
            *ip += 4;
            break;
        case 99:
            puts("HALT");
            return -1337;

        default:
            fprintf(stderr, "Unknown opcode %d\n", opcode);
            exit(1);
        }
    }
}
