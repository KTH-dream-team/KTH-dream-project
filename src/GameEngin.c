#include "GameEngin.h"
#include "DataTypes.h"
#include "TextureManager.h"
#include <stdio.h>
#include "Animation.h"
#include "EntityManager.h"
#include "Animation.h"

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
    texterManager->load(texterManager, "warrior", "./assets/WariorAnim.png");

    EntityManager *entityManager = getEntityManager();

    Animation *anim = newAnimation();
    anim->set(anim, "warrior", 32, 32, 4, 10, 50, 0);
    anim->draw(anim, 0, 0);

    entityManager->add(entityManager, "anim1", anim);

    Engin->instance->isRunning = true;

    return 1;
}

void handleEvents(void *self)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            ((GameEngin *)self)->instance->isRunning = false;
            break;
        default:
            break;
        }
    }
}

void handleUpdates(void *self)
{
    // updates functions go here !!!
    // printf("Updates!!!! \n");
    EntityManager *entityManager = getEntityManager();
    Animation *anim = entityManager->getByID(entityManager, "anim1");
    anim->update(anim);

    // updates functions go here !!!
}

void handleRenders(void *self)
{
    GameEngin *Engin = ((GameEngin *)self);
    SDL_SetRenderDrawColor(Engin->instance->renderer, 255, 0, 0, 255);
    SDL_RenderClear(Engin->instance->renderer);

    // render functions go here !!!
    EntityManager *entityManager = getEntityManager();
    Animation *anim = entityManager->getByID(entityManager, "anim1");
    anim->draw(anim, 55, 55);

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
    //  destroy all assets here !!!

    // destroy functions go here !!!
    printf("Engine Cleaned");
    return false;
}

SDL_Renderer *getRenderer(void *self)
{
    return ((GameEngin *)self)->instance->renderer;
}

GameEngin *createGameEngin()
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
    printf("GameEngin initialized\n");

    return &self;
}
