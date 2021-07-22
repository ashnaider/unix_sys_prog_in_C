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
#include <sys/resource.h>
#include <signal.h>
#include <syslog.h>
#include <ctype.h>

#include "utils.h"

void free_str(char* str) {
    if (str != NULL) {
        free(str);
    }
}

void ToUpper(char* str) {
    for (int i = 0; str[i] != '\0'; ++i) {
        str[i] = toupper(str[i]);
    }
}

int my_daemonize() {
    int i;
    pid_t pid;
    struct sigaction sa;
    struct rlimit rl;

    umask(0);

    // определить максимально возможный номер
    //дескриптора
    if (getrlimit(RLIMIT_NOFILE, &rl) < 0) {
        return -1;  
    }

    //Стать лидером нового сеансп
    if ((pid = fork()) < 0) {
        return -1;
    }
    else if (pid != 0) { // родительский процесс
        exit(0);
    }

    setsid();


    //Воспрепятствовать получению управляющего терминала
    sa.sa_handler = SIG_IGN;

    sigemptyset(&sa.sa_mask);

    sa.sa_flags = 0;

    if (sigaction(SIGHUP, &sa, NULL) < 0) {
        return -1;
    }

    if ((pid = fork()) < 0) {
        return -1;
    }
    else if (pid != 0) {
        exit(0);
    }

    // Сделать корневой каталог текущим рабочим каталогом
    if (chdir("/") < 0) {
        return -1;
    }


    // Закрыть все ненужные файловые дескрипторы.
    if (rl.rlim_max == RLIM_INFINITY) {
        rl.rlim_max = 1024;
    }

    for (i = 0; i < rl.rlim_max; i++) {
        close(i);
    }

    open("/dev/null", O_RDWR);
    dup(0);
    dup(0);   // why is it here two times??
    return 0;
}

int main() {
    int fd_server,
        fd_dummy_reader,
        fd_client;

    ssize_t nread;
    message_t msg;

    printf("Starting server...\n");
    
    daemon(1, 0);  // no chdir
    // my_daemonize();

    openlog("Anton's daemon", LOG_CONS|LOG_NDELAY|LOG_PERROR|LOG_PID, LOG_LOCAL0);

    syslog(LOG_INFO, "Starting server...\n");

    CreateFifo(SERVER_FIFO_NAME);
    
    fd_server = Open(SERVER_FIFO_NAME, O_RDONLY);
    
    fd_dummy_reader = Open(SERVER_FIFO_NAME, O_WRONLY);
    


    while ((nread = read(fd_server, &msg, sizeof(msg))) > 0) {
        if (StopServer(msg.data)) {
            break;
        }

        char* pfifoname;
        pfifoname = makefifoname(msg.clientpid, FIFO_PREFIX);
        fd_client = Open(pfifoname, O_WRONLY);

        syslog(LOG_INFO, "Recieved message '%s' from client %d", msg.data, msg.clientpid);
        char* res = GetFileLastChangeTime(msg.data);
        strcpy(msg.data, res);
        write(fd_client, &msg, sizeof(msg));
        
        close(fd_client);
        free(pfifoname);
        free(res);
    }


    close(fd_dummy_reader);
    close(fd_server);
    syslog(LOG_INFO, "Stoping sever.");
    closelog();
    unlink(SERVER_FIFO_NAME);
    printf("Stopping server...\n");
    exit(EXIT_SUCCESS);
}