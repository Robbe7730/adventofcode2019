#include <ctype.h>
#include <stdlib.h>
#include <errno.h>
#include "input_helper.h"

FILE* open_file(char* filename) {
    FILE* ret = fopen(filename, "re");

    if (ret == NULL) {
        fputs("Could not open file\n", stderr);
        exit(EXIT_FAILURE);
    }

    return ret;
}

int read_int(FILE* f, char separator, bool* succesful) {
    int ret = 0;
    int mul = 1;

    *succesful = true;

    int curr_char = fgetc(f);
    if (curr_char == '-') {
        mul = -1;
        curr_char = fgetc(f);
    }
    while ( curr_char != separator ) {
        if (curr_char == EOF) {
            fputs("File ended before separator was found \n", stderr);
            *succesful = false;
            return READ_INT_NULL;
        }
        
        if (!isdigit(curr_char)) {
            fputs("Unknown digit found \n", stderr);
            *succesful = false;
            return READ_INT_NULL;
        }

        ret *= 10;
        ret += curr_char - '0';

        curr_char = fgetc(f);
    }

    return mul*ret;
}

unsigned long long int read_unsigned_long_long_int(FILE* f, char separator, bool* succesful) {
    unsigned long long int ret = 0;
    int mul = 1;

    *succesful = true;

    int curr_char = fgetc(f);
    if (curr_char == '-') {
        mul = -1;
        curr_char = fgetc(f);
    }
    while ( curr_char != separator ) {
        if (curr_char == EOF) {
            fputs("File ended before separator was found \n", stderr);
            *succesful = false;
            return READ_INT_NULL;
        }
        
        if (!isdigit(curr_char)) {
            fputs("Unknown digit found \n", stderr);
            *succesful = false;
            return READ_INT_NULL;
        }

        ret *= (unsigned long long int) 10;
        ret += (unsigned long long int)(curr_char - '0');

        curr_char = fgetc(f);
    }

    return mul*ret;
}

bool read_string_until(FILE* f, char separator, char** result) {
    int curr_char = fgetc(f);
    int curr_len = 0;
    while ( curr_char != separator ) {
        if (curr_char == EOF) {
            fputs("File ended before separator was found \n", stderr);
            return false;
        }
        
        (*result)[curr_len] = (char) curr_char;
        curr_len++;

        curr_char = fgetc(f);
    }

  return true;
}
