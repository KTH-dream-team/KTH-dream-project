#pragma once
#include <stdbool.h>
#include "SDL2/SDL_net.h"
typedef struct tcpServerInstance TCPServerInstance;

typedef struct
{

    bool (*init)(void *self);
    bool (*isRunning)(void *self);
    void (*destroy)(void *self);
    void (*listen)(void *self);

    TCPServerInstance *instance;
} TCPserver;

TCPserver *getTCPserver();