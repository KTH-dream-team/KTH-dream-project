#include <SDL2/SDL.h>
#include <stdbool.h>
#include "GameEngin.h"
#include "startMenu.h"
#include "TextButton.h"
#include "EntityManager.h"
#include "TextureManager.h"
#include "SDL2/SDL_image.h"

struct startmenuinstance
{
    bool isRunning;
    SDL_Surface *surface;
};

void initStartMenu(void *self)
{
}

void renderStartMenu(void *self)
{
    StartMenuInstance *instance = ((StartMenu *)self)->instance;
    GameEngin *Engine = getGameEngin();
    TextureManager *TM = getTextureManager();

    instance->surface = IMG_Load("src/menu.jpg");
    SDL_Renderer *renderer = Engine->getRenderer(Engine);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, instance->surface);
    
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

void destroyStartMenu(void *self)
{
}

bool startMenuIsRunning(void *self)
{
    return ((StartMenu *)self)->instance->isRunning;
}

StartMenu *getStartMenu()
{
    static StartMenu self;
    if (self.instance != NULL)
        return &self;

    self.instance = malloc(sizeof(StartMenuInstance));
    self.instance->isRunning = true;

    self.render = renderStartMenu;
    self.isRunning = startMenuIsRunning;

    return &self;
}