#pragma once
#include "SDL2/SDL.h"
#include "DataTypes.h"

typedef struct enginInstance EnginInstance;

typedef struct gameEngin
{
    int (*init)(void *self, char *title, int width, int height, int fullScreen);
    bool (*isRunning)(void *self);
    bool (*destroyEngine)(void *self);
    void (*handleEvents)(void *self);
    void (*handleUpdates)(void *self);
    void (*handleRenders)(void *self);

    EnginInstance *instance;

} GameEngin;

GameEngin *createGameEngin();
