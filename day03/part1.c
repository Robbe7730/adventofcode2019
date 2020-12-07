#include "../helpers/input_helper.h"
#include "../helpers/memory.h"
#include "../helpers/arraylist.h"

#include <stdbool.h>
#include <limits.h>

typedef struct coord_struct {
    int x;
    int y;
} coord;

typedef struct wire_struct {
    int dx;
    int dy;
    coord* pos;
    int remaining;
} wire;


int main(int argc, char** argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input 1> <input2>\n", argv[0]);
    }

    FILE* f1 = open_file(argv[1]);
    FILE* f2 = open_file(argv[2]);

    int first_collision_x = 0;
    int first_collision_y = 0;

    int second_collision_x = 0;
    int second_collision_y = 0;

    wire* wire1 = checked_calloc(sizeof(wire), 1);
    wire* wire2 = checked_calloc(sizeof(wire), 1);

    wire1->pos = checked_calloc(sizeof(coord), 1);
    wire2->pos = checked_calloc(sizeof(coord), 1);

    wire1->pos->x = 0;
    wire1->pos->y = 0;

    wire2->pos->x = 0;
    wire2->pos->y = 0;

    arraylist* trail1 = arraylist_new(sizeof(coord));

    bool running = true;

    while (running) {
        if (wire1->remaining <= 0) {
            //printf("Corner at %d, %d\n", wire1->pos->x, wire1->pos->y);
            int next_direction = fgetc(f1);
            switch (next_direction) {
            case 'U':
                // puts("U");
                wire1->dx = 1;
                wire1->dy = 0;
                break;
            case 'R':
                // puts("R");
                wire1->dx = 0;
                wire1->dy = 1;
                break;
            case 'D':
                // puts("D");
                wire1->dx = -1;
                wire1->dy = 0;
                break;
            case 'L':
                // puts("L");
                wire1->dx = 0;
                wire1->dy = -1;
                break;
            case EOF:
                running = false;
                break;
            default:
                printf("Unknown direction %c", next_direction);
                break;
            }
            bool ignored;
            wire1->remaining = read_int(f1, ',', &ignored);
        }
        wire1->pos->x += wire1->dx;
        wire1->pos->y += wire1->dy;
        wire1->remaining--;
        //printf("Step at %d, %d\n", wire1->pos->x, wire1->pos->y);

        arraylist_add(trail1, wire1->pos);

    }

    //printf("Ended at %d, %d\n", wire1->pos->x, wire1->pos->y);

    running = true;
    int min_distance = INT_MAX;

    while (running) {
        if (wire2->remaining <= 0) {
            int next_direction = fgetc(f2);
            switch (next_direction) {
            case 'U':
                // puts("U");
                wire2->dx = 1;
                wire2->dy = 0;
                break;
            case 'R':
                // puts("R");
                wire2->dx = 0;
                wire2->dy = 1;
                break;
            case 'D':
                // puts("D");
                wire2->dx = -1;
                wire2->dy = 0;
                break;
            case 'L':
                // puts("L");
                wire2->dx = 0;
                wire2->dy = -1;
                break;
            case EOF:
                running = false;
                break;
            default:
                printf("Unknown direction %c", next_direction);
                break;
            }
            bool ignored;
            wire2->remaining = read_int(f2, ',', &ignored);
        }
        wire2->pos->x += wire2->dx;
        wire2->pos->y += wire2->dy;
        wire2->remaining--;

        //printf("Step at %d, %d\n", wire2->pos->x, wire2->pos->y);

        if(arraylist_contains(trail1, wire2->pos) != -1) {
            if (abs(wire2->pos->x) + abs(wire2->pos->y) < min_distance) {
                min_distance = abs(wire2->pos->x) + abs(wire2->pos->y);
            }
        }
    }

    printf("result = %d\n", min_distance);

    arraylist_free(trail1);
}
