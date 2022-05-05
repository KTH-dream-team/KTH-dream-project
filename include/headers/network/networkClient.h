#pragma once
#include "stdbool.h"
#include "data.h"


typedef struct networkClientInstance NetworkClientInstance;

typedef struct {

    bool (*init)(void *self);
    void (*listen)(void *self);
    int (*TCPbroadCast)(void *self, Data *data, int dataSize, int datatype);
    void (*UDPbroadCast)(void *self, void *data, int dataSize);
    void (*connect)(void*self);
    
    NetworkClientInstance *instance;
} NetworkClient;

NetworkClient * getNetworkClient();