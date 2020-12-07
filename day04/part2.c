#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include <assert.h>

bool matches(int num) {
    bool has_double = false;
    int streak = 0;
    int last_digit = -1;
    for (int i = 0; i < 6; i++) {
        int curr_digit = ((num / (int) pow(10, (5 - i)))) % 10;
        if (curr_digit == last_digit) {
            streak++;
        } else {
            if (streak == 2) {
                has_double = true;
            }
            streak = 1;
        }
        if (curr_digit < last_digit) {
            return false;
        }
        last_digit = curr_digit;
    }
    return has_double || streak == 2;
}

int main() {
    int count = 0;
    matches(111122);
    fflush(stdout);
    assert(matches(112233));
    assert(!matches(123444));
    assert(matches(111122));
    for (int i = 138241; i < 674034; i++) {
        if (matches(i)) {
            count++;
        }
    }
    printf("result = %d\n", count);
}