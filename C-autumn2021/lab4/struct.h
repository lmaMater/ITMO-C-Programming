typedef struct Header {
    char *data;     // entire header data
    int size;       // size of data except of header
    int ext_header; // extended header byte
} Header;

typedef struct Frame {
    char *header;   // header of frame
    int size;       // size of entire frame
    char *data;     // only text frame data
    int encoding_byte;
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

void writeBytesToFrameHeader(int size, FILE *file) {
    // 0xFF
    int buf = 4278190080;
    for (int i = 0; i < 4; i++) {
        fputc((size & buf) >> (8 * (3 - i)), file);
        // 0x(FF 00 00 00) -> 0x(00 FF 00 00) -> ...
        buf = buf >> 8;
    }
}

void writeBytesToHeader(int size, FILE *file) {
    int buf = 266338304; //
    for (int i = 0; i < 4; i++) {
        fputc((size & buf) >> (7 * (3 - i)), file);
        buf = buf >> 7;
    }
}


int countInfoFlags(char flag) {
    int out = 0;
    // meaning 1, 5, 7 bits
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
    char *symbols = "TUCWEGILMORU";
    for (int i = 0; i < strlen(symbols); i++) {
        if (str[0] == symbols[i]) {
            return 1;
        }
    }
    return 0;
}

int hasNoExtraByte(char ch) {
    char *symbols = "W";
    for (int i = 0; i < strlen(symbols); i++) {
        if (ch == symbols[i]) {
            return 1;
        }
    }
    return 0;
}

void printLine() {
    printf("____________________________\n");
}

void readField(char *field, int size,
                FILE *file, FILE *duplicate_file, int need_copy) {
    for (int i = 0; i < sizeof(char) * size; i++) {
        int c = fgetc(file);
        field[i] = c;

        // need_copy == 1, then copying to duplicate_file
        if (need_copy) {
            fputc((char) c, duplicate_file);
        }
    }
}

void copyFieldToFile(char *field, int size, FILE *file) {
    for (int i = 0; i < size; i++) {
        fputc(field[i], file);
    }
}

void getRest(FILE *file1, FILE *file2) {
    int c;
    while (1) {
        c = fgetc(file1);
        if (c == EOF) {
            break;
        } else {
            fputc((char) c, file2);
        }
    }
}
