#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>

/* Вариант 5. Напишите программу, */
/* которая при получении сигнала SIGUSR1 порождает два новых процесса, */
/* после чего получение сигнала SIGUSR1 должно приводить к окончанию одного из них, */
/* а получение сигнала SIGUSR2 прекращать работу второго и восстанавливать */
/* обработку сигнала SIGUSR1. */
/* Порожденные процессы должны ожидать получения сигнала. */

pid_t first_sig, second_sig;

static void sigusr1_hndlr(int signo);
static void sigusr1_hndlr_after(int signo);
static void sigusr2_hndlr(int signo);


void Kill(pid_t proc, const char* proc_name) {
    int k_res = kill(proc, SIGTERM);
    if (k_res != 0) {
        fprintf(stderr, "Unable to kill %s process! id: %d, Error: %s\n", proc_name, proc, strerror(errno));
        exit(EXIT_FAILURE);
    }
    wait(NULL);
    printf("Killed %s process, pid: %d\n\n", proc_name, proc);
}

static void sigusr1_hndlr(int signo) {
    /***при получении сигнала SIGUSR1 порождает два новых процесса***/
    printf("In SIGUSR1 handler\n");
    first_sig = fork();


    if (first_sig == -1) {
        fprintf(stderr, "Unable to create first process! Error: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }


    if (first_sig == 0) {
        while(1) {
            pause();
        }
    }
    else {
        printf("From pid: %d\n", getpid());
        printf("Created first process, id: %d\n", first_sig);

        second_sig = fork();

        if (second_sig == -1) {
            fprintf(stderr, "Unable to create second process! Error: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }

        if (second_sig == 0) {
            while(1) {
                pause();
            }
        }
        else {
            signal(SIGUSR1, sigusr1_hndlr_after);
            printf("Fromt pid: %d\n", getpid());
            printf("Created second process, id: %d\n\n", second_sig);
        }
    }

}

static void sigusr1_hndlr_after(int signo) {
    /***второе получение сигнала SIGUSR1 должно приводить к окончанию одного из них***/
    printf("In second SIGUSR1 handler\n");
    Kill(first_sig, "first");

    signal(SIGUSR2, sigusr2_hndlr);
    /* signal(SIGUSR1, NULL); */
}

static void sigusr2_hndlr(int signo) {
    printf("In SIGUSR2 handler\n");
    Kill(second_sig, "second");
    signal(SIGUSR1, sigusr1_hndlr);
    /* signal(signo, NULL); */
}


int main() {
    int p_id = getpid();
    printf("Current process id: %d\n", p_id);

    signal(SIGUSR1, sigusr1_hndlr);

    while(1) {
        pause();
    }

    return 0;
}
