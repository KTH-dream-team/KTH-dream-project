#pragma once
typedef struct UDPserverInstance UDPServerInstance;

typedef struct {

    bool (*init)(void *self);
    bool (*isRunning)(void *self);
    void (*destroy)(void *self);
    void (*listen)(void *self);

    UDPServerInstance *instance;
} UDPserver;


UDPserver *getUDPserver();