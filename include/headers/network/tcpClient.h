#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "SDL2/SDL_net.h"
#include "tcpClient.h"
#include "data.h"

typedef struct TCPclientInstance TCPClientInstance;

typedef struct
{

    bool (*init)(void *self);
    bool (*isRunning)(void *self);
    void (*destroy)(void *self);
    void (*listen)(void *self);
    int (*broadCast)(void *self, Data *data, int dataSize);
    TCPClientInstance *instance;
} TCPclient;

TCPclient *getTCPclient();