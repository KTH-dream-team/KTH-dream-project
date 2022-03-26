#pragma once
#include "SDL2/SDL.h"
#include "DataTypes.h"

typedef struct animationInstance AnimationInstance;

typedef struct animation
{
    void (*update)(void *self);
    void (*draw)(void *self, float x, float y);
    void (*set)(void *self, char *id, int spriteWidth, int spriteHeight, int row, int totalFrame, int animSpeed, SDL_RendererFlip flip);
    void (*destroy)(void *self);

    AnimationInstance *instance;

} Animation;

Animation *newAnimation();
