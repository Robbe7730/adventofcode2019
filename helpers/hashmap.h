#ifndef HELPERS_HASHMAP_H
#define HELPERS_HASHMAP_H
#include <stdlib.h>

#define HASHMAP_SIZE 2048

typedef struct hashmap_entry_struct {
    void* key;
    void* value;
    struct hashmap_entry_struct* next;
} hashmap_entry;

typedef struct hashmap_struct {
    hashmap_entry** values;
    size_t key_size;
    size_t value_size;
} hashmap;

hashmap* hashmap_new(size_t key_size, size_t value_size);
hashmap_entry* hashmap_entry_new(hashmap* hm, void* key, void* value);

unsigned int hashmap_hash(void* value, size_t size);

void hashmap_add(hashmap* d, void* key, void* value);
hashmap_entry* hashmap_query(hashmap* hm, void* key);

void hashmap_entry_free(hashmap_entry* d);
void hashmap_free(hashmap* d);

#endif // HELPERS_HASHMAP_H
