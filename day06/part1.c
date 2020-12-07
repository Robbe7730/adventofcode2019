#include <stdio.h>
#include "../helpers/input_helper.h"
#include "../helpers/memory.h"
#include "../helpers/hashmap.h"
#include "../helpers/arraylist.h"

typedef struct planet_struct {
    char* id;
    struct planet_struct* orbiting;
} planet;

int main(int argv, char** argc) {
    if (argv != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argc[0]);
        exit(EXIT_FAILURE);
    }

    FILE* f = open_file(argc[1]);

    hashmap* hm = hashmap_new(3 * sizeof(char), sizeof(planet));
    arraylist* names = arraylist_new(3 * sizeof(char));

    bool running = true;

    while(running) {
        char* id = checked_calloc(3, sizeof(char));
        char* orbiting = checked_calloc(3, sizeof(char));

        running = read_string_until(f, ')', &orbiting) & read_string_until(f, '\n', &id);

        if (hashmap_query(hm, orbiting) == NULL) {
            planet* orbiting_planet = checked_malloc(sizeof(planet));

            orbiting_planet->id = orbiting;
            orbiting_planet->orbiting = NULL;

            // printf("Adding %s as parent planet\n", orbiting_planet->id);
            hashmap_add(hm, orbiting, orbiting_planet);
            arraylist_add(names, orbiting);
        }

        if (hashmap_query(hm, id) == NULL) {
            planet* curr_planet = checked_malloc(sizeof(planet));

            curr_planet->id = id;
            curr_planet->orbiting = hashmap_query(hm, orbiting)->value;

            // printf("Adding %s orbiting %p\n", curr_planet->id, curr_planet->orbiting);
            hashmap_add(hm, id, curr_planet);
            arraylist_add(names, id);
        } else {
            planet* curr_planet = hashmap_query(hm, id)->value;
            planet* parent_planet = hashmap_query(hm, orbiting)->value;
            curr_planet->orbiting = parent_planet;
            // printf("Adding orbit between %s and %s\n", curr_planet->id, parent_planet->id);
        }
    }

    int indirect = 0;
    int direct = 0;

    for (int i = 0; i < HASHMAP_SIZE; i++) {
        hashmap_entry* curr_entry = hm->values[i];
        if (curr_entry != NULL) {
            printf("%d: %s -> %s\n", i, curr_entry->key, ((planet*)(curr_entry->value))->id);
        } else {
            printf("%d: (empty)\n", i);
        }
    }

    for (int i = 0; i < names->num_values; i++) {
        char* result = checked_calloc(3, sizeof(char));
        arraylist_get(names, i, result);
        printf("%d: %s\n", i, result);
    }

    for (int i = 0; i < names->num_values; i++) {
        char* name = checked_calloc(3, sizeof(char));
        
        arraylist_get(names, i, name);
        hashmap_entry* found = hashmap_query(hm, name);
        if (found == NULL) {
            fprintf(stderr, "Planet %s did not exist.\n", name);
            exit(EXIT_FAILURE);
        }
        planet* curr_planet = found->value;

        if (curr_planet->orbiting != NULL) {
            // printf("%s directly orbits %s\n", curr_planet->id, curr_planet->orbiting->id);
            direct++;

            curr_planet = curr_planet->orbiting;

            while (curr_planet->orbiting != NULL) {
                // printf("%s indirectly orbits %s\n", curr_planet->id, curr_planet->orbiting->id);
                curr_planet = curr_planet->orbiting;
                indirect++;
            }
        } else {
            // printf("%s doesn't directly orbit anything\n", curr_planet->id);
        }
    }

    hashmap_free(hm);
    arraylist_free(names);

    printf("result = d:%d + i:%d = %d\n", direct, indirect, direct + indirect);
}