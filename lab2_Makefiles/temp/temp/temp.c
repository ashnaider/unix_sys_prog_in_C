#include <stdio.h>
#include <stdlib.h>

int main() {
    char* str = malloc(sizeof(char) * 5);

    str[0] = 'a';
    str[1] = 'a';
    str[2] = 'a';
    str[3] = 'a';

    printf("%s\n", str);

    free(str);

    return 0;
}
