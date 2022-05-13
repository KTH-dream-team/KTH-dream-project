#include <SDL2/SDL.h>
#include <stdbool.h>
#include "GameEngin.h"
#include "startMenu.h"
#include "TextButton.h"
#include "EntityManager.h"
#include "TextureManager.h"
#include "SDL2/SDL_image.h"
#include "stdio.h"

struct startmenuinstance
{
    bool isRunning;
    SDL_Surface *surface;
    TextButton *connect;
    TextButton *createServer;
};

void renderStartMenu(void *self)
{
    StartMenuInstance *instance = ((StartMenu *)self)->instance;
    GameEngin *Engine = getGameEngin();

    SDL_Renderer *renderer = Engine->getRenderer(Engine);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, instance->surface);

    SDL_RenderCopy(renderer, texture, NULL, NULL); //! ordning spelar roll på renderingen
    instance->connect->render(instance->connect);
    instance->createServer->render(instance->createServer);
    SDL_RenderPresent(renderer);
}

void updateStartMenu(void *self)
{
    // get user input on buttons
    StartMenuInstance *instance = ((StartMenu *)self)->instance;
    instance->connect->update(instance->connect);
    instance->createServer->update(instance->createServer);

    int connectBtnState = instance->connect->getStateTextButton(instance->connect);
    int serverBtnState = instance->createServer->getStateTextButton(instance->createServer);
    if (connectBtnState == 0)
    {
        SDL_Color bgColor = {0, 0, 0, 255};
        SDL_Color txtColor = {255, 255, 255, 255};
        instance->connect->changeColor(instance->connect, bgColor, txtColor);
    }
    else if (connectBtnState == 1)
    {
        SDL_Color bgColor = {255, 255, 255, 255};
        SDL_Color txtColor = {1, 1, 1, 255};
        instance->connect->changeColor(instance->connect, bgColor, txtColor);
    }
    else if (connectBtnState == 2)
    {
        instance->isRunning = false;
    }

    if (serverBtnState == 0)
    {
        SDL_Color bgColor = {0, 0, 0, 255};
        SDL_Color txtColor = {255, 255, 255, 255};
        instance->createServer->changeColor(instance->createServer, bgColor, txtColor);
    }
    else if (serverBtnState == 1)
    {
        SDL_Color bgColor = {255, 255, 255, 255};
        SDL_Color txtColor = {1, 1, 1, 255};
        instance->createServer->changeColor(instance->createServer, bgColor, txtColor);
    }
    else if (serverBtnState == 2)
    {
        printf("Create server left click\n");
    }
}

void destroyStartMenu(void *self)
{
    StartMenuInstance *instance = ((StartMenu *)self)->instance;
    SDL_FreeSurface(instance->surface);
    instance->connect->destroy(instance->connect);
    instance->createServer->destroy(instance->createServer);
    free(instance);
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
    self.update = updateStartMenu;

    // load background
    self.instance->surface = IMG_Load("src/menu.jpg");

    // create button
    SDL_Color txtColor = {255, 255, 255, 255};
    SDL_Color bgColor = {0, 0, 0, 255};
    SDL_Rect connectRect = {100, 100, 200, 60};
    SDL_Rect ServerRect = {100, 200, 200, 60};
    self.instance->connect = newTextButton("Connect", txtColor, bgColor, 24, connectRect);
    self.instance->createServer = newTextButton("CreateServer", txtColor, bgColor, 24, ServerRect);

    return &self;
}