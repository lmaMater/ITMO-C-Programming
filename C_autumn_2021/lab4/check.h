int checkArgc(int argc) {
    if (argc < 3){
        printf("! Wrong input !\n"
               "Right format:\n"
               "app.exe *filepath* *option and args*\n");
        return 0;
    }
    return 1;
}

void wrongOptionWarning() {
    printf("! Wrong option format !\n"
           "Input show for entire metadata info\n"
           "Input set prop_name prop_value for editing field\n"
           "Input get prop_name for the certain field info\n");
}