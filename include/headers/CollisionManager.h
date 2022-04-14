#pragma once
#include "SDL2/SDL.h"
#include <stdbool.h>

typedef struct collisionManagerInstance CollisionManagerInstance;

typedef struct colisionManager
{
    bool (*PointVsRect)(float x, float y, SDL_Rect *r);
    bool (*RectVsRect)(SDL_Rect *r1, SDL_Rect *r2);
    bool (*RayVsRect)(SDL_FPoint rayOrigin, SDL_FPoint rayDir, SDL_Rect r, SDL_FPoint *normal, SDL_FPoint *contact, float *t);
    bool (*DynamicRectVsRect)(SDL_Rect DRect, SDL_FPoint dir, SDL_Rect SRect, SDL_FPoint *normal, SDL_FPoint *contact, float *t, unsigned int dt);

    CollisionManagerInstance *instance;
} CollisionManager;

CollisionManager *GetCollisionManager();