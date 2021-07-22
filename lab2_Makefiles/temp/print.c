#include <stdio.h>

#include "print.h"


void printwords(char** words)
{
    int i = 0;
    while(words[i] != NULL)
        printf("--- %s\n", words[i++]);
}
