#include "GameEngin.h"
#include "DataTypes.h"
#include "TextureManager.h"
#include <stdio.h>
#include "Animation.h"
#include "EntityManager.h"
#include "Animation.h"
#include "InputHandler.h"
#include "Warrior.h"
#include "FpsManager.h"
#include "Cube.h"
#include "map.h"

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

    EntityManager *entityManager = getEntityManager();
    // Warrior creation
    Warrior *warrior = createWarrior();
    entityManager->add(entityManager, "warrior-1", warrior);//add to entity manager list

    MapManager *mapManager = getMapManager();

    mapManager->initMap(mapManager);//! initializes map


    // cube creation
    Cube *cube = newCube();
    entityManager->add(entityManager, "cube-1", cube);

    Engin->instance->isRunning = true;


    return 1;
}

void handleEvents(void *self)
{
    EntityManager *entityManager = getEntityManager();

    // Warrior Events
    Warrior *warrior = entityManager->getByID(entityManager, "warrior-1");
    warrior->eventHandler(warrior);

    // cube
    Cube *cube = entityManager->getByID(entityManager, "cube-1");
    cube->events(cube);
}

void handleUpdates(void *self)
{
    // updates functions go here !!!
    FpsManager *fpsManager = getFpsManager();
    float dt = fpsManager->getDelta(fpsManager);
    // printf("dt: %f\n", dt);

    EntityManager *entityManager = getEntityManager();

    Warrior *warrior = entityManager->getByID(entityManager, "warrior-1");
    warrior->update(warrior, dt);

    Cube *cube = entityManager->getByID(entityManager, "cube-1");
    cube->update(cube, dt);

    // updates functions go here !!!
}
void handleRenders(void *self)
{
    GameEngin *Engin = ((GameEngin *)self);
    SDL_SetRenderDrawColor(Engin->instance->renderer, 135, 206, 235, 255);
    SDL_RenderClear(Engin->instance->renderer);

    // render functions go here !!!

    EntityManager *entityManager = getEntityManager();

    MapManager *mapManger = getMapManager();
   
    mapManger->showMap(mapManger);//!render map behind player
    //Warrior Render
    Warrior *warrior = entityManager->getByID(entityManager, "warrior-1");
    warrior->render(warrior);

    // render Cube
    SDL_SetRenderDrawColor(Engin->instance->renderer, 0, 0, 255, 255);
    Cube *cube = entityManager->getByID(entityManager, "cube-1");
    cube->render(cube);

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

    InputHandler *inputHandler = getInputHandler();
    inputHandler->destroy(inputHandler);
    // destroy all assets here !!!

    // destroy functions go here !!!
    MapManager *mapManager = getMapManager();
    mapManager->destroyMap(mapManager);
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
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
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

    return true;
}