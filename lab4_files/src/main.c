/* 3. Написать программу, добавляющую содержимое одного файла в */
/* конец другого в обратном порядке. Имена файлов задавать в виде аргументов */
/* командной строки. Проверить, не возникают ли ошибки при системных */
/* вызовах. */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>

#include "debug.h"

#define BUFFSIZE 2
#define PERMS S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH


int max(int a, int b) {
    if (a < b)
        return b;
    return a;
}

int min(int a, int b) {
    if (a < b)
        return a;
    return b;
}

int Open(const char* source_file_name, int flags, mode_t perms) {

    int fd = open(source_file_name, flags, perms);
    if (fd == -1) {
        fprintf(stderr, "Can't open file. No such file %s\n", source_file_name);
        exit(EXIT_FAILURE);
    }
    return fd;
}

off_t get_file_size(int fd) {
    off_t curpos, len = 1;
    if ((curpos = lseek(fd, 0, SEEK_CUR)) >= 0) {
        len = lseek(fd, 0, SEEK_END);
        lseek(fd, curpos, SEEK_SET);
    }
    return len;
}

void strrev(char s[]) {
    int len = strlen(s);
    int i, j;
    char c;

    for (i = 0, j = len - 1; i < j; ++i, --j) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}


int main(int argc, char* argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Few argumetns!\n");
        exit(EXIT_FAILURE);
    }

    const char* source_file_name      = argv[1];
    const char* destination_file_name = argv[2];

// номер устройства и индексный узел (системный вызов stat)
    struct stat sb1, sb2;
    stat(source_file_name, &sb1);
    stat(destination_file_name, &sb2);

    if (sb1.st_dev == sb2.st_dev &&
        sb1.st_ino == sb2.st_ino) {
        fprintf(stderr, "Error: files are same!\n");
        exit(EXIT_FAILURE);
    }

    mode_t prev_mode = umask(0);

    int source_fd = Open(source_file_name, O_RDONLY, PERMS);
    int dest_fd = Open(destination_file_name, O_CREAT | O_WRONLY | O_APPEND, PERMS);

    debug_print("Source fd: %d\n", source_fd);
    debug_print("Dest fd: %d\n", dest_fd);


    int  nbytes;

    char* buffer = malloc(BUFFSIZE);
    char* out;

    off_t source_size = get_file_size(source_fd) - 1;
    off_t pos = max(source_size - BUFFSIZE, 0);

    int i = 1;
    int total_read = 0;
    int c = 0;

    while ((nbytes = pread(source_fd, buffer, BUFFSIZE, pos)) != 0) {

        if (nbytes == -1 && errno != EINTR) {
            fprintf(stderr, "read: %s\n", strerror(errno));
            break;
        }

        int s = min(BUFFSIZE, source_size - total_read);
        if (s <= 0) {
            break;
        }

        out = malloc(s);
        memcpy(out, buffer, s);

        strrev(out);

        if (write(dest_fd, out, s) < 0) {
            fprintf(stderr, "Error while writing file %s: %s\n",
                    destination_file_name, strerror(errno));
            close(source_fd);
            exit(EXIT_FAILURE);
        }

        total_read += nbytes;
        if (out != NULL) {
            free(out);
        }

        ++i;
        pos = max(source_size - (BUFFSIZE * i), 0);

        if (pos == 0) {
            ++c;
            if (c == 2) {
                break;
            }
        }

    }

    if (buffer != NULL) {
        free(buffer);
    }

    close(source_fd);
    close(dest_fd);

    /* umask(prev_mode); */

    return 0;

}

// pread, pwrite
