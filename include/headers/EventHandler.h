#include <stdbool.h>
#include "SDL2/SDL.h"
#include "GameEngin.h"
#include <stdlib.h>

typedef struct eventHandlerInstance EventHandlerInstance;

typedef struct eventHandler
{
    void (*listen)(void*self);
    bool (*getKeyPress)(void*self, SDL_Scancode scancode);
    void (*destroy)(void *self);
    unsigned int (*getMouseState)(int *mouseX, int *mouseY);

    EventHandlerInstance *instance;
}EventHandler;

EventHandler *getEventHandler();