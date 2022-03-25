#include "GameEngin.h"
#include "DataTypes.h"
#include <stdio.h>

struct enginInstance
{
    bool isRunning;
    SDL_Window *window;
    SDL_Renderer *renderer;
};

bool isRunning(void *self)
{
    return ((GameEngin *)self)->instance->isRunning;
}

int init(void *self, char *title, int width, int height, int fullScreen)
{
    GameEngin *Engin = ((GameEngin *)self);

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Error: SDL failed to initialize\nSDL Error: '%s'\n", SDL_GetError());
        return 0;
    }

    Engin->instance->window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, fullScreen);
    if (!Engin->instance->window)
    {
        printf("Error: Failed to open window\nSDL Error: '%s'\n", SDL_GetError());
        return 0;
    }

    Engin->instance->renderer = SDL_CreateRenderer(Engin->instance->window, -1, SDL_RENDERER_ACCELERATED);
    if (!(Engin->instance->window))
    {
        printf("Error: Failed to create renderer\nSDL Error: '%s'\n", SDL_GetError());
        return 0;
    }
    Engin->instance->isRunning = true;

    return 1;
}

void handleEvents(void *self)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            ((GameEngin *)self)->instance->isRunning = false;
            break;
        default:
            break;
        }
    }
}

void handleUpdates(void *self)
{
    // updates functions go here !!!
    printf("Updates!!!! \n");
    // updates functions go here !!!
}

void handleRenders(void *self)
{
    GameEngin *Engin = ((GameEngin *)self);
    SDL_SetRenderDrawColor(Engin->instance->renderer, 255, 0, 0, 255);
    SDL_RenderClear(Engin->instance->renderer);
    // render functions go here !!!

    // updates functions go here !!!
    SDL_RenderPresent(Engin->instance->renderer);
}

bool destroyEngine(void *self)
{
    GameEngin *Engin = ((GameEngin *)self);
    SDL_Quit();
    SDL_DestroyWindow(Engin->instance->window);
    SDL_DestroyRenderer(Engin->instance->renderer);
    free(Engin->instance);
    // destroy all assets here !!!

    // destroy functions go here !!!
    printf("Engine Cleaned");
    return false;
}

GameEngin *createGameEngin()
{
    static GameEngin self;

    self.instance = malloc(sizeof(EnginInstance *));
    self.init = init;
    self.isRunning = isRunning;
    self.handleEvents = handleEvents;
    self.handleUpdates = handleUpdates;
    self.handleRenders = handleRenders;
    self.destroyEngine = destroyEngine;

    return &self;
}