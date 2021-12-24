/* HELPERS */

const int BUF_SIZE = 1024;

int ToSeconds(int days, int hours, int minutes, int seconds) {
    return seconds + minutes * 60 + hours * 3600 + days * 86400;
}

int stringToSeconds(char *str) {
    int day, hour, min, sec;
    for (int i = 0; i < BUF_SIZE; i++) {
        if (str[i] == '[') {
            int x = 0;
            x += i;
            char first[2] = {str[i + 1]}, second[2] = {str[i + 2]};
            strcat(first, second);
            i += x;
            char first2[2] = {str[i + 13]}, second2[2] = {str[i + 14]};
            strcat(first2, second2);
            i += x;
            char first3[2] = {str[i + 16]}, second3[2] = {str[i + 17]};
            strcat(first3, second3);
            i += x;
            char first4[2] = {str[i + 19]}, second4[2] = {str[i + 20]};
            strcat(first4, second4);
            day = atoi(first);
            hour = atoi(first2);
            min = atoi(first3);
            sec = atoi(first4);
            break;
        }
    }
    ToSeconds(day, hour, min, sec);
}
