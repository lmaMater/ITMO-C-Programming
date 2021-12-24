#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "check.h"
#include "struct.h"


int main(int argc, char *argv[]) {
    if (!(checkArgc(argc))) {
        return 1;
    }
    Header header;

    FILE *file;
    if ((file = fopen(argv[1], "rwb")) == NULL) {
        printf("! Wrong file format !\n");
        return 1;
    }
    // duplicate file
    FILE *duplicate = fopen("duplicate.mp3", "wb");

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

    // reading header
    header.data = malloc(10 * sizeof(char));
    readField(header.data, 10, file, duplicate, 1);

    header.size = toDec(header.data, 6, 10);
    int temp_size = header.size;
    header.ext_header = (int) header.data[5] & 64;

    // reading frames
    while (temp_size > 0) {
        Frame frame;

        frame.header = malloc(10 * sizeof(char));
        readField(frame.header, 10, file, duplicate, 1);

        // won't consider padding
        if (isFrameHeader(frame.header, 0, 4)) {
            // filling fields of frame
            frame.size = toDec(frame.header, 4, 8);
            frame.shift = countInfoFlags(frame.header[9]);
            for (int i = 0; i < frame.shift; i++) {
                int c = fgetc(file);
                fputc((char) c, duplicate);
            }

            int data_len;
            data_len = frame.size - frame.shift;

            // for example, Wxxx-type has no extra encoding byte
            if (hasNoExtraByte(frame.header[0]) == 1) {
                frame.encoding = 3;
                frame.encoding_byte = 0;
            } else {
                frame.encoding = fgetc(file);
                fputc(frame.encoding, duplicate);
                frame.encoding_byte = 1;
                data_len--;
            }

            frame.data = malloc((data_len) * sizeof(char));
            readField(frame.data, data_len, file, duplicate, 0);

            // show only for editable text frames
            if (isTextFrameHeader(frame.header)) {
                // "show" or "get" requested tag
                if (mode == 0 || mode == 1 && strncmp(argv[3], frame.header, 4) == 0) {
                    // print line and tag
                    printLine();
                    for (int i = 0; i < 4; i++) {
                        printf("%c", frame.header[i]);
                    }
                    printf(": ");

                    // print tag
                    for (int i = 0; i < data_len; i++) {
                        printf("%c", frame.data[i]);
                    }
                    printf("\n");
                } else if (mode == 2 && strncmp(argv[3], frame.header, 4) == 0) {
                    // editing frame header size bytes

                    // seek: (optional) meaning flags + 2 flags + 4 size + (optional) 1 encoding
                    fseek(duplicate, -((frame.shift + 6 + frame.encoding_byte) * sizeof(char)), SEEK_CUR);
                    writeBytesToFrameHeader(strlen(argv[4]) + frame.shift + frame.encoding_byte, duplicate);
                    // skip 2 bytes of flags
                    fseek(duplicate, (2 + frame.shift + frame.encoding_byte) * sizeof(char), SEEK_CUR);

                    // editing general header size bytes
                    int cur_pos = ftell(duplicate);
                    fseek(duplicate, 6, SEEK_SET);
                    writeBytesToHeader(header.size + strlen(argv[4]) - data_len, duplicate);
                    fseek(duplicate, cur_pos, SEEK_SET);

                    // prop_value -> frame.data
                    data_len = strlen(argv[4]);
                    frame.data = malloc(data_len);
                    frame.data = argv[4];
                }
                // copying to duplicate.mp3
                copyFieldToFile(frame.data, data_len, duplicate);
                free(frame.header);
                free(frame.data);
            } else {
                copyFieldToFile(frame.data, data_len, duplicate);
            }
        } else {
            getRest(file, duplicate);
            break;
        }
        // adding 10 bytes of frame header
        temp_size -= (frame.size + 10);
    }

    free(header.data);
    fclose(file);
    fclose(duplicate);

    // rewriting duplicate to original file
    if ((duplicate = fopen("duplicate.mp3", "rb")) == NULL) {
        printf("!Cannot open duplicate file to write!!\n");
        return 1;
    }
    if ((file = fopen(argv[1], "wb")) == NULL) {
        printf("! Cannot open source file to write!\n");
        return 1;
    }
    rewrite(duplicate, file);
    fclose(duplicate);
    fclose(file);

    //remove("duplicate.mp3");
    return 0;
}
