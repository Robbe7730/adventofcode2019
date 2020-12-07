#include "acutest.h"
#include "../helpers/hashmap.h"
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

void test_hashmap_usage_int_int() {
    hashmap* hm = hashmap_new(sizeof(int), sizeof(int));

    for(int key = 0; key < HASHMAP_SIZE; key++) {
        int value = key+1;
        hashmap_add(hm, &key, &value);
    }

    for (int i = 0; i < HASHMAP_SIZE; i++) {
        hashmap_entry* res = hashmap_query(hm, &i);
        TEST_CHECK(res != NULL);
        TEST_MSG("Did not find value with key %d.\n", i);
        check_expected_int(i+1, *(int*) (res->value));
        check_expected_int(i, *(int*) (res->key));
    }

    hashmap_free(hm);
}

void test_hashmap_usage_int_coord() {
    hashmap* hm = hashmap_new(sizeof(int), sizeof(coord));

    coord* curr_coord = checked_malloc(sizeof(coord));

    for(int i = 0; i < HASHMAP_SIZE; i++) {
        curr_coord->x = i+1;
        curr_coord->y = i+2;
        hashmap_add(hm, &i, curr_coord);
    }

    for (int i = 0; i < HASHMAP_SIZE; i++) {
        coord* expected_value = checked_malloc(sizeof(coord));
        expected_value->x = i+1;
        expected_value->y = i+2;

        hashmap_entry* res = hashmap_query(hm, &i);
        TEST_CHECK(res != NULL);
        TEST_MSG("Did not find value with key %d.\n", i);

        check_expected_coord(expected_value, res->value);
        check_expected_int(i, *(int*) (res->key));
    }

    hashmap_free(hm);
}

void test_hashmap_usage_coord_int() {
    hashmap* hm = hashmap_new(sizeof(coord), sizeof(int));

    coord* curr_coord = checked_malloc(sizeof(coord));

    for(int i = 0; i < HASHMAP_SIZE; i++) {
        curr_coord->x = i+1;
        curr_coord->y = i+2;
        hashmap_add(hm, curr_coord, &i);
    }

    coord* key = checked_malloc(sizeof(coord));
    for(int i = 0; i < HASHMAP_SIZE; i++) {
        key->x = i+1;
        key->y = i+2;

        hashmap_entry* res = hashmap_query(hm, key);
        TEST_CHECK(res != NULL);
        TEST_MSG("Did not find value with coord (%d,%d).\n", key->x, key->y);

        check_expected_int(i, *(int*)(res->value));
        check_expected_coord(key, res->key);
    }


    hashmap_free(hm);
}

TEST_LIST = {
    { "hashmap with int, int", test_hashmap_usage_int_int },
    { "hashmap with int, coord", test_hashmap_usage_int_coord },
    { "hashmap with coord, int", test_hashmap_usage_coord_int },
    { NULL, NULL }
};