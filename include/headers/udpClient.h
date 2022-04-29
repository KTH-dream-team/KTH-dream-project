#pragma once
#include"data.h"

typedef struct clientInstance ClientInstance;


typedef struct {

    bool (*init)(void *self);
    bool (*isRunning)(void *self);
    int (*getID)(void *self);
    void (*destroy)(void *self);
    void (*makeHandShake)(void *self);
    void (*closeConnection)(void *self);
    void (*listen)(void *self);
    void (*broadCast)(void *self, DataClient *data, unsigned long length);
    ClientInstance *instance;
} UDPclient;


UDPclient *getUDPclient();