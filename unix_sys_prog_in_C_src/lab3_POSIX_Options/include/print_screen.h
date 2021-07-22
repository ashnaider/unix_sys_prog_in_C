#ifndef __PRINT_SCREEN_H_
#define __PRINT_SCREEN_H_

#include <stdbool.h>

int print_request(const char* path_var_name, bool s_flag, bool v_flag, bool l_flag);

void print_conditions(bool s_flag, bool v_flag);

#endif // __PRINT_SCREEN_H_
