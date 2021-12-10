#include <stdio.h>
#include <inttypes.h>
#include <string.h>


const uint64_t base = 4294967296;


typedef struct uint {
    unsigned int elements[32];
} uint1024_t;


uint1024_t from_uint(unsigned int x) {
    uint1024_t result;

    result.elements[0] = x;
    for (int i = 1; i < 32; i++) {
        result.elements[i] = 0;
    }
    return result;
}


uint1024_t add_op(uint1024_t x, uint1024_t y) {
    uint1024_t result;
    int buffer = 0;

    for (int i = 0; i < 32; i++) {
        uint64_t num_x = x.elements[i];
        uint64_t num_y = y.elements[i];
        result.elements[i] = (num_x + num_y + buffer) % base;
        buffer = (num_x + num_y + buffer) / base;
    }
    return result;
}


uint1024_t max(uint1024_t x, uint1024_t y) {

    for (int i = 31; i >= 0; i--) {
        if (x.elements[i] != y.elements[i]) {
            if (x.elements[i] > y.elements[i]) {
                return x;
            } else {
                return y;
            }
        }
    }
    return x;
}


int equal(uint1024_t x, uint1024_t y) {

    for (int i = 31; i >= 0; i--) {
        if (x.elements[i] != y.elements[i]) {
            return 0;
        }
    }
    return 1;
}


uint1024_t subtr_op(uint1024_t x, uint1024_t y) {

    if (equal(max(x, y), y) == 1) {
        uint1024_t tmp = x;
        x = y;
        y = tmp;
    }

    uint1024_t result;
    int buffer = 0;
    for (int i = 0; i < 32; i++) {
        uint64_t num_x = x.elements[i];
        uint64_t num_y = y.elements[i];

        if (num_x - buffer >= num_y) {
            result.elements[i] = num_x - buffer - num_y;
            buffer = 0;
        } else {
            result.elements[i] = num_x + base - num_y - buffer;
            buffer = 1;
        }
    }
    return result;
}


uint1024_t mult_op(uint1024_t x, uint1024_t y) {
    uint1024_t result = from_uint(0);

    for (int i = 0; i < 32; i++) {
        uint1024_t temp = from_uint(0);
        uint64_t buffer = 0;
        uint64_t num_y = y.elements[i];
        for (int j = 0; j < 32; j++) {
            uint64_t num_x = x.elements[j];
            temp.elements[j] = (num_x * num_y + buffer) % base;
            buffer = (num_x * num_y + buffer) / base;
        }
        for (int j = 0; j < i; j++) {
            for (int q = 31; q > 0; q--) {
                temp.elements[q] = temp.elements[q - 1];
            }
            temp.elements[0] = 0;
        }
        result = add_op(result, temp);
    }
    return result;
}


void scanf_value(uint1024_t *x) {
    char line[310];
    uint1024_t y = from_uint(0);
    scanf("%s", line);
    for (int i = 0; i < strlen(line); i++) {
        y = mult_op(y, from_uint(10));
        y = add_op(y, from_uint(line[i] - '0'));
    }
    for (int i = 0; i < 32; i++) {
        x->elements[i] = y.elements[i];
    }
}


void printf_value(uint1024_t x) {
    uint1024_t res = from_uint(0);
    uint1024_t null = from_uint(0);
    if (memcmp(&x, &null, sizeof(int) * 32) == 0) {
        printf("0");
        return;
    }
    char line[310];
    memset(line, '\0', 310 * sizeof(char));
    while (memcmp(&x, &null, sizeof(int) * 32) != 0) {
        uint64_t buffer = 0;
        for (int i = 31; i >= 0; i--) {
            uint64_t num_x = x.elements[i];
            uint64_t value = (num_x + buffer * base);
            res.elements[i] = value / 10;
            buffer = value % 10;
        }
        line[strlen(line)] = buffer + '0';
        x = res;
    }
    for (int i = strlen(line) - 1; i >= 0; i--) {
        printf("%c", line[i]);
    }
}


int main(int argc, char *argv[]) {
    uint1024_t x, y;
    scanf_value(&x);
    scanf_value(&y);

    printf("Sum   = ");
    printf_value(add_op(x, y));
    printf("\n");
    printf("Subtr = ");
    printf_value(subtr_op(x, y));
    printf("\n");
    printf("Mult  = ");
    printf_value(mult_op(x, y));
    printf("\n");

    return 0;
}