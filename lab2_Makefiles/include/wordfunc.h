#ifndef __WORDFUNC_H_
#define __WORDFUNC_H_

#include <stdbool.h>

void first_func(char** words, int word_size);

bool first_condition(char* word, int word_size, char** value_pos);

void second_func(char** words);

bool second_condition(char* word);

char* get_value(char* word);

char* get_name(char* word);

#endif // __WORDFUNC_H_
