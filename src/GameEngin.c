#include <stdio.h>
#include<time.h>
#include "GameEngin.h"
#include "DataTypes.h"
#include "TextureManager.h"
#include "Animation.h"
#include "EntityManager.h"
#include "Animation.h"
#include "InputHandler.h"
#include "Warrior.h"
#include "FpsManager.h"
#include "Cube.h"
#include "map.h"
#include "Bullet.h"
#include "networkClient.h"
#include "audio.h"
#include "text.h"

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
    if (!isRenderSucced)
        return 0;

    // init map
    MapManager *mapManager = getMapManager();
    mapManager->initMap(mapManager); //! initializes map

    Audio *audio = newAudio();
    audio->init();
    audio->backgroud(audio, "assets/back.wav", 10);

    EntityManager *entityManager = getEntityManager();
    // Warrior creation handel network

    NetworkClient *network = getNetworkClient();
    WarriorCreation wa = {network->getTCPID(network),100, 0};
    network->TCPbroadCast(network, &wa, sizeof(WarriorCreation), 2);

    Warrior *warrior = createWarrior(100*network->getTCPID(network)+100, 0, network->getTCPID(network), -1,true);
    char * wID = warrior->getID(warrior);
    entityManager->add(entityManager, wID, warrior); // add to entity manager list
    printf("warrior id %s\n",wID);
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
{
    GameEngin *Engin = ((GameEngin *)self);
    // render background
    TextureManager *textureManager = getTextureManager(textureManager);
    textureManager->load(textureManager, "moon", "./assets/moon.jpg");
    SDL_Rect srcRect = {0, 0, 1000, 500};
    SDL_Rect destRect = {0, 0, 1000, 500};
    textureManager->draw(textureManager, "moon", srcRect, destRect, 1); //! draw bakgrundsbild

    // render map
    MapManager *mapManger = getMapManager();
    mapManger->showMap(mapManger);

    EntityManager *entityManager = getEntityManager();
    entityManager->renderAll(entityManager);

    SDL_Color color = {100,100,100,0};
    Text *text = newText("HELLO", 100, 100, 26, color);
    text->render(text);

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

    // destroy texture manager
    TextureManager *texterManager = getTextureManager();
    texterManager->destroy(texterManager);

    // destroy entityManager
    EntityManager *entityManager = getEntityManager();
    entityManager->destroy(entityManager);

    // destroy inputHandler
    InputHandler *inputHandler = getInputHandler();
    inputHandler->destroy(inputHandler);

    // destroy mapManager
    MapManager *mapManager = getMapManager();
    mapManager->destroyMap(mapManager);

    //destroy audio
    Audio *audio = newAudio();
    audio->destroy(audio);    
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
    if ((SDL_Init(SDL_INIT_VIDEO| SDL_INIT_AUDIO)) < 0)
    {
        printf("Error: SDL failed to initialize\nSDL Error: '%s'\n", SDL_GetError());
        return false;
    }

    Engin->instance->window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, fullScreen);
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

    if(!TTF_WasInit() && TTF_Init()==-1) 
    {
        printf("TTF_Init: %s\n", TTF_GetError());
        exit(1);
        return false;
    }

    return true;
}