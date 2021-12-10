/* CHECK */


int check_argc(int argc) {
    if (argc != 3) {
        printf("! Wrong input !\n");
        printf("Retry in this format:\n");
        printf("WordCount.exe *option* file_directory\n");
        return 1;
    }
    return 0;
}

int check_file(void *input_file) {
    if (input_file == NULL) {
        printf("Wrong file format\n");
        return 1;
    }
    return 0;
}

void wrong_option_warning() {
    printf("! Wrong option format !\n");
    printf("Input -l for number of lines\n");
    printf("Input -c for number of bytes\n");
    printf("Input -w for number of words\n");
}
