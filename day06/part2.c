#include <stdio.h>
#include <string.h>

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
        }

        if (hashmap_query(hm, id) == NULL) {
            planet* curr_planet = checked_malloc(sizeof(planet));

            curr_planet->id = id;
            curr_planet->orbiting = hashmap_query(hm, orbiting)->value;

            // printf("Adding %s orbiting %p\n", curr_planet->id, curr_planet->orbiting);
            hashmap_add(hm, id, curr_planet);
        } else {
            planet* curr_planet = hashmap_query(hm, id)->value;
            planet* parent_planet = hashmap_query(hm, orbiting)->value;
            curr_planet->orbiting = parent_planet;
            // printf("Adding orbit between %s and %s\n", curr_planet->id, parent_planet->id);
        }
    }

    planet* curr_planet = hashmap_query(hm, "SAN")->value;

    hashmap* san_path = hashmap_new(3 * sizeof(char), sizeof(int));

    int i = 0;

    while (strcmp(curr_planet->id, "COM") != 0) {
        curr_planet = curr_planet->orbiting;
        printf("SAN: %s takes %i steps\n", curr_planet->id, i);
        hashmap_add(san_path, &(curr_planet->id), &i);
        i++;
    }

    curr_planet = hashmap_query(hm, "YOU")->value;

    int steps = 0;

    while (strcmp(curr_planet->orbiting->id, "COM") != 0) {
        curr_planet = curr_planet->orbiting;
        printf("YOU: %s takes %i steps\n", curr_planet->id, steps);
        hashmap_entry* found = hashmap_query(san_path, &(curr_planet->id));
        if (found != NULL) {
            puts("BANG");
            steps += (*(int*) found->value);
            break;
        }
        steps++;
    }

    hashmap_free(hm);
    arraylist_free(san_path);

    printf("result = %d\n", steps);
}