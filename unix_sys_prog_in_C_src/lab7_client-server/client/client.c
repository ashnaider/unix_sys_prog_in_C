#define _GNU_SOURCE

#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdbool.h>

#include "utils.h"

// 11. Процес клієнт запитує тимчасові характеристики файлу

int main() {
    int fd_server,
        fd_client = -1;

    ssize_t nread;
    message_t msg;

    msg.clientpid = getpid();
    printf("Client %ld is starting...\n", (long) msg.clientpid);
    
    char* pfifoname = makefifoname(msg.clientpid, FIFO_PREFIX);

    CreateFifo(pfifoname);  

    fd_server = Open(SERVER_FIFO_NAME, O_WRONLY);

    while (true) {
        scanf("%s", msg.data);
    
        if (StopClient(msg.data)) {
            break;
        } 

        int wrote = write(fd_server, &msg, sizeof(msg));

        if (StopServer(msg.data)) {
            break;
        }

        fd_client = Open(pfifoname, O_RDONLY);
        
        if ((nread = read(fd_client, &msg, sizeof(msg))) > 0) {
            printf("Server output: %s\n", msg.data);
        }

        close(fd_client);
    }
    
    close(fd_server);
    close(fd_client);
    unlink(pfifoname);
    printf("Client %d finished work\n", msg.clientpid);
    
}

