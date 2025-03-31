#include "int2str.h"
#include <assert.h>
#include <stdio.h>

char* int2str(int number) {
    static char buffer[12];
    int i = 0;
    int is_negative = 0;

    if (number == 0) {
        buffer[0] = '0';
        buffer[1] = '\0';
        return buffer;
    }

    if (number < 0) {
        is_negative = 1;
        if (number == -2147483648) {
            buffer[0] = '-'; buffer[1] = '2'; buffer[2] = '1';
            buffer[3] = '4'; buffer[4] = '7'; buffer[5] = '4';
            buffer[6] = '8'; buffer[7] = '3'; buffer[8] = '6';
            buffer[9] = '4'; buffer[10] = '8'; buffer[11] = '\0';
            return buffer;
        }
        number = -number;
        buffer[i++] = '-';
    }

    int len = 0;
    int temp = number;
    while (temp > 0) {
        len++;
        temp /= 10;
    }

    int index = i + len;
    buffer[index] = '\0';
    while (number > 0) {
        buffer[--index] = '0' + (number % 10);
        number /= 10;
    }

    return buffer;
}