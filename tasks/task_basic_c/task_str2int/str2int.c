#include <stdio.h>

// Function to convert string to integer
int str2int(const char *string_value) {
    int result = 0;       // Final integer
    int i = 0;            // Index for string
    int is_negative = 0;  // Flag to check if the number is negative

    // Check for leading whitespace
    while (string_value[i] == ' ') {
        i++;
    }

    // Check for negative sign
    if (string_value[i] == '-') {
        is_negative = 1;  // Set negative flag
        i++;              // Move to the next character
    } else if (string_value[i] == '+') {
        i++;              // Move to the next character for positive
    }

    // Iterate through the string until we hit the null terminator
    while (string_value[i] != '\0') {
        // Ensure the character is a digit
        if (string_value[i] >= '0' && string_value[i] <= '9') {
            // Shift and accumulate the digits
            result = result * 10 + (string_value[i] - '0');
        } else {
            // If a non-digit character is encountered, break the loop
            break;
        }
        i++;  // Shift to the next character
    }

    // If number is negative
    if (is_negative) {
        result = -result;
    }

    return result;  // Return the final result
}

int main() {
    const char *input_string = "-aboba123aboba?_zxc_GHOUL_DEADINSIDE_4000MMR";  
    int result = str2int(input_string);
    printf("%d\n", result);  // Output
    return 0;
}