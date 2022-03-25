#pragma once
#include "SDL2/SDL.h"
#include "datatypes.h"

typedef struct gameInstance GameInstance;

typedef struct gameEngin
{
    int (*init)(void *self, char *title, int width, int height, int fullScreen);
    bool (*isRunning)(void *self);
    bool (*destroyEngine)(void *self);
    void (*handleEvents)(void *self);
    void (*handleUpdates)(void *self);
    void (*handleRenders)(void *self);

    GameInstance *instance;

} GameEngin;

GameEngin *createGameEngin();
