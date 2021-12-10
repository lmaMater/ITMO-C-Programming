int is_alpha(char current_char) {
    int char_code = (int) (current_char);
    if (char_code >= (int) ('a') && char_code <= (int) ('z')) {
        return 1;
    }
    if (char_code >= (int) ('A') && char_code <= (int) ('Z')) {
        return 1;
    }

    return 0;
}

int words(void *input_file) {
    char current_char;
    int words_counter = 0, in_word = 0;
    do {
        current_char = fgetc(input_file);
        if (is_alpha(current_char) == 1) {
            if (in_word == 0) {
                words_counter++;
                in_word = 1;
            }
        } else { in_word = 0; }
    } while (current_char != EOF);
    return words_counter;
}

int bytes(void *input_file) {
    char current_char;
    int bytes_sum = 0;
    do {
        current_char = fgetc(input_file);
        bytes_sum += sizeof(current_char);

    } while (current_char != EOF);
    return bytes_sum;
}

int lines(void *input_file) {
    char current_char;
    int lines_sum = 1;
    do {
        current_char = fgetc(input_file);
        if (current_char == '\n') { lines_sum++; }

    } while (current_char != EOF);
    return lines_sum;
}