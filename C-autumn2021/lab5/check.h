/* CHECK */

int checkArgc(int argc) {
    if (argc < 5 || (argc % 2) == 0) {
        printf("Wrong number of args");
        return 0;
    }
    return 1;
}