#include <stdio.h>

int main() {

    int a = 5, b = 10, result = 0;
    __asm__("movl %1, %%eax;"
            "addl %2, %%eax;"
            "movl %%eax, %0;"
            : "=r"(result)
            : "r"(a), "r"(b)
            : "%eax");
    printf("result = %d\n", result);

    return 0;
}