/*
    cube is a test object, which render a rectangle to the screen. it could move around by click on the winow. It have position, velocity and a hitbox. this object used to test colition detection.
*/

#pragma once
#include <stdbool.h>
#include "SDL2/SDL.h"



typedef struct bulletInstance BulletInstance;

typedef struct bullet
{
    void (*render)(void *self);
    void (*update)(void *self, float dt);
    void (*events)(void *self);
    char* (*getID)(void *self);
    void (*destroy)(void *self);
    int (*getIntId)(void *self);

    BulletInstance *instance;
} Bullet;

Bullet *newBullet(SDL_FPoint pos, SDL_FPoint vel, int id, bool isLocal,int bulletType);