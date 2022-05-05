#pragma once
#include "stdbool.h"
#include "data.h"
#include "SDL2/SDL_net.h"


typedef struct networkClientInstance NetworkClientInstance;


typedef struct {

    bool (*init)(void *self);
    void (*listen)(void *self);
    int (*TCPresive)(void *self, void *dest, int desireInt);
    //void (*UDPresive)(void *self);
    int (*TCPgetID)(void *self);
    int (*UDPgetID)(void *self);
    int (*TCPbroadCast)(void *self, void *data, int dataSize);
    void (*UDPbroadCast)(void *self, void *data, int dataSize);
    void (*destroy)(void *self);
    
    NetworkClientInstance *instance;
} NetworkClient;

NetworkClient * getNetworkClient();