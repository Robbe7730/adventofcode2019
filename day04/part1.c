#include <stdbool.h>
#include <math.h>
#include <stdio.h>

bool matches(int num) {
    bool has_double = false;
    int last_digit = -1;
    for (int i = 0; i < 6; i++) {
        int curr_digit = ((num / (int) pow(10, (5 - i)))) % 10;
        if (curr_digit == last_digit) {
            has_double = true;
        }
        if (curr_digit < last_digit) {
            return false;
        }
        last_digit = curr_digit;
    }
    return has_double;
}

int main() {
    int count = 0;
    for (int i = 138241; i < 674034; i++) {
        if (matches(i)) {
            count++;
        }
    }
    printf("result = %d\n", count);
}