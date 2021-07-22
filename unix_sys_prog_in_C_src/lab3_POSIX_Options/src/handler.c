#include <stdio.h>
#include <unistd.h>

#include "handler.h"

void few_arguments_err_handler () {
    fprintf(stderr, "Few arguments, one more expected\n");
}

void wrong_key_err_hanlder() {
    fprintf(stderr, "Wrong option: %c\n", optopt);
}

void no_arg_err_hanlder() {
    fprintf(stderr, "Arguments for option %c are missed\n", optopt);
}


void unknown_path_var_handler() {
    fprintf(stderr, "Unknown PATH variable: %s\n", invalid_path_var);
}
