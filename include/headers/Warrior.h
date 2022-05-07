/*
    warrior function is a test class used to test out our components
    testing animation, trasform, rigidbody
*/
#pragma once
#include <stdbool.h>
#include "SDL2/SDL.h"

typedef struct warriorInstance WarriorInstance;

typedef struct warrior
{
    void (*update)(void *self, float dt);
    void (*render)(void *self);
    void (*events)(void *self);
    void (*destroy)(void *self);
    void (*updatePossition)(void *self, float x, float y);
    char *(*getID)(void*self);
    bool (*checkColisionWarriorVsBullet)(void *self,SDL_Rect bulletDRect,SDL_FPoint *vel, float dt);

    WarriorInstance *instance;
} Warrior;

Warrior *createWarrior(float x, float y, int id, int networkId, bool isLocal);