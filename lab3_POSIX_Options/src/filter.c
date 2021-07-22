#include <string.h>

#include "filter.h"


bool b_filter(const char* path_var_name, const char* suffix) {
    /* выводить информацию о тех переменных окружения,
     * имена которых начинаются со строки 'string'
     */

    int suf_size = strlen(suffix);

    if (strlen(path_var_name) < suf_size) {
        return false;
    }

    for (int i = 0; i < suf_size; ++i) {
        if (path_var_name[i] != suffix[i]) {
            return false;
        }
    }

    return true;
}


bool y_filter(const char* path_var_name, int min_len) {
    if (strlen(path_var_name) > min_len) {
        return true;
    }
    return false;
}
