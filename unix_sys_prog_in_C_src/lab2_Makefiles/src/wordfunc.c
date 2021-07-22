#include "wordfunc.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DICT_SIZE 257

void first_func(char** words, int word_size) {
    /*
     * 3. Вывести поле «значение» для всех слов,
     * длина имен которых равна
     * заданному числу
     */
    char* value_pos;
    int i = 0;

    printf("----In first function----\n");
    while (words[i] != NULL) {

        if (first_condition(words[i], word_size, &value_pos)) {
            printf("%s\n", value_pos);
        }

        ++i;
    }
}

bool first_condition(char* word, int word_size, char** value_pos) {
    /*
     * Проверка, что длина 'длина' имени
     * равна заданному числу
     */

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


void second_func(char** words) {
    /*
     * 16.
     * Вывести поле «имя» для всех слов
     * в именах которых каждая буква
     * входит не менее двух раз
     */

    int i = 0;

    printf("----In second function----\n");
    while (words[i] != NULL) {
        char* curr = get_name(words[i]);

        if (second_condition(curr)) {
            printf("%s\n", curr);
        }

        if (curr != NULL) {
            free(curr);
        }

        ++i;
    }
}

char* get_value(char* word) {
    /* Get value from 'name=value' */
    char* curr = word;
    while (*curr != '\n' && *curr != '\0' && *curr != '=') {
        ++curr;
    }
    return ++curr;
}

char* get_name(char* word) {
/* Get name from 'name=value' */

    char* curr = word;

    while (*curr != '\n' && *curr != '\0' && *curr != '=') {
        ++curr;
    }

    int name_length = curr - word;
    size_t name_mem_size = sizeof(char) * name_length;
    char* res = malloc(name_mem_size);
    memcpy(res, word, name_mem_size);
    res[name_length] = '\0';

    return res;
}

bool second_condition(char* word) {
    /*
     * проверка, что каждая буква
     * входит не менее двух раз
     */

    char letters[DICT_SIZE] = {0};

    char* curr = word;
    while (*curr != '\n' && *curr != '\0') {
        letters[*curr] += 1;
        ++curr;
    }

    for (int i = 0; i < DICT_SIZE; ++i) {
        if (letters[i] == 1) {
            return false;
        }
    }
    return true;
}
