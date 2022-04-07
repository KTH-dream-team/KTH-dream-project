#include "EventHandler.h"
#include <stdlib.h>
#include <stdio.h>
#include "GameEngin.h"



struct eventHandlerInstance
{
    const Uint8 * keyState;
};
void keyUp(void*self)
{
    printf("keyup\n");
    EventHandlerInstance * instance = ((EventHandler*)self)->instance;
    instance->keyState = SDL_GetKeyboardState(NULL);
}
void keyDown(void*self)
{
    printf("keydown\n");
    EventHandlerInstance * instance = ((EventHandler*)self)->instance;
    instance->keyState = SDL_GetKeyboardState(NULL);
    
}
void listen(void*self)
{
    SDL_Event event;
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
bool getKeyPress(void*self, SDL_Scancode scancode)//!if scancode is pressed, return true
{
    EventHandlerInstance * instance = ((EventHandler*)self)->instance;
    if(instance->keyState[scancode] == 1)
        return true;
    return false;
}
void destroyEventHandler(void*self)
{
    EventHandlerInstance * instance = ((EventHandler*)self)->instance;
    free(instance);

    printf("EventHandler Destroyed\n");
}


EventHandler *getEventHandler(){
    static EventHandler self;

    if(self.instance != NULL)
        return &self;

    SDL_GetKeyboardState(NULL);
    self.instance = malloc(sizeof(EventHandlerInstance*));
    self.listen = listen;
    self.getKeyPress = getKeyPress;
    self.destroy = destroyEventHandler;
    self.getMouseState = getMouseState;
    self.instance->keyState = SDL_GetKeyboardState(NULL);

    return &self;
}