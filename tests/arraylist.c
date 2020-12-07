#include "acutest.h"
#include "../helpers/arraylist.h"
#include "../helpers/memory.h"

#include <stdbool.h>

#define check_expected_int(expected, got) _check_expected_int(expected, got, __FILE__, __LINE__)
#define check_expected_coord(expected, got) _check_expected_coord(expected, got, __FILE__, __LINE__)

typedef struct coord_struct {
    int x;
    int y
} coord;

void _check_expected_int(int expected, int got, char* file, int line) {
    TEST_CHECK(expected == got);
    TEST_MSG("Expected %d but got %d at %s:%d", expected, got, file, line);
}

void _check_expected_coord(coord* expected, coord* got, char* file, int line) {
    TEST_CHECK(expected->x == got->x);
    TEST_MSG("Expected x value of %d but got %d at %s:%d", expected->x, got->x, file, line);
    TEST_CHECK(expected->y == got->y);
    TEST_MSG("Expected y value of %d but got %d at %s:%d", expected->y, got->y, file, line);
}

// ============ arraylist<INT> ============

void test_arraylist_create_int() {
    arraylist* al = arraylist_new(sizeof(int));

    TEST_CHECK(al->value_size == sizeof(int));
    TEST_MSG("Size of element did not match, expected %ld, got %ld", sizeof(int), al->value_size);

    TEST_CHECK(al->num_values == 0);
    TEST_MSG("Arraylist already has %d values in it", al->num_values);

    TEST_CHECK(al->values != NULL);
    TEST_MSG("Arraylists value is not initialized");

    arraylist_free(al);
}

void test_arraylist_use_int() {
    arraylist* al = arraylist_new(sizeof(int));

    int five = 5;
    arraylist_add(al, &five);

    check_expected_int(5, ((int*)al->values)[0]);

    int result;
    arraylist_get(al, 0, &result);
    check_expected_int(5, result);

    result = arraylist_contains(al, &five);
    check_expected_int(five, 5);
    check_expected_int(result, 0);

    arraylist_free(al);
}

void test_arraylist_use_int_large_index() {
    arraylist* al = arraylist_new(sizeof(int));

    int five = 5;
    arraylist_set(al, 10000, &five);

    check_expected_int(5, ((int*)al->values)[10000]);

    int result;
    arraylist_get(al, 10000, &result);
    check_expected_int(5, result);

    result = arraylist_contains(al, &five);
    check_expected_int(five, 5);
    check_expected_int(result, 10000);

    arraylist_free(al);
}

// ============ arraylist<COORD> ============

void test_arraylist_create_coord() {
    arraylist* al = arraylist_new(sizeof(coord));

    TEST_CHECK(al->value_size == sizeof(coord));
    TEST_MSG("Size of element did not match, expected %ld, got %ld", sizeof(coord), al->value_size);

    TEST_CHECK(al->num_values == 0);
    TEST_MSG("Arraylist already has %d values in it", al->num_values);

    TEST_CHECK(al->values != NULL);
    TEST_MSG("Arraylists value is not initialized");

    arraylist_free(al);
}

void test_arraylist_use_coord() {
    arraylist* al = arraylist_new(sizeof(coord));

    coord* fivesix = checked_calloc(sizeof(coord), 1);

    fivesix->x = 5;
    fivesix->y = 6;

    arraylist_add(al, fivesix);

    check_expected_coord(fivesix, &((coord*)al->values)[0]);

    coord* result = checked_malloc(sizeof(coord));
    arraylist_get(al, 0, result);
    check_expected_coord(fivesix, result);

    int result_pos = arraylist_contains(al, fivesix);
    check_expected_coord(fivesix, result);
    check_expected_int(result_pos, 0);

    arraylist_free(al);
}

// RESIZING

void test_arraylist_resize() {
    arraylist* al = arraylist_new(sizeof(int));
    for (int i = 0; i < ARRAYLIST_INITIAL_SIZE; i++) {
        arraylist_add(al, &i);
    }
    check_expected_int(ARRAYLIST_INITIAL_SIZE, al->size);
    int next = 5000;
    arraylist_add(al, &next);
    check_expected_int(ARRAYLIST_INITIAL_SIZE * 2, al->size);

    int result;
    arraylist_get(al, ARRAYLIST_INITIAL_SIZE, &result);
    check_expected_int(next, result);

    arraylist_set(al, next, &next);
    check_expected_int(next+1, al->size);

    arraylist_get(al, next, &result);
    check_expected_int(next, result);

    arraylist_free(al);
}

TEST_LIST = {
    {"arraylist_create_int", test_arraylist_create_int},
    {"arraylist_use_int", test_arraylist_use_int},
    {"arraylist_use_int_large_index", test_arraylist_use_int_large_index},
    {"arraylist_create_coord", test_arraylist_create_coord},
    {"arraylist_use_coord", test_arraylist_use_coord},
    {"arraylist_resize", test_arraylist_resize},
    {NULL, NULL}
};