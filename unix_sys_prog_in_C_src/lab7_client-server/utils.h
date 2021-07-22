#ifndef __INFO_H
#define __INFO_H

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define FILE_PERM S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH
#define SERVER_FIFO_NAME "/tmp/fifo.request"
#define FIFO_PREFIX "/tmp/fifo.answer"
#define BUFF_SIZE 400
#define STOP_CLIENT_WORD "stop-client"
#define STOP_SERVER_WORD "stop-server"

struct Message {
    pid_t clientpid;
    char data[BUFF_SIZE];
} typedef message_t;

char* makefifoname(pid_t pid, const char* prefix);

bool StopServer(const char* msg);

bool StopClient(const char* msg);

void CreateFifo(char* pfifoname);

int Open(const char* filename, int flags);

char* GetFileLastChangeTime(const char* filename);

#endif