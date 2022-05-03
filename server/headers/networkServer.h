#pragma once
#include "stdbool.h"

typedef struct networkServerInstance NetworkServerInstance;

typedef struct
{

    bool (*init)(void *self);
    void (*listen)(void *self);
    void (*destroy)(void *self);

    NetworkServerInstance *instance;
} NetworkServer;

NetworkServer *getNetworkServer();