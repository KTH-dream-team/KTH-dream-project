#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "SDL2/SDL_net.h"

#define MAX_CLIENTS 4
#define MAX_SIZE 512
#define CLIENT_PORT 0
#define CLIENT_IP "127.0.0.1"
#define SERVER_PORT 3000
#define SERVER_IP "127.0.0.1"


int setRand(void *self);

typedef struct TCPclientInstance TCPClientInstance;


typedef struct {

    bool (*init)(void *self);
    bool (*isRunning)(void *self);
    void (*destroy)(void *self);
    void (*recive)(void *self);
    void (*listen)(void *self);
    int (*broadCast)(void *self, void *data, int dataSize);
    TCPClientInstance *instance;
} TCPclient;


TCPclient *getTCPclient();