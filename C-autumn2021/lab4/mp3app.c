#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "check.h"
#include "struct.h"

       __
      /**\
      \= / funny
______/  \_ man__
_____       _____
    |{.}{.}|     
int main(int argc, char *argv[]) {
    if (!(checkArgc(argc))) {
        return 1;
    }

    Header header;
    if ((header.file = fopen(argv[1], "rwb")) == NULL) {
        printf("! Wrong file format !\n");
        return 1;
    }

    //mode: 0 - show, 1 - get, 2 - set;
    int mode;
    if (argc == 3 && strcmp(argv[2], "show") == 0) {
        mode = 0;
    } else if (argc == 4 && strcmp(argv[2], "get") == 0) {
        mode = 1;
    } else if (argc == 5 && strcmp(argv[2], "set") == 0) {
        mode = 2;
    } else {
        wrongOptionWarning();
        return 1;
    }
    //printf("mode: %d\n", mode);

    // reading header
    header.data = malloc(10 * sizeof(char));
    fgets(header.data, 10 * sizeof(char) + 1, header.file);
    header.size = toDec(header.data, 6, 10);
    int temp_size = header.size;
    header.ext_header = (int) header.data[5] & 64;
    //printf("ext_header: %d\n", header.ext_header);

    // frames
    while (temp_size > 0) {
        Frame frame;
        frame.header = malloc(10 * sizeof(char));
        fgets(frame.header, 10 * sizeof(char) + 1, header.file);

        if (isFrameHeader(frame.header, 0, 4)) {
            frame.size = toDec(frame.header, 4, 8);
            frame.shift = countInfoFlags(frame.header[9]);
            fseek(header.file, frame.shift * sizeof(char), SEEK_CUR);
            int data_len = frame.size - frame.shift;

            // Wxxx-type has no extra encoding byte
            if (frame.header[0] != 'W') {
                frame.encoding = fgetc(header.file);
                data_len -= 1;
            } else {
                frame.encoding = (char) 3;
            }

            frame.data = malloc((data_len) * sizeof(char));
            fgets(frame.data, (data_len) * sizeof(char) + 1, header.file);

            if (isTextFrameHeader(frame.header)) {
                if (mode == 0 || mode == 1 && strncmp(argv[3], frame.header, 4) == 0) {
                    print_line();
                    for (int i = 0; i < 4; i++) {
                        printf("%c", frame.header[i]);
                    }
                    printf(": ");
                    //printf("Info flags: %d\n", frame.shift);
                    //printf("Encoding: %d\n", (int) frame.encoding);
                    printf("%s\n", frame.data);
                    if (mode == 1 && strncmp(argv[3], frame.header, 4) == 0) {
                        break;
                    }
                } else if (mode == 2 && strncmp(argv[3], frame.header, 4) == 0) {
                    // fseek(header.file, -frame.size + 4, SEEK_CUR);
                    // записать 4 байта нового размера
                    //          2 байта флагов
                    //          prop_value, а затем склеить с началом следующего фрейма
                    // ? fputs() ?
                    break;
                }
            } else if (isPaddingHeader(frame.header, 0, 4)) {
                break;
            }
        } else {
            break;
        }
        temp_size -= frame.size;
    }

    return 0;
}
