#include <stdio.h>

#include "info.h"

// 11. –s, –v, –l, –b, –y,–p, –u

void print_info() {
    printf("aprog 0.1.0\n");
    printf("Anton Shnaider <anton.shnaider@stud.onu.edu.ua>\n");

    printf("\naprog gives you information about PATH variables.\n\n");

    printf("Use -h for short descriptions and -0 for authors information.\n\n");

    printf("USAGE:\n\t");
    printf("aprog [PATH_VARS] [OPTIONS] [PATH_VARS]");

    printf("\n\nARGS:\n");
    printf("\t<PATH_VARS>\tPATH variables to find and show");

    printf("\n\nOPTIONS:\n");
    printf("\t-s\t\tOnly show the PATH names (without values).\n");
    printf("\t-v\t\tOnly show the PATH values (without names).\n");
    printf("\t-l\t\tShow lengths of PATH values.\n\n");
    printf("\t-b\t\tFind variables, which names start with 'string'\n");
    printf("\t-y\t\tFind variables, which names length are more than 'number'\n\n");
    printf("\t-u\t\tPrint total length of all found strings\n\n");
    printf("\t-h\t\tPrints help information\n");
    printf("\t-0\t\tPrints authors information\n");
}


void print_author_info() {
    printf("Author: Anton Shnaider\n");
    printf("e-mail: <anton.shnaider@stud.onu.edu.ua\n");
    printf("Group: 2d group\n");

}
