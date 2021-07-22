/* /\* We want POSIX.1-2008 + XSI, i.e. SuSv4, features *\/ */

/* POSIX.1-2008 standardized the nftw() function, */
/*     also defined in the Single Unix Specification v4 (SuSv4), */
/*     and available in Linux (glibc, man 3 nftw), OS X, */
/*     and most current BSD variants.  */
#define _XOPEN_SOURCE 700

/* /\* Added on 2017-06-25: */
/*    If the C library can support 64-bit file sizes */
/*    and offsets, using the standard names, */
/*    these defines tell the C library to do so. *\/ */
/* #define _LARGEFILE64_SOURCE */
/* #define _FILE_OFFSET_BITS 64 */

#include <stdlib.h>
#include <unistd.h>
#include <ftw.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>


#ifndef USE_FDS
#define USE_FDS 15
#endif

bool is_link(const char* name) {
    struct stat buf;
    if (lstat(name, &buf) == -1) {
        fprintf(stderr, "Error! Unable to get lstat for file \"%s\": %s\n", name, strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (S_ISLNK(buf.st_mode)) {
        return true;
    }
    return false;
}

bool is_broken_link(const char* name) {
    struct stat buf;
    return (stat(name, &buf) == -1);

}

void delete_broken_link(const char* name) {
    if (is_link(name)) {
        if (is_broken_link(name)) {
            if (unlink(name) == -1) {
                fprintf(stderr, "Error! Unable to delete broken link \"%s\": %s", name, strerror(errno));
                exit(EXIT_FAILURE);
            }
        }
    }
}

int print_entry(const char *filepath, const struct stat *info,
                const int typeflag, struct FTW *pathinfo)
{

    if (typeflag == FTW_SL || typeflag == FTW_SLN) {
        printf("symlink: %s\n", filepath);
        delete_broken_link(filepath);
    }

    return 0;
}


void recursive_parse_dir(const char *const dirpath)
{
    int result;

    /* Invalid directory path? */
    if (dirpath == NULL || *dirpath == '\0') {
        fprintf(stderr, "Error! Empty \"dir_name\" in \"recursive_parse_dir\"\n");
        exit(EXIT_FAILURE);
    }

    nftw(dirpath, print_entry, USE_FDS, FTW_PHYS);
}

int main(int argc, char *argv[])
{
    char* dir_name;
    if (argc < 2) {
        dir_name = ".";
    } else {
        dir_name = argv[1];
    }

    recursive_parse_dir(dir_name);

    return EXIT_SUCCESS;
}
