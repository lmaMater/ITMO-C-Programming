#include <stdio.h>
#include <string.h>
#include "options.h"
#include "check.h"

int main(int argc, char *argv[]) {
    if (check_argc(argc) == 1) {
        return 1;
    }

    FILE *input_file = fopen(argv[argc - 1], "r");
    if (check_file(input_file) == 1) {
        return 1;
    }

    if (strcmp(argv[1], "-l") == 0) {
        printf("number of lines: %d\n", lines(input_file));
    } else if (strcmp(argv[1], "-c") == 0) {
        printf("number of bytes: %d\n", bytes(input_file));
    } else if (strcmp(argv[1], "-w") == 0) {
        printf("number of words: %d\n", words(input_file));
    } else {
        wrong_option_warning();
        return 1;
    }

    fclose(input_file);

    return 0;
}
