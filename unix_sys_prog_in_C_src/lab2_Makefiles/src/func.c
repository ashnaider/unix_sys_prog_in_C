#include "func.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

char** split(char* string, char* delimiters)
{
    char* pch;
    char** result;
    pch = strtok(string, delimiters);

    int n = 0, delim_len = strlen(delimiters);
    int str_len = strlen(string);
    char curr;
    for (int i = 0; i < str_len; ++i) {
        curr = string[i];

        for (int j = 0; j < delim_len; ++j) {
            if (curr == delimiters[j]) {
                ++n;
            }
        }
    }

    long unsigned size = sizeof(char*);
    result = malloc(size * n);

    int i = 0;
    while (pch != NULL)
    {
        result[i] = pch;
        pch = strtok (NULL, delimiters);
        i++;
        /* size+=sizeof(char *); */
        /* result = realloc(result, size); */
    }

    result[i] = NULL;
    return result;
}
