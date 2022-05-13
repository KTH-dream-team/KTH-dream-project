#include <SDL2/SDL.h>
#include <stdbool.h>
#include "GameEngin.h"
#include "startMenu.h"
#include "TextButton.h"
#include "EntityManager.h"
#include "SDL2/SDL_image.h"

struct startmenuinstance
{
    SDL_Surface *surface;
    bool isRunning;
    TextButton *button;
    SDL_Renderer *renderer;
};


void initStartMenu(void *self)
{
    StartMenuInstance *instance = ((StartMenu *)self)->instance;
    EntityManager *EM = getEntityManager();
    GameEngin *Engine = getGameEngin();

    instance->isRunning = true;
    if (!(instance->surface = IMG_Load("src/menu.jpg")))
        printf("Couldn't find the Image\n");

    // create a button on start menu
    SDL_Color txtColor= {255, 255, 255, 255};
    SDL_Color bgColor= {0, 0, 0, 255};
    SDL_Rect rect= {100, 100, 200, 60};
    instance->button = newTextButton("Start Game", txtColor, bgColor, 24,rect);
    EM->add(EM, "TextButton-1", instance->button);
    instance->renderer = Engine->getRenderer(Engine);;
}

void renderStartMenu(void *self)
{
    StartMenuInstance *instance = ((StartMenu *)self)->instance;
    SDL_Texture *texture = SDL_CreateTextureFromSurface(instance->renderer, instance->surface);
    instance->button->render(instance->button);
    SDL_RenderCopy(instance->renderer, texture, NULL, NULL);
    SDL_RenderPresent(instance->renderer);




    //EntityManager *EM = getEntityManager();
    //TextButton *button = EM->getByID(EM, "TextButton-1");
    // button->render(button);
    // int btnState = button->getBtnState(button);
    // if(btnState == 2){
    //    printf("Is clicking here\n\n");
    //    instance->isRunning = 0;
    // }
}

void destroyStartMenu(void *self)
{
    StartMenuInstance *instance = ((StartMenu *)self)->instance;
    SDL_FreeSurface(instance->surface);
    free(instance);
    printf("Menu has been destroyed!\n");
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
    self.init = initStartMenu;
    self.destroy = destroyStartMenu;
    self.isRunning = startMenuIsRunning;
    self.render = renderStartMenu;

    self.instance->isRunning = false;
    self.instance->surface = NULL;

    return &self;
}