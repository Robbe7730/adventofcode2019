#include "../helpers/input_helper.h"
#include "../helpers/arraylist.h"
#include "../helpers/memory.h"

typedef struct coord_struct {
    float x;
    float y;
} coord;

bool compare_coord(coord* a, coord* b) {
    //printf("\t\t%f,%f and %f,%f\n", a->x, a->y, b->x, b->y);
    return (a->x == b->x) && (a->y == b->y);
}

int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage %s <filename>", argv[0]);
    }

    FILE* f = open_file(argv[1]);

    arraylist* positions = arraylist_new(sizeof(coord));

    char curr_char;
    float x = 1;
    float y = 1;
    while ((curr_char = fgetc(f)) != EOF) {
        if (curr_char == '#') {
            coord* new_coord = checked_malloc(sizeof(coord));
            new_coord->x = x;
            new_coord->y = y;
            arraylist_add(positions, new_coord);
        } else if(curr_char == '\n') {
            y++;
            x = 0;
        }
        x++;
    }

    float max_x = 0;
    float max_y = 0;
    int max_score = 0;
    int score = 0;

    for (int i = 0; i < positions->num_values; i++) {
        coord* curr_coord = checked_malloc(sizeof(coord));
        arraylist_get(positions, i, curr_coord);

        arraylist* directions = arraylist_new(sizeof(coord));
        coord* checking_coord = checked_malloc(sizeof(coord));
        coord* checking_coord_direction = checked_malloc(sizeof(coord));
        score = 0;

        //printf("Angles for %f, %f\n", curr_coord->x, curr_coord->y);
        for (int j = 0; j < positions->num_values; j++) {
            arraylist_get(positions, j, checking_coord);
            float max = ((checking_coord->x - curr_coord->x) > (checking_coord->y - curr_coord->y)) ? (checking_coord->x - curr_coord->x) : (checking_coord->y - curr_coord->y);

            if (checking_coord->x == curr_coord->x) {
                checking_coord_direction->x = 0;
            } else {
                checking_coord_direction->x = (float) (checking_coord->x - curr_coord->x) / max;
            }

            if (checking_coord->y == curr_coord->y) {
                checking_coord_direction->y = 0;
            } else {
                checking_coord_direction->y = (float) (checking_coord->y - curr_coord->y) / max;
            }

            //printf("%f, %f\n", checking_coord_direction->x, checking_coord_direction->y);

            if (arraylist_contains_comparator(directions, checking_coord_direction, compare_coord) == -1) {
                arraylist_add(directions, checking_coord_direction);
                score++;
            }
        }

        if (score > max_score) {
            max_x = curr_coord->x;
            max_y = curr_coord->y;
            max_score = score;
        }
    }

    printf("result = %f,%f -> %d\n", max_x-1, max_y-1, max_score+1);
}
