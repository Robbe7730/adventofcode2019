#include "../helpers/input_helper.h"
#include "../helpers/arraylist.h"
#include "../helpers/memory.h"

#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <assert.h>

#define MEMORY_TYPE long long int

MEMORY_TYPE get_value(arraylist* memory, MEMORY_TYPE value, int mode, MEMORY_TYPE relative_base) {
    //printf("get_value(memory, %lld, %lld, %lld)\n", value, mode, relative_base);
    MEMORY_TYPE new_value;
    switch (mode) {
    case 0:
        arraylist_get(memory, value, &new_value);
        //printf("    -> %lld\n", new_value);
        return new_value;
    case 1:
        //printf("    -> %lld\n", value);
        return value;
    case 2:
        arraylist_get(memory, relative_base + value, &new_value);
        //printf("    -> %lld\n", new_value);
        return new_value;
    default:
        fprintf(stderr, "Invalid mode %d", mode);
        exit(EXIT_FAILURE);
    }
}

void set_value(arraylist* memory, MEMORY_TYPE pointer, int mode, MEMORY_TYPE relative_base, MEMORY_TYPE value) {
    //printf("set_value(memory, %lld, %d, %lld, %lld)\n", pointer, mode, relative_base, value);
    MEMORY_TYPE assertion;
    switch (mode) {
    case 0:
        //printf("setting %lld to %lld\n", pointer, value);
        arraylist_set(memory, pointer, &value);

        arraylist_get(memory, pointer, &assertion);
        assert(assertion == value);
        break;
    case 1:
        //printf("setting %lld to %lld\n", pointer, value);
        arraylist_set(memory, pointer, &value);

        arraylist_get(memory, pointer, &assertion);
        assert(assertion == value);
        break;
    case 2:
        //printf("setting %lld to %lld\n", relative_base + pointer, value);
        arraylist_set(memory, relative_base + pointer, &value);

        arraylist_get(memory, relative_base + pointer, &assertion);
        assert(assertion == value);
        break;
    default:
        fprintf(stderr, "Invalid mode %d", mode);
        exit(EXIT_FAILURE);
    }
}


void amplifier_output(arraylist* memory, int* inputs) {
    MEMORY_TYPE ip = 0;
    
    MEMORY_TYPE input_pointer = 0;
    MEMORY_TYPE relative_base = 0;

    while (1 == 1) {
        MEMORY_TYPE full_opcode = 0;
        arraylist_get(memory, ip, &full_opcode);

        MEMORY_TYPE first_position = 0;
        MEMORY_TYPE second_position = 0;
        MEMORY_TYPE third_position = 0;

        MEMORY_TYPE first_value = 0;
        MEMORY_TYPE second_value = 0;
        MEMORY_TYPE third_value = 0;

        MEMORY_TYPE value;

        int opcode = full_opcode % 100;
        int mode_a = (full_opcode / 100) % 10;
        int mode_b = (full_opcode / 1000) % 10;
        int mode_c = (full_opcode / 10000) % 10;

        //printf("opcode %d = %d modes a:%d b:%d c:%d \n", ip, opcode, mode_a, mode_b, mode_c);

        switch (opcode % 100) {
        case 1:
            //puts("+");

            arraylist_get(memory, ip + 1, &first_position);
            arraylist_get(memory, ip + 2, &second_position);
            arraylist_get(memory, ip + 3, &third_position);

            first_value = get_value(memory, first_position, mode_a, relative_base);
            second_value = get_value(memory, second_position, mode_b, relative_base);

            MEMORY_TYPE sum = first_value + second_value;

            set_value(memory, third_position, mode_c, relative_base, sum);

            ip += 4;
            break;
        case 2:
            //puts("*");

            arraylist_get(memory, ip + 1, &first_position);
            arraylist_get(memory, ip + 2, &second_position);
            arraylist_get(memory, ip + 3, &third_position);

            first_value = get_value(memory, first_position, mode_a, relative_base);
            second_value = get_value(memory, second_position, mode_b, relative_base);

            MEMORY_TYPE product = first_value * second_value;

            set_value(memory, third_position, mode_c, relative_base, product);

            ip += 4;
            break;
        case 3:
            //puts("input");

            arraylist_get(memory, ip + 1, &first_position);

            unsigned long long input = inputs[input_pointer];
            input_pointer++;

            set_value(memory, first_position, mode_a, relative_base, input);

            ip += 2;
            break;
        case 4:
            //puts("print");

            arraylist_get(memory, ip + 1, &first_position);

            first_value = get_value(memory, first_position, mode_a, relative_base);

            printf("OUT: %lld\n", first_value, ip);

            ip += 2;
            break;
        case 5:
            arraylist_get(memory, ip + 1, &first_position);
            arraylist_get(memory, ip + 2, &second_position);

            first_value = get_value(memory, first_position, mode_a, relative_base);
            second_value = get_value(memory, second_position, mode_b, relative_base);

            //printf("jump-if-true %d, %d\n", first_value, second_value);

            if (first_value != 0) {
                ip = second_value;
            } else {
                ip += 3;
            }

            break;
        case 6:

            arraylist_get(memory, ip + 1, &first_position);
            arraylist_get(memory, ip + 2, &second_position);

            first_value = get_value(memory, first_position, mode_a, relative_base);
            second_value = get_value(memory, second_position, mode_b, relative_base);

            //printf("jump-if-false %d, %d\n", first_value, second_value);

            if (first_value == 0) {
                ip = second_value;
            } else {
                ip += 3;
            }
            
            break;
        case 7:
            //puts("less than");
            arraylist_get(memory, ip + 1, &first_position);
            arraylist_get(memory, ip + 2, &second_position);
            arraylist_get(memory, ip + 3, &third_position);

            first_value = get_value(memory, first_position, mode_a, relative_base);
            second_value = get_value(memory, second_position, mode_b, relative_base);

            if (first_value < second_value) {
                value = 1;
            } else {
                value = 0;
            }

            set_value(memory, third_position, mode_c, relative_base, value);
            
            ip += 4;
            break;
        case 8:
            //puts("equals");

            arraylist_get(memory, ip + 1, &first_position);
            arraylist_get(memory, ip + 2, &second_position);
            arraylist_get(memory, ip + 3, &third_position);

            first_value = get_value(memory, first_position, mode_a, relative_base);
            second_value = get_value(memory, second_position, mode_b, relative_base);

            if (first_value == second_value) {
                value = 1;
            } else {
                value = 0;
            }

            set_value(memory, third_position, mode_c, relative_base, value);
            
            ip += 4;
            break;
        case 9:
            arraylist_get(memory, ip + 1, &first_position);

            relative_base += get_value(memory, first_position, mode_a, relative_base);
            //printf("setting relative base %d\n", relative_base);
            ip += 2;
            break;
        case 99:
            //puts("HALT");
            return;

        default:
            fprintf(stderr, "Unknown opcode %d\n", opcode);
            exit(1);
        }
    }
}

int main(int argv, char** argc) {
    if (argv != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argc[0]);
    }

    FILE* f = open_file(argc[1]);

    arraylist* memory = arraylist_new(sizeof(MEMORY_TYPE));
    MEMORY_TYPE num;
    bool succesful = true;
    while (succesful) {
        num = read_unsigned_long_long_int(f, ',', &succesful);
        arraylist_add(memory, &num);
    }

    int input[1] = {2};
    amplifier_output(memory, input);

}