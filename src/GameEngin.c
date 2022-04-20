#include "GameEngin.h"
#include "DataTypes.h"
#include "TextureManager.h"
#include <stdio.h>
#include "Animation.h"
#include "EntityManager.h"
#include "Animation.h"
#include "EventHandler.h"
#include "Warrior.h"
#include "FpsManager.h"
#include "map.h"

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

    TextureManager *texterManager = getTextureManager();
    texterManager->load(texterManager, "body", "./assets/image.png");

    EntityManager *entityManager = getEntityManager();

    //Warrior creation
    Warrior *warrior = createWarrior();
    entityManager->add(entityManager, "warrior-1", warrior);//add to entity manager list

    initMap();//!

    Engin->instance->isRunning = true;


    return 1;
}

void handleEvents(void *self)
{
    EventHandler *eventHandler = getEventHandler();
    eventHandler->listen(eventHandler);

    EntityManager *entityManager = getEntityManager();

    //Warrior Events
    Warrior *warrior = entityManager->getByID(entityManager, "warrior-1");
    warrior->eventHandler(warrior);


}

void handleUpdates(void *self)
{
    // updates functions go here !!!
    FpsManager *fpsManager = getFpsManager();
    float dt = fpsManager->getDelta(fpsManager);
    //printf("dt: %f\n", dt);


    EntityManager *entityManager = getEntityManager();

    //Warrior update
    Warrior *warrior = entityManager->getByID(entityManager, "warrior-1");
    warrior->update(warrior, dt);

    // updates functions go here !!!
}

void quit(void *self)
{
    GameEngin *Engin = ((GameEngin *)self);
    Engin->instance->isRunning = false;
}

void handleRenders(void *self)
{
    GameEngin *Engin = ((GameEngin *)self);
    SDL_SetRenderDrawColor(Engin->instance->renderer, 135, 206, 235, 255);
    SDL_RenderClear(Engin->instance->renderer);

    // render functions go here !!!

    EntityManager *entityManager = getEntityManager();

    showMap();//!render map behind player
    //Warrior Render
    Warrior *warrior = entityManager->getByID(entityManager, "warrior-1");
    warrior->render(warrior);

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

    TextureManager *texterManager = getTextureManager();
    texterManager->destroy(texterManager);

    EntityManager *entityManager = getEntityManager();
    entityManager->destroy(entityManager);

    EventHandler *eventHandler = getEventHandler();
    eventHandler->destroy(eventHandler);
    // destroy all assets here !!!

    // destroy functions go here !!!
    printf("Engine Cleaned");
    return false;
}

SDL_Renderer *getRenderer(void *self)
{
    return ((GameEngin *)self)->instance->renderer;
}

GameEngin *getGameEngin()
{
    static GameEngin self;

    if (self.instance != NULL)
        return &self;

    self.instance = malloc(sizeof(EnginInstance *));
    self.init = init;
    self.isRunning = isRunning;
    self.handleEvents = handleEvents;
    self.handleUpdates = handleUpdates;
    self.handleRenders = handleRenders;
    self.destroyEngine = destroyEngine;
    self.getRenderer = getRenderer;
    self.quit = quit;
    printf("GameEngin initialized\n");

    return &self;
}
