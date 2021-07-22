#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

char* makefifoname(pid_t pid, const char* prefix) {
    char* pfifoname;
    asprintf(&pfifoname, "%s.%ld", prefix, (long)pid);
    return pfifoname;
}

bool StopServer(const char* msg) {
    if (strcmp(msg, STOP_SERVER_WORD) == 0) {
        return true;
    }
    return false;
}

bool StopClient(const char* msg) {
    if (strcmp(msg, STOP_CLIENT_WORD) == 0) {
        return true;
    }
    return false;
}

void CreateFifo(char* pfifoname) {
    if (mkfifo(pfifoname, FILE_PERM) == -1 && errno != EEXIST) {
        fprintf(stderr, "Error while creating channel %s: %s\n", pfifoname, strerror(errno));
        exit(EXIT_FAILURE);
    }
}

int Open(const char* filename, int flags) {
    int fd;
    if ((fd = open(filename, flags)) < 0) {
        fprintf(stderr, "Error while opening channel %s: %s\n", filename, strerror(errno));
        exit(EXIT_FAILURE);
    }
    return fd;
}

char* GetFileLastChangeTime(const char* filename) {
    
    struct stat statp;
    char* res = malloc(BUFF_SIZE);
    if (stat(filename, &statp) == -1) {
        strcpy(res, strerror(errno));
        return res;
    }

    time_t now;
    double diff;
    char* fmt;
    char* tmp = malloc(70);

    if (time(&now) == -1)
        fprintf(stderr, "Unable to get current time: %s\n", strerror(errno));
    else
        {
        // modification time
        strcat(res, "\n\tLast modification time: ");
        diff = difftime(now, statp.st_mtime);
        if (diff < 0 || diff > 60 * 60 * 24 * 182.5) /*~ 6 месяцев */
            fmt = "%b %e %Y";   
        else
            fmt = "%b %e %H:%M";
        strftime(tmp, BUFF_SIZE, fmt, localtime(&statp.st_mtime));
        strcat(res, tmp);

        // last access time
        strcat(res, "\n\tLast acces time: ");
        diff = difftime(now, statp.st_atime);
        if (diff < 0 || diff > 60 * 60 * 24 * 182.5) /*~ 6 месяцев */
            fmt = "%b %e %Y";   
        else
            fmt = "%b %e %H:%M";
        strftime(tmp, BUFF_SIZE, fmt, localtime(&statp.st_atime));            

        // last metadat modification time
        strcat(res, tmp);
        strcat(res, "\n\tLast metatada modification: ");
        diff = difftime(now, statp.st_ctime);
        if (diff < 0 || diff > 60 * 60 * 24 * 182.5) /*~ 6 месяцев */
            fmt = "%b %e %Y";   
        else
            fmt = "%b %e %H:%M";
        strftime(tmp, BUFF_SIZE, fmt, localtime(&statp.st_ctime));

        strcat(res, tmp);
        strcat(res, "\n");

        free(tmp);
    }
    return res;
}
