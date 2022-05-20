#include "InputHandler.h"
#include <stdlib.h>
#include <stdio.h>
#include "GameEngin.h"


struct inputHandlerInstance
{
    const Uint8 *keyState;
    SDL_Event event;
};
void keyUp(void *self)
{
    InputHandlerInstance *instance = ((InputHandler *)self)->instance;
    instance->keyState = SDL_GetKeyboardState(NULL);
}
void keyDown(void *self)
{
    InputHandlerInstance *instance = ((InputHandler *)self)->instance;
    instance->keyState = SDL_GetKeyboardState(NULL);
}


void listen(void *self)
{
    SDL_Event event = ((InputHandler*)self)->instance->event;
    while (SDL_PollEvent(&event))
    {
        GameEngin *Engine = getGameEngin();
        switch (event.type)
        {
        case SDL_QUIT:
            Engine->quit(Engine);
            break;
        case SDL_KEYUP:
            keyUp(self);
            break;
        case SDL_KEYDOWN:
            keyDown(self);
            break;
        default:
            break;
        }
    }
}
unsigned int getMouseState(int *mouseX, int *mouseY)
{
    return SDL_GetMouseState(mouseX, mouseY);
}
bool getKeyPress(void *self, SDL_Scancode scancode)
{
    InputHandlerInstance *instance = ((InputHandler *)self)->instance;
    if (instance->keyState[scancode] == 1)
        return true;
    return false;
}

SDL_Event getEvent(void *self){
    return ((InputHandler*)self)->instance->event;
}

void destroyInputHandler(void *self)
{
    InputHandlerInstance *instance = ((InputHandler *)self)->instance;
    free(instance);
    printf("InputHandler Destroyed\n");
}

InputHandler *getInputHandler()
{
    static InputHandler self;

    if (self.instance != NULL)
        return &self;

    SDL_GetKeyboardState(NULL);
    self.instance = malloc(sizeof(InputHandlerInstance *));
    self.listen = listen;
    self.getEvent = getEvent;
    self.getKeyPress = getKeyPress;
    self.destroy = destroyInputHandler;
    self.getMouseState = getMouseState;
    self.instance->keyState = SDL_GetKeyboardState(NULL);

    return &self;
}