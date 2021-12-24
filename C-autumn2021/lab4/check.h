int checkArgc(int argc) {
    if (argc < 3) {
        printf("! Wrong input !\n"
               "Right format:\n"
               "app.exe *filepath* *option and args*\n");
        return 0;
    }
    return 1;
}

void wrongOptionWarning() {
    printf("! Wrong option format !\n"
           "Input --show for entire metadata info\n"
           "Input --set=prop_name --value=prop_value for editing field\n"
           "Input --get=prop_name for the certain field info\n");
}

void rewrite(FILE *file1, FILE *file2) {
    char buffer[256];
    size_t read;
    while ((read = fread(buffer, sizeof(char), sizeof(buffer), file1))) {
        fwrite(buffer, sizeof(char), read, file2);
    }
}