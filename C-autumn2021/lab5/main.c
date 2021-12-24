#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct.h"
#include "check.h"


int main(int argc, char *argv[]) {
    if (!checkArgc(argc)) {
        return 1;
    }

    char *input = NULL;
    char *output = NULL;
    int max_iter = 999999;
    int dump_freq = 1;

    // parameters initialization
    for (int i = 1; i < argc; i += 2) {
        if (strcmp(argv[i], "--input") == 0) {
            input = argv[i + 1];
        } else if (strcmp(argv[i], "--output") == 0) {
            output = argv[i + 1];
        } else if (strcmp(argv[i], "--max_iter") == 0) {
            max_iter = atoi(argv[i + 1]);
        } else if (strcmp(argv[i], "--dump_freq") == 0) {
            dump_freq = atoi(argv[i + 1]);
        } else {
            printf("! Wrong arguments !");
            return 1;
        }
    }

    FILE *file;
    if ((file = fopen(input, "rb")) == NULL) {
        printf("! Wrong file format !\n");
        return 1;
    }

    // struct initialization
    BMP bmp;
    bmp.bm_file_header = malloc(14 * sizeof(char));
    readField(bmp.bm_file_header, 14, file);

    bmp.dib_header = malloc(40 * sizeof(char));
    readField(bmp.dib_header, 40, file);

    bmp.file_size = toDec(bmp.bm_file_header, 2, 6);
    bmp.offset = toDec(bmp.bm_file_header, 10, 14);
    bmp.width = toDec(bmp.dib_header, 4, 8);
    bmp.height = toDec(bmp.dib_header, 8, 12);

    int till_pixels = bmp.offset - (14 + 40);
    bmp.rest_till_pixels = malloc(till_pixels * sizeof(char));
    readField(bmp.rest_till_pixels, till_pixels, file);

    int pixel_table_size = 3 * (bmp.file_size - bmp.offset);
    bmp.pixel_table = malloc(pixel_table_size);
    readField(bmp.pixel_table, pixel_table_size, file);

    fclose(file);

    char *current_pixel_table;

    current_pixel_table = malloc(pixel_table_size);
    memcpy(current_pixel_table, bmp.pixel_table, pixel_table_size);

    for (int generation = 1; generation <= max_iter; generation++) {
        char *updated_pixel_table;
        updated_pixel_table = malloc(pixel_table_size);
        int same_pixels = 0;
        for (int i = 0; i < bmp.height; i++) {
            for (int j = 0; j < bmp.width; j++) {
                same_pixels += updateCell(i, j, bmp.width, bmp.height,
                                          current_pixel_table, updated_pixel_table);
            }
        }

        if (generation % dump_freq == 0) {
            // [dir_name]/generation[out_counter].bmp
            char *cur = malloc(strlen(output) + countDigits(generation)
                               + 8);
            sprintf(cur, "%s/lab%d.bmp", output, generation);
            //printf("%s", cur);
            FILE *updated;
            updated = fopen(cur, "wb");

            copyField(bmp.bm_file_header, 14, updated);
            copyField(bmp.dib_header, 40, updated);
            copyField(bmp.rest_till_pixels, till_pixels, updated);
            copyField(updated_pixel_table, pixel_table_size, updated);
            fclose(updated);
        }
        memcpy(current_pixel_table, updated_pixel_table, pixel_table_size);
        free(updated_pixel_table);

        if (same_pixels == bmp.width * bmp.height) {
            printf("! Static picture !");
            break;
        }
    }

    free(bmp.pixel_table);
    free(bmp.bm_file_header);
    free(bmp.dib_header);
    free(bmp.rest_till_pixels);
    free(input);
    free(output);

    return 0;
}