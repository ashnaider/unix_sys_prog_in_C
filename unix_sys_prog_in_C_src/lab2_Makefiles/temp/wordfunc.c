#include "wordfunc.h"

#include <stdio.h>

void custom_func(char** words, int word_size) {
    char* value_pos;
    int i = 0;

    while (words[i] != NULL) {

        if (custom_condition(words[i], word_size, &value_pos)) {
            printf("%s\n", value_pos);
        }

        ++i;
    }
}

bool custom_condition(char* word, int word_size, char** value_pos) {

    char* curr = word;
    int count = 0;

    while (*curr != '\n' && *curr != '\0' && *curr != '=') {
        ++count;
        ++curr;
    }

    if (count == word_size) {

        *value_pos = ++curr;
        return true;
    }

    return false;
}
