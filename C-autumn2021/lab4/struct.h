typedef struct Header {
    FILE *file;
    char *data;
    int size;       // size of data except of header
    int ext_header; // extended header byte
} Header;

typedef struct Frame {
    char *header;   // header of frame
    int size;       // size of entire frame
    char *data;
    char encoding;  // extra byte of encoding
    int shift;      // number of info flags
} Frame;

int toDec(char *data, int begin, int end) {
    int out = 0;
    int buffer = 128 * 128 * 128;
    for (int i = begin; i < end; i++) {
        out += buffer * ((int) data[i] % 128);
        buffer /= 128;
    }
    return out;
}

int countInfoFlags(char flag) {
    int out = 0;
    if (((int) flag & 2) == 1) out++;
    if (((int) flag & 32) == 1) out++;
    if (((int) flag & 128) == 1) out++;
    return out;
}

int isFrameHeader(char *str, int begin, int end) {
    for (int i = begin; i < end; i++) {
        if (!((int) str[i] >= (int) 'A' &&
              (int) str[i] <= (int) 'Z' ||
              (int) str[i] >= (int) '0' &&
              (int) str[i] <= (int) '9')) {
            return 0;
        }
    }
    return 1;
}

int isTextFrameHeader(char *str) {
    char *symbols = "TUCW";
    for (int i = 0; i < strlen(symbols); i++) {
        if (str[0] == symbols[i]) {
            return 1;
        }
    }
    return 0;
}

int isPaddingHeader(char *str, int begin, int end) {
    for (int i = begin; i < end; i++) {
        if (!((int) str[i] == 0)) {
            return 0;
        }
    }
    return 1;
}

void print_line() {
    printf("____________________________\n");
}
