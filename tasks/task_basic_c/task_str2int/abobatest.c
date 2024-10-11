#include <assert.h> 
#include "str2int.h" 
#include "stdio.h" 
 
int str2int(const char *str) { 
    int num = 0; 
    int sign = 1; 
    int i = 0; 
 
    // Check for negative sign 
    if (str[0] == '-') { 
        sign = -1; 
        i++; 
    } 
 
    // Iterate through the string and convert characters to integer 
    while (str[i] != '\0') { 
        assert(str[i] >= '0' && str[i] <= '9'); // Check if character is a digit 
        num = num * 10 + (str[i] - '0'); 
        i++; 
    } 
 
    return sign * num; 
}