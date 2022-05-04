#include "GameEngin.h"
#include "DataTypes.h"
#include "TextureManager.h"
#include <stdio.h>
#include <string.h>
#include "Animation.h"
#include "EntityManager.h"
#include "Animation.h"
#include "InputHandler.h"
#include "Warrior.h"
#include "OtherWarrior.h"
#include "FpsManager.h"
#include "Cube.h"
#include "map.h"
#include "Bullet.h"
#include "networkClient.h"
#include "audio.h"


struct enginInstance
{
    bool isRunning;
    SDL_Window *window;
    SDL_Renderer *renderer;
};

bool initSDL(GameEngin *Engin, char *title, int width, int height, int fullScreen);
bool init(void *self, char *title, int width, int height, int fullScreen)
{
    GameEngin *Engin = ((GameEngin *)self);
    bool isRenderSucced = initSDL(Engin, title, width, height, fullScreen);
    if (!isRenderSucced){
        return 0;}
    NetworkClient *client = getNetworkClient();
    
    //init map
    MapManager *mapManager = getMapManager();
    mapManager->initMap(mapManager);//! initializes map

    EntityManager *entityManager = getEntityManager();
    // Warrior creation
    Warrior *warrior = createWarrior(client->TCPgetID(client));

    static char myWarrior[10] = "Warrior-";
    char clientID = '0' + client->TCPgetID(client);
    printf("TCPgetID: %d, string: %s, charID: %c\n", client->TCPgetID(client), myWarrior, clientID);
    strncat(myWarrior,&clientID,1);

    entityManager->add(entityManager, myWarrior, warrior);//add to entity manager list

/*
    entityManager->add(entityManager, "OtherWarrior-0", otherwarrior0);
    entityManager->add(entityManager, "OtherWarrior-1", otherwarrior1);
    entityManager->add(entityManager, "OtherWarrior-2", otherwarrior2);
    entityManager->add(entityManager, "OtherWarrior-3", otherwarrior3);

    OtherWarrior *otherwarrior0 =  createOtherWarrior(0, 0, 0);
    OtherWarrior *otherwarrior1 =  createOtherWarrior(1, 1, 1);
    OtherWarrior *otherwarrior2 =  createOtherWarrior(2, 2, 2);
    OtherWarrior *otherwarrior3 =  createOtherWarrior(3, 3, 3);
*/
    // Other warrior creation
    for(int i=0;i<MAX_CLIENTS;i++)
    {
        if(i==client->TCPgetID(client)) continue;
        if(i==0)
        {
            OtherWarrior *otherwarrior0 =  createOtherWarrior(i, i, i);
            entityManager->add(entityManager, "OtherWarrior-0", otherwarrior0);
        }
        else if(i==1)
        {
            OtherWarrior *otherwarrior1 =  createOtherWarrior(i, i, i);
            entityManager->add(entityManager, "OtherWarrior-1", otherwarrior1);
        }
        else if(i==2)
        {
            OtherWarrior *otherwarrior2 =  createOtherWarrior(i, i, i);
            entityManager->add(entityManager, "OtherWarrior-2", otherwarrior2);
        }
        else if(i==3)
        {
            OtherWarrior *otherwarrior3 = createOtherWarrior(i, i, i);
            entityManager->add(entityManager, "OtherWarrior-3", otherwarrior3);
        }
    }

    Engin->instance->isRunning = true;

    return 1;
}

void handleEvents(void *self)
{
    EntityManager *entityManager = getEntityManager();
    entityManager->handleAllEvents(entityManager);
}

void handleUpdates(void *self)
{
    // updates functions go here !!!
    FpsManager *fpsManager = getFpsManager();
    float dt = fpsManager->getDelta(fpsManager);

    EntityManager *entityManager = getEntityManager();
    entityManager->updateAll(entityManager, dt);
}

void handleRenders(void *self)
{//todo implemnt neew bakgrund
    GameEngin *Engin = ((GameEngin *)self);
    SDL_RenderClear(Engin->instance->renderer);
    // SDL_SetRenderDrawColor(Engin->instance->renderer, 135, 206, 235, 255);//färg bakgrund
    
    //render background
    TextureManager *textureManager = getTextureManager(textureManager);
    textureManager->load(textureManager,"moon","./assets/moon.jpg");
    SDL_Rect srcRect = {0, 0, 1000, 500};
    SDL_Rect destRect = {0, 0, 1000, 500};
    textureManager->draw(textureManager,"moon",srcRect, destRect,1);//!draw bakgrundsbild
    
    //render map
    MapManager *mapManger = getMapManager();
    mapManger->showMap(mapManger);
    EntityManager *entityManager = getEntityManager();

    entityManager->renderAll(entityManager); //! Here inside segmentation fault
    
    // render functions go here !!!
    SDL_RenderPresent(Engin->instance->renderer);
}
bool destroyEngine(void *self)
{
    GameEngin *Engin = ((GameEngin *)self);
    SDL_Quit();
    SDL_DestroyWindow(Engin->instance->window);
    SDL_DestroyRenderer(Engin->instance->renderer);
    free(Engin->instance);

    //destroy texture manager
    TextureManager *texterManager = getTextureManager();
    texterManager->destroy(texterManager);

    //destroy entityManager
    EntityManager *entityManager = getEntityManager();
    entityManager->destroy(entityManager);

    //destroy inputHandler
    InputHandler *inputHandler = getInputHandler();
    inputHandler->destroy(inputHandler);

    //destroy mapManager
    MapManager *mapManager = getMapManager();
    mapManager->destroyMap(mapManager);

    //destroy networkClient
    NetworkClient *network = getNetworkClient();
    network->destroy(network);
    
    // destroy all assets here !!!

    // destroy functions go here !!!
    printf("Engine Cleaned");
    return false;
}
SDL_Renderer *getRenderer(void *self)
{
    return ((GameEngin *)self)->instance->renderer;
}
bool isRunning(void *self)
{
    return ((GameEngin *)self)->instance->isRunning;
}
void quit(void *self)
{
    GameEngin *Engin = ((GameEngin *)self);
    Engin->instance->isRunning = false;
}
GameEngin *getGameEngin()
{
    static GameEngin self;

    if (self.instance != NULL)
        return &self;

    self.instance = malloc(sizeof(EnginInstance *));
    self.init = init;
    self.handleEvents = handleEvents;
    self.handleUpdates = handleUpdates;
    self.handleRenders = handleRenders;
    self.destroyEngine = destroyEngine;
    self.getRenderer = getRenderer;
    self.isRunning = isRunning;
    self.quit = quit;

    return &self;
}

// helper functions
bool initSDL(GameEngin *Engin, char *title, int width, int height, int fullScreen)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        printf("Error: SDL failed to initialize\nSDL Error: '%s'\n", SDL_GetError());
        return false;
    }
    Audio *audio = newAudio();
    audio->init();
    audio->backgroud(audio, "assets/back.wav", 10);

    Engin->instance->window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height,fullScreen);
    if (!Engin->instance->window)
    {
        printf("Error: Failed to open window\nSDL Error: '%s'\n", SDL_GetError());
        return false;
    }

    Engin->instance->renderer = SDL_CreateRenderer(Engin->instance->window, -1, SDL_RENDERER_ACCELERATED);
    if (!(Engin->instance->window))
    {
        printf("Error: Failed to create renderer\nSDL Error: '%s'\n", SDL_GetError());
        return false;
    }

    return true;
}