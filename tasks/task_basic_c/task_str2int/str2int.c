#include "str2int.h"
#include <assert.h>
#include "stdio.h"

int str2int(const char* str) {
    int sign = 1;
    long long result = 0;

    while (*str == ' ') {
        str++;
    }

    if (*str == '-') {
        sign = -1;
        str++;
    } else if (*str == '+') {
        str++;
    }

    while (*str >= '0' && *str <= '9') {
        result = result * 10 + (*str - '0');
        str++;

        if (sign == 1 && result > 2147483647) {
            return 2147483647;
        }
        if (sign == -1 && result > 2147483648LL) {
            return -2147483648;
        }
    }

    return (int)(sign * result);
}