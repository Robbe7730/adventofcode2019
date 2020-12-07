#include "../helpers/input_helper.h"
#include "../helpers/memory.h"
#include "../helpers/hashmap.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>

#define IMAGE_WIDTH 25
#define IMAGE_HEIGHT 6

// #define IMAGE_WIDTH 2
// #define IMAGE_HEIGHT 2

#define CC -1
#define EOI -2

void write_headers(FILE* file) {
    fputs("GIF89a", file);
    short width = IMAGE_WIDTH;
    short height = IMAGE_HEIGHT;
    char packed_field = 0b10010001;
    char background_color_index = 0;
    char pixel_aspect_ratio = 0;
    char global_color_table[12] = {
        0, 0, 0,
        255, 255, 255,
        255, 0, 0,
        255, 127, 0
    };

    fwrite(&width, sizeof(short), 1, file);
    fwrite(&height, sizeof(short), 1, file);
    fwrite(&packed_field, sizeof(char), 1, file);
    fwrite(&background_color_index, sizeof(char), 1, file);
    fwrite(&pixel_aspect_ratio, sizeof(char), 1, file);
    fwrite(&global_color_table, sizeof(char), 12, file);

    char gce_introducer = 0x21;
    char gce_label = 0xF9;
    char gce_block_size = 0x04;
    char gce_packed_field = 0b00000000; // lsb is transparant
    short gce_delay_time = 0;
    char gce_transparant_color_index = 0;
    char gce_block_terminator = 0;

    fwrite(&gce_introducer, sizeof(char), 1, file);
    fwrite(&gce_label, sizeof(char), 1, file);
    fwrite(&gce_block_size, sizeof(char), 1, file);
    fwrite(&gce_packed_field, sizeof(char), 1, file);
    fwrite(&gce_delay_time, sizeof(short), 1, file);
    fwrite(&gce_transparant_color_index, sizeof(char), 1, file);
    fwrite(&gce_block_terminator, sizeof(char), 1, file);
}

void lzw_encode(FILE* file, char* data) {
    hashmap* code_table = hashmap_new(sizeof(long long int), sizeof(char));

    char curr_token = 0;

    long long int curr_key = 0;
    hashmap_add(code_table, &curr_key, &curr_token);
    curr_token++;

    curr_key = 1;
    hashmap_add(code_table, &curr_key, &curr_token);
    curr_token++;

    curr_key = 2;
    hashmap_add(code_table, &curr_key, &curr_token);
    curr_token++;

    curr_key = 3;
    hashmap_add(code_table, &curr_key, &curr_token);
    curr_token++;

    curr_key = CC;
    hashmap_add(code_table, &curr_key, &curr_token);
    curr_token++;

    curr_key = EOI;
    hashmap_add(code_table, &curr_key, &curr_token);
    curr_token++;

    long long int buffer = CC;

    char* write_buffer = checked_calloc(IMAGE_HEIGHT * IMAGE_WIDTH, sizeof(char));
    int write_buffer_i = 0;
    char write_buffer_cache = 4;
    int cache_filled = 3;
    int code_size = 3;
    int buffer_size = 3;

    curr_token++;


    for (int i = 0; i < IMAGE_HEIGHT*IMAGE_WIDTH; i++){
        char next_data = data[i] + 1;
        long long int query_key = (buffer << 3) | (next_data);
        hashmap_entry* result = hashmap_query(code_table, &query_key);
        if (result != NULL) {
            printf("%lld is already a token\n", query_key);
            buffer = query_key;
            buffer_size++;
        } else {
            hashmap_add(code_table, &query_key, &curr_token);
            curr_token++;

            hashmap_entry* smaller_result = hashmap_query(code_table, &buffer);
            printf("%lld is not a token, outputting %d\n", query_key, *(char*) smaller_result->value);

            char new_value = *(char*)smaller_result->value;

            for (int bit = 0; bit < code_size; bit++) {
                write_buffer_cache = write_buffer_cache | ((new_value & 1) << cache_filled);
                printf("Cache is now %x, new_value = %x, written %d\n", write_buffer_cache, new_value, new_value & 1);
                new_value = new_value >> 1;
                cache_filled++;
                if (cache_filled == 8) {
                    printf("Cache is full, writing %x\n", write_buffer_cache);
                    write_buffer[write_buffer_i] = write_buffer_cache;
                    write_buffer_i++;
                    cache_filled = 0;
                    write_buffer_cache = 0;
                }
            }

            if (curr_token == ((1 << (code_size)))) {
                code_size++;
            }

            buffer = next_data;
            buffer_size = code_size;
        }
    }

    printf("Writing remaining buffer %d\n", buffer);

    hashmap_entry* result = hashmap_query(code_table, &buffer);
    char new_value = *(char*)result->value;
    printf(" as key %d\n", buffer, new_value);
    for (int bit = 0; bit < code_size; bit++) {
        write_buffer_cache = write_buffer_cache | ((new_value & 1) << cache_filled);
        printf("Cache is now %x, new_value = %x, written %d\n", write_buffer_cache, new_value, new_value & 1);
        new_value = new_value >> 1;
        cache_filled++;
        if (cache_filled == 8) {
            printf("Cache is full, writing %x\n", write_buffer_cache);
            write_buffer[write_buffer_i] = write_buffer_cache;
            write_buffer_i++;
            cache_filled = 0;
            write_buffer_cache = 0;
        }
    }

    printf("Writing EOI\n");
    new_value = 5;
    for (int bit = 0; bit < code_size; bit++) {
        write_buffer_cache = write_buffer_cache | ((new_value & 1) << cache_filled);
        printf("Cache is now %x, new_value = %x, written %d\n", write_buffer_cache, new_value, new_value & 1);
        new_value = new_value >> 1;
        cache_filled++;
        if (cache_filled == 8) {
            printf("Cache is full, writing %x\n", write_buffer_cache);
            write_buffer[write_buffer_i] = write_buffer_cache;
            write_buffer_i++;
            cache_filled = 0;
            write_buffer_cache = 0;
        }
    }

    printf("Data is done, writing remainder: %x\n", write_buffer_cache);
    write_buffer[write_buffer_i] = write_buffer_cache;
    write_buffer_i++;

    printf("Writing # of bytes: %d\n", write_buffer_i),
    fwrite(&write_buffer_i, sizeof(char), 1, file);

    printf("Writing entire buffer\n"),
    fwrite(write_buffer, sizeof(char), write_buffer_i, file);

    char block_terminator = 0;

    printf("Writing block terminator: %d\n", block_terminator),
    fwrite(&block_terminator, sizeof(char), 1, file);
}

void draw_image(FILE* file, char* data) {
    char image_separator = 0x2c;
    short image_left = 0;
    short image_top = 0;
    short image_width = IMAGE_WIDTH;
    short image_height = IMAGE_HEIGHT;
    char image_packed_field = 0b00000000;

    fwrite(&image_separator, sizeof(char), 1, file);
    fwrite(&image_left, sizeof(short), 1, file);
    fwrite(&image_top, sizeof(short), 1, file);
    fwrite(&image_width, sizeof(short), 1, file);
    fwrite(&image_height, sizeof(short), 1, file);
    fwrite(&image_packed_field, sizeof(char), 1, file);

    char lzw_minimum_code_size = 2;
    fwrite(&lzw_minimum_code_size, sizeof(char), 1, file);
    lzw_encode(file, data);
}

bool process_layer(FILE* f, int i) {
    char curr_char;

    char* filename = checked_calloc(26, sizeof(char));
    sprintf(filename, "day08/images/layer%d.gif", i);
    FILE* gif_file = fopen(filename, "we");

    if (gif_file == NULL) {
        fprintf(stderr, "Could not open file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    write_headers(gif_file);

    char* data = checked_calloc(IMAGE_HEIGHT * IMAGE_WIDTH, sizeof(char));

    for (int i = 0; i < IMAGE_HEIGHT * IMAGE_WIDTH; i++) {
        curr_char = getc(f);
        if (curr_char == EOF) {
            return false;
        }
        if (curr_char == '0') {
            data[i] = 0;
        } else if (curr_char == '1') {
            data[i] = 1;
        } else if (curr_char == '2') {
            data[i] = 2;
        } else {
            data[i] = 3;
        }
    }
    char my_data[14] = {1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 1, 1, 1, 1};
    draw_image(gif_file, my_data);
    write_trailer(gif_file);
    return true;
}

void write_trailer(FILE* gif_file) {
    char write_hold = 0x3b;
    fwrite(&write_hold, sizeof(char), 1, gif_file);
}

int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    FILE* f = open_file(argv[1]);

    int i = 0;
    process_layer(f, i);

    printf("done\n");
}