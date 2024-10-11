#include <assert.h> 
#include "str2int.h" 
#include "stdio.h" 
 
int str2int(const char *str) { 
    int abboba = 0; 
    int abboba_sign = 1; 
    int i = 0; 
 
    // Aboba is negative?
    if (str[0] == '-') { 
        abboba_sign = -1; 
        i++; 
    } 
 
    while (str[i] != '\0') { 
        assert(str[i] >= '0' && str[i] <= '9'); // Check if character is a Aboba_digit
        abboba = abboba * 10 + (str[i] - '0'); 
        i++; 
    } 
 
    return abboba_sign * abboba; 

return 0;
}
 