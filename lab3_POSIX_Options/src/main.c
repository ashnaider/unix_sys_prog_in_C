#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#include "info.h"
#include "handler.h"
#include "utils.h"
#include "print_screen.h"
#include "filter.h"

// 11. –s, –v, –l, –b, –y,–p, –u

struct FormatInfo {
    bool s_flag;
    bool v_flag;
    bool l_flag;

    bool b_flag;
    bool y_flag;

    bool u_flag;

    bool p_flag;

    bool h_flag;

    bool zero_flag;

    char* b_args;
    char* y_args;

    int total_keys_and_args;
};


extern char** environ;

const char* APROG_KEYS = "h0psvlb:y:u";


struct FormatInfo ParseFlags(int argc, char* argv[]) {
    struct FormatInfo formatInfo = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    char c;
    while ((c = getopt(argc, argv, APROG_KEYS)) != -1) {
        switch (c) {
            case 'h':
                formatInfo.h_flag = true;
                return formatInfo;

            case '0':
                formatInfo.zero_flag = true;
                break;

            case 's':
                formatInfo.s_flag = true;
                formatInfo.v_flag = false;

                ++formatInfo.total_keys_and_args;
                break;

            case 'v':
                formatInfo.v_flag = true;
                formatInfo.s_flag = false;

                ++formatInfo.total_keys_and_args;
                break;

            case 'l':
                formatInfo.l_flag = true;
                ++formatInfo.total_keys_and_args;
                break;

            case 'b':
                formatInfo.b_flag = true;
                formatInfo.b_args = optarg;

                formatInfo.total_keys_and_args += 2;
                break;

            case 'y':
                formatInfo.y_flag = true;
                formatInfo.y_args = optarg;

                formatInfo.total_keys_and_args += 2;
                break;

            case 'u':
                formatInfo.u_flag = true;
                ++formatInfo.total_keys_and_args;
                break;

            case 'p':
                formatInfo.p_flag = true;
                ++formatInfo.total_keys_and_args;
                break;

            case ':' :
                /* atexit(wrong_key_err_hanlder); */
                exit(EXIT_FAILURE);

            case '?' :
                /* atexit(no_arg_err_hanlder); */
                exit(EXIT_FAILURE);
        }
    }

    return formatInfo;
}


int Process_P_flag(char* argv[], const struct FormatInfo formatInfo) {
    int i = formatInfo.total_keys_and_args;
    int j = 0;
    int total_len = 0;

    while (argv[i] != NULL) {
        /* printf("%s\n", argv[i]); */
        if (argv[i][0] != '-') {
            total_len += print_request(argv[i], formatInfo.s_flag, formatInfo.v_flag, formatInfo.l_flag);
            ++j;
        }
        ++i;
    }

    if (j == 0) {
        fprintf(stderr, "No command line arguments needed by -p option.\n");
        exit(EXIT_FAILURE);
    }
    return total_len;
}


int Process_other_flags(char* argv[], const struct FormatInfo formatInfo) {
    int i = 0;
    int total_len = 0;

    char* end;
    char* curr_name;
    bool print_curr_var;
    int min_len;

    if (formatInfo.y_flag) {
        min_len = strtod(formatInfo.y_args, &end);
    }

    while (environ[i] != NULL) {
        print_curr_var = true;
        curr_name = get_name_from_path_line(environ[i]);

        if (formatInfo.b_flag) {
            if (!b_filter(curr_name, formatInfo.b_args)) {
                print_curr_var = false;
            }
        }

        if (formatInfo.y_flag) {
            if (!y_filter(curr_name, min_len)) {
                print_curr_var = false;
            }
        }

        if (print_curr_var) {
            total_len += print_request(curr_name, formatInfo.s_flag, formatInfo.v_flag, formatInfo.l_flag);
        }

        ++i;
        free(curr_name);
    }

    return total_len;
}


int main(int argc, char* argv[]) {

    if (argc < 2) {
        print_info();
        return EXIT_SUCCESS;
    }

    struct FormatInfo formatInfo = ParseFlags(argc, argv);

    if (formatInfo.h_flag) {
        print_info();
        return EXIT_SUCCESS;
    }

    if (formatInfo.zero_flag) {
        print_author_info();
        return EXIT_SUCCESS;
    }

    print_conditions(formatInfo.s_flag, formatInfo.v_flag);
    int total_len = 0;

    if (formatInfo.p_flag == true) {
        total_len += Process_P_flag(argv, formatInfo);

    } else {
        total_len += Process_other_flags(argv, formatInfo);
    }

    if (formatInfo.u_flag) {
        printf("total string len: %d\n", total_len);
    }

    return 0;
}
