#ifndef INPUT_HELPER_H
#define INPUT_HELPER_H

#include <stdio.h>
#include <stdbool.h>

#define READ_INT_NULL -1

FILE* open_file(char* filename);
int read_int(FILE* f, char separator, bool* succesful);
unsigned long long int read_unsigned_long_long_int(FILE* f, char separator, bool* succesfull);
bool read_string_until(FILE* f, char separator, char** result);

#endif //INPUT_HELPER_H
