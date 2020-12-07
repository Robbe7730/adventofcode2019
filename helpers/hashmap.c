#include "hashmap.h"

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "../helpers/memory.h"

hashmap* hashmap_new(size_t key_size, size_t value_size) {
    hashmap* ret = checked_malloc(sizeof(hashmap));
    ret->key_size = key_size;
    ret->value_size = value_size;
    ret->values = checked_calloc(HASHMAP_SIZE, sizeof(hashmap_entry));
    return ret;
}

hashmap_entry* hashmap_entry_new(hashmap* hm, void* key, void* value) {
    hashmap_entry* ret = checked_malloc(sizeof(hashmap_entry));

    ret->key = objcopy(key, hm->key_size);
    ret->value = objcopy(value, hm->value_size);
    ret->next = NULL;

    return ret;
}

void hashmap_put(hashmap* hm, void* key, void* value) {
    unsigned int hashvalue = hashmap_hash(key, hm->key_size);

    if (hm->values[hashvalue] == NULL) {
        hm->values[hashvalue] = hashmap_entry_new(hm, key, value);
    } else {
        // fprintf(stderr, "Collision at %d for key %s\n", hashvalue, key);

        int hashvalue_original = hashvalue;

        hashvalue++;

        while (hm->values[hashvalue] != NULL && hashvalue != hashvalue_original) {
            hashvalue = (hashvalue + 1) % HASHMAP_SIZE;
        }

        hashmap_entry* last_entry = hm->values[hashvalue_original];
        while (last_entry->next != NULL) {
            last_entry = last_entry->next;
        }

        if (hashvalue == hashvalue_original) {
            fprintf(stderr, "Could not insert value, hashmap full");
            exit(EXIT_FAILURE);
        } else {
            hm->values[hashvalue] = hashmap_entry_new(hm, key, value);
            last_entry->next = hm->values[hashvalue];
        }
    }
}

void hashmap_add(hashmap* hm, void* key, void* value) {
    hashmap_entry* existing_entry = hashmap_query(hm, key);
    if (existing_entry == NULL) {
        hashmap_put(hm, key, value);
    } else {
        checked_free(existing_entry->value);
        existing_entry->value = objcopy(value, hm->value_size);
    }
}

unsigned int hashmap_hash(void* value, size_t size) {
    unsigned int hashvalue = 0;
    for (int i = 0; i < size; i++) {
        hashvalue = ((char*) value)[i] + 31 * hashvalue;
    }
    return hashvalue % HASHMAP_SIZE;
}

hashmap_entry* hashmap_query(hashmap* hm, void* key) {
    unsigned int hashvalue = hashmap_hash(key, hm->key_size);

    hashmap_entry* curr_entry = hm->values[hashvalue];

    while (true) {
        if (curr_entry == NULL) {
            return NULL;
        } 
        if (memcmp(curr_entry->key, key, hm->key_size) == 0) {
            return curr_entry;
        }
        curr_entry = curr_entry->next;
    }
}

void hashmap_entry_free(hashmap_entry* hm) {
    checked_free(hm->key);
    checked_free(hm->value);
    checked_free(hm);
}

void hashmap_free(hashmap* hm) {
    for (int i = 0; i < HASHMAP_SIZE; i += sizeof(hashmap_entry)) {
        if (hm->values[i] != NULL) {
            hashmap_entry_free(hm->values[i]);
        }
    }
    checked_free(hm);
}