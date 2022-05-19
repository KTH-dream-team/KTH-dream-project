/*
    GameEngin classen är den största objecten och det finns endast en instance av GameEngin i hela spellet;

    int init(void *self, char *title, int width, int height, int fullScreen);
        return true of init success, false if failed.
        tittle: tittle för spellet
        width, height: window dimention;
        fullscreen: fullscreen mode;

    void (*handleEvents)(void *self)
        this function handle events, the FINAL eventhandler function should go here.

    void (*handleUpdates)(void *self)
        all FINAL update functions should go here. this functions runs on every frames.

    void (*handleRenders)(void *self)
        All FINAL render functions should go here. this functions runs on every frames.

    bool (*destroyEngine)(void *self)
        This function cleans the program when user close the window. All manually located memories MUST be destroyed.

    SDL_Renderer *(*getRenderer)(void *self)
        This functions return instance of the current renderer, which might be needed for SDL operations.
*/

#pragma once
#include "SDL2/SDL.h"
#include <stdbool.h>
typedef struct enginInstance EnginInstance;

typedef struct gameEngin
{
    bool (*init)(void *self, char *title, int width, int height, int fullScreen);
    bool (*isRunning)(void *self);
    bool (*destroyEngine)(void *self);
    void (*handleEvents)(void *self);
    void (*handleUpdates)(void *self);
    void (*handleRenders)(void *self);
    void (*innitGameInstances)(void*self);
    void (*quit)(void *self);
    SDL_Renderer *(*getRenderer)(void *self);
    SDL_Window *(*getWindow)(void *self);

    EnginInstance *instance;

} GameEngin;

GameEngin *getGameEngin();
