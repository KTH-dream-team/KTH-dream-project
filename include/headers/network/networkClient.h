#pragma once
#include "stdbool.h"
#include "data.h"


typedef struct networkClientInstance NetworkClientInstance;

typedef struct {

    bool (*init)(void *self);
    void (*listen)(void *self);
    int (*TCPbroadCast)(void *self, void *data, int dataSize, int datatype);
    void (*UDPbroadCast)(void *self, void *data, int dataSize, int dataType);
    void (*connect)(void*self);
    int (*getTCPID) (void *self);
    bool (*getIsAllClientJoined) (void*self);
    
    NetworkClientInstance *instance;
} NetworkClient;

NetworkClient * getNetworkClient();