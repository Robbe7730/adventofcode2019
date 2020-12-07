#ifndef ARRAYLIST_H
#define ARRAYLIST_H

#include <stdbool.h>
#include <stdio.h>

#define ARRAYLIST_INITIAL_SIZE 32

typedef struct arraylist_struct {
    void* values;
    int num_values;
    int size;
    size_t value_size;
} arraylist;

arraylist* arraylist_new(size_t emem_size);
arraylist* arraylist_copy(arraylist* al);

void arraylist_add(arraylist* al, void* value);
void arraylist_resize(arraylist* al, size_t new_size);
void arraylist_get(arraylist* al, int index, void* result);
void arraylist_set(arraylist* al, int index, void* new_value);
int arraylist_contains(arraylist* al, void* value);
int arraylist_contains_comparator(arraylist* al, void* value, bool (*cmp)(void*, void*));

void arraylist_free(arraylist* al);

#endif // ARRAYLIST_H
