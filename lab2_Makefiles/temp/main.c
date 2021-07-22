#include <stdio.h>

#include "func.h"
#include "print.h"
#include "defs.h"
#include "module.h"
#include "wordfunc.h"


int main() {
    char str[MAX_STRING_SIZE];
    // чтение строки со словами(разделители – пробел,
    // знак табуляции, точка и запятая)
    //

    module_func(5);

    fgets(str, MAX_STRING_SIZE, stdin);
    char ** words;
    words = split(str, " \t,.");

    /* printwords(words); */

    custom_func(words, 3);

    return 0;
}
