#pragma once
#include <stdbool.h>
#include "data.h"

typedef struct clientInstance ClientInstance;


typedef struct {

    bool (*init)(void *self);
    bool (*isRunning)(void *self);
    void (*destroy)(void *self);
    void (*makeHandShake)(void *self);
    void (*closeConnection)(void *self);
    void (*listen)(void *self);
    void (*broadCast)(void *self, void *data, unsigned long length, int dataType);
    int (*getNrOfClients)(void*self);
    ClientInstance *instance;
} UDPclient;


UDPclient *getUDPclient();