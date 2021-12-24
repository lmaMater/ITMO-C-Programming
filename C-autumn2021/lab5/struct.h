/* STRUCT */

const char *name = "generation";
const int WHITE = -1;   // #[ff][ff][ff]
const int BLACK = 0;    // #[00][00][00]

typedef struct {
    char *bm_file_header; /* 14 bytes
                             [2 bytes header field]
                             [4 bytes size of file]
                             [2 + 2 bytes reserved]
                             [4 bytes the offset  ]
                          */
    char *dib_header;     /* 40 bytes
                            [4 bytes size of header]
                            [4 bytes bitmap width  ]
                            [4 bytes bitmap height ]
                            [~2 bytes USELESS~     ]
                            [2 bytes bits per pixel]
                            [~4 bytes USELESS~     ]
                            [4 bytes image size    ]
                            [16 bytes colors       ]
                          */
    char *rest_till_pixels; // colors table etc.
    char *pixel_table;
    int file_size;
    int offset;
    int width;
    int height;
    int bits_per_pixel;
} BMP;

void readField(char *field, int size, FILE *file) {
    for (int i = 0; i < size * sizeof(char); i++) {
        int c = fgetc(file);
        field[i] = (char) c;

    }
}

void copyField(char *field, int size, FILE *file_to_copy) {
    for (int i = 0; i < size * sizeof(char); i++) {
        char c = field[i];
        fputc(c, file_to_copy);
    }
}

// Little Indian
int toDec(char *data, int begin, int end) {
    int out = 0;
    int buffer = 255, shift = 8;
    for (int i = begin; i < end; i++) {
        int cur = (int) data[i];
        out += (cur & buffer) << (shift * (i - begin));
    }
    return out;
}

void fillBytes(char *array, int index, int size, int value) {
    for (int i = 0; i < size; i++) {
        array[index + i] = (char) value;
    }
}

int countNeighbours(int row, int col, int w, int h,
                    char *array) {
    int counter = 0;

    // array indexes: l - left, r - right, u - upper, b - bottom
    int l = 3 * (row * w + (col - 1));
    int u = 3 * ((row + 1) * w + col);
    int r = 3 * (row * w + (col + 1));
    int b = 3 * ((row - 1) * w + col);
    int lb = 3 * ((row - 1) * w + (col - 1));
    int lu = 3 * ((row + 1) * w + (col - 1));
    int ru = 3 * ((row + 1) * w + (col + 1));
    int rb = 3 * ((row - 1) * w + (col + 1));

    if (col - 1 >= 0 && array[l] == BLACK) {
        counter++;
    }
    if (col + 1 < w && array[r] == BLACK) {
        counter++;
    }
    if (row - 1 >= 0 && array[b] == BLACK) {
        counter++;
    }
    if (row + 1 < h && array[u] == BLACK) {
        counter++;
    }
    if (col - 1 >= 0 && row - 1 >= 0 && array[lb] == BLACK) {
        counter++;
    }
    if (col - 1 >= 0 && row + 1 < h && array[lu] == BLACK) {
        counter++;
    }
    if (col + 1 < w && row + 1 < h && array[ru] == BLACK) {
        counter++;
    }
    if (col + 1 < w && row - 1 >= 0 && array[rb] == BLACK) {
        counter++;
    }
    return counter;
}

int isColor(char *array, int index, int color) {
    for (int i = 0; i < 3; i++) {
        if (array[index] != color) {
            return 0;
        }
    }
    return 1;
}

int updateCell(int row, int col, int w, int h,
               char *array, char *updated_array) {
    int counter = countNeighbours(row, col, w, h, array);
    int index = 3 * (row * w + col);

    if (isColor(array, index, WHITE)) {
        if (counter == 3) {
            fillBytes(updated_array, index, 3, BLACK);
            return 0;
        } else {
            fillBytes(updated_array, index, 3, WHITE);
            return 1;
        }
    } else {
        if (!(counter == 2 || counter == 3)) {
            fillBytes(updated_array, index, 3, WHITE);
            return 0;
        } else {
            fillBytes(updated_array, index, 3, BLACK);
            return 1;
        }
    }
}

int countDigits(int n) {
    int counter = 0;
    while (n > 0) {
        n /= 10;
        counter++;
    }
    return counter;
}