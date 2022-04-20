/*
    animation class take care of sprite animation, every object that have sprite animation should have an animation class in it. a sprite is basicly a frame, on a image it should have multiple fram which caled sprite. This class will draw one frame after others to create an animation.

    void set(void *self, char *id, int spriteWidth, int spriteHeight, int row, int totalFrame, int animSpeed, SDL_RendererFlip flip);
        Set animation. this function uses when you init a object that have animation or when you whant to change the animation of that object
        id: id to the texture, it need to be loaded on textureManager first;
        spriteWidth, spriteHeight: width and height of sprite

    void draw(void *self, float x, float y);
        Draw the animation. This function should be called in side the render function of corresponding object. it should be called on every render. this function is smart enough to know when to draw which frame based on parameter that got set by set function.
        x,y is the location that the animation will be rendered;

    void (*destroy)(void *self)
        destroy animation. it should be called when its owner object get destroed.
*/

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
