#include <stdio.h>

// This function is called by your assembly code.
void display(const char *str) {
    printf("%s\n", str);
}

// This function is also called by your assembly code.
extern int my_program();

int main() {
    int result = my_program();
    printf("Program returned: %d\n", result);
    return 0;
}