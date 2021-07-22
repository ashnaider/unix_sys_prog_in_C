#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "print_screen.h"


int print_request(const char* path_var_name, bool s_flag, bool v_flag, bool l_flag) {
    int total_len = 0;
    char* env_val = getenv(path_var_name);

    if (env_val == NULL) {
        fprintf(stderr, "No such PATH variable: %s\n", path_var_name);
        return 0;
    }

    if (!s_flag && !v_flag) {

        total_len += strlen(path_var_name) + 1 + strlen(env_val);

        printf("%s=", path_var_name);
        printf("%s\n", env_val);
    }
    else if (s_flag) {
        total_len += strlen(path_var_name);
        printf("%s\n", path_var_name);
    }
    else { // v_flag

        total_len += strlen(env_val);
        printf("%s\n", env_val);
    }

    if (l_flag) {
        printf("value length: %ld\n", strlen(env_val));
    }

    return total_len;
}


void print_conditions(bool s_flag, bool v_flag) {
    if (!s_flag && !v_flag) {
        printf("-s and -v were not set, so we print 'name=value'\n");
    }
    else if (s_flag) {
        printf("-s was set last, so we print only 'name'\n");
    }
    else { // v_flag
        printf("-v was set last, so we print only 'value'\n");
    }

}
