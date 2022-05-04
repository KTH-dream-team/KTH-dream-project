// inspriration: https://github.com/OneLoneCoder/olcPixelGameEngine/blob/master/Videos/OneLoneCoder_PGE_Rectangles.cpp

/*
    Klassen innehåller funktioner för att checka collision och resolve collision.
    CollisionManager är static vilket innebär det finns endast en collisionManager instance på hela programmet.

    bool (*PointVsRect)(float x, float y, SDL_Rect *r);
        retunerar true om punkten (x,y) befinner sig i rektangel r

    bool (*RectVsRect)(SDL_Rect *r1, SDL_Rect *r2);
        returnerar true om rektangel r1 och r2 overlappar varann

    bool (*RayVsRect)(SDL_FPoint rayOrigin, SDL_FPoint rayDir, SDL_Rect r, SDL_FPoint *normal, SDL_FPoint *contact, float *t);
        funtionenn tar in rayOrigin som är rays origin, rayDir  som  är rays direktion och en rektangel r.
        Den skickar ut kontakt punkten, normalen, och avstånd t i procent
        returnerar true om Ray intersekt mev rektangel r

    bool (*DynamicRectVsRect)(SDL_Rect DRect, SDL_FPoint dir, SDL_Rect SRect, SDL_FPoint *normal, SDL_FPoint *contact, float *t, unsigned int dt);
        funtionen checkar om en dynamic rektangen kommer att överlappa en statisk rektangen på nästa frame
        DRect: Dynamic rektangel
        dir: direktions vectorn för dynamic rektangel
        SRect: statist rektangel
        normal: funtionen skickar ut normalen av kontakt yta

        return värden är true om rektanglarna kommer att överlappa.

*/

#pragma once
#include "SDL2/SDL.h"
#include <stdbool.h>

typedef struct collisionManagerInstance CollisionManagerInstance;

typedef struct colisionManager
{
    bool (*PointVsRect)(float x, float y, SDL_Rect *r);
    bool (*RectVsRect)(SDL_Rect *r1, SDL_Rect *r2);
    bool (*RayVsRect)(SDL_FPoint rayOrigin, SDL_FPoint rayDir, SDL_Rect r, SDL_FPoint *normal, SDL_FPoint *contact, float *t);
    bool (*DynamicRectVsRect)(SDL_Rect DRect, SDL_FPoint dir, SDL_Rect SRect, SDL_FPoint *normal, float *t, float dt);
    bool (*ResolveDynamicRectVsRect)(SDL_Rect DRect, SDL_FPoint *dir, SDL_Rect SRect, float dt);
    bool (*ResolveBulletVSRect)(SDL_Rect DRect, SDL_FPoint *dir, SDL_Rect SRect, float dt);

    CollisionManagerInstance *instance;
} CollisionManager;

CollisionManager *GetCollisionManager();
