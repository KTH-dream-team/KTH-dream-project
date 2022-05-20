/*
    Handle input events. (mouse, keyboard)

    void (*listen)(void *self);
        update inputhandler, need to be called on every single frame;

    bool (*getKeyPress)(void *self, SDL_Scancode scancode);
        return if a key is pressed, true if is pressed, false if not;
        scancode: SDL_Scancode, SDL used to identify key

    unsigned int (*getMouseState)(int *mouseX, int *mouseY);
        get mouse state,
        update mouseX and mouseY by current mouse position;
        return mouse buttons;

    void (*destroy)(void *self);
        destroy input handler;
*/

#include <stdbool.h>
#include "SDL2/SDL.h"
#include "GameEngin.h"

typedef struct inputHandlerInstance InputHandlerInstance;

typedef struct inputHandler
{
    void (*listen)(void *self);
    SDL_Event (*getEvent)(void *self);
    bool (*getKeyPress)(void *self, SDL_Scancode scancode);
    unsigned int (*getMouseState)(int *mouseX, int *mouseY);
    void (*destroy)(void *self);

    InputHandlerInstance *instance;
} InputHandler;

InputHandler *getInputHandler();