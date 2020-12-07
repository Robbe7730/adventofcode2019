#include "arraylist.h"

#include "memory.h"
#include "string.h"

arraylist* arraylist_new(size_t value_size) {
    arraylist* ret = checked_malloc(sizeof(arraylist));
    
    ret->values = checked_calloc(ARRAYLIST_INITIAL_SIZE, value_size);
    ret->num_values = 0;
    ret->size = ARRAYLIST_INITIAL_SIZE;
    ret->value_size = value_size;

    return ret;
}

void arraylist_add(arraylist* al, void* value) {
    if (al->num_values == al->size) {
        arraylist_resize(al, 2 * al->size);
    }

    memcpy(&al->values[al->num_values * al->value_size], value, al->value_size);
    al->num_values++;
}

void arraylist_resize(arraylist* al, size_t new_size) {
    al->values = checked_realloc(al->values, new_size * al->value_size);
    al->size = new_size;
}

void arraylist_free(arraylist* al) {
    checked_free(al->values);
    checked_free(al);
}

void arraylist_get(arraylist* al, int index, void* result) {
    // if (index >= al->size) {
    //     memset(result, 0, al->value_size);
    // } else {
    //     memcpy(result, &al->values[index * al->value_size], al->value_size);
    // }
    if (index >= al->size) {
        arraylist_resize(al, index+1);
    }
    memcpy(result, &al->values[index * al->value_size], al->value_size);
}

void arraylist_set(arraylist* al, int index, void* new_value) {
    if (index >= al->size) {
        arraylist_resize(al, index + 1);
    }
    memcpy(&al->values[index * al->value_size], new_value, al->value_size);
}

arraylist* arraylist_copy(arraylist* al) {
    arraylist* ret = arraylist_new(al->value_size);

    ret->size = al->size;
    arraylist_resize(ret, al->num_values * al->value_size);
    memcpy(ret->values, al->values, al->num_values * al->value_size);

    return ret;
}

int arraylist_contains(arraylist* al, void* value) {
    for (int i = 0; i < al->size; i++) {
        if (memcmp(&al->values[i * al->value_size], value, al->value_size) == 0) {
            return i;
        }
    }
    return -1;
}

int arraylist_contains_comparator(arraylist* al, void* value, bool (*cmp)(void*, void*)) {
    for (int i = 0; i < al->size; i++) {
        if (cmp(&al->values[i * al->value_size], value)) {
            return i;
        }
    }
    return -1;
}