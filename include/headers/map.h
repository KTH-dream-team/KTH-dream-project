#pragma once
#include <stdbool.h>
#include "SDL2/SDL.h"

typedef struct mapManagerInstance MapManagerInstance;

typedef struct mapManager{
    void(*showMap)(void *self);//draws the map on screen using drw from texture manager
    void(*initMap)(void *self);//initialize the map load textuers creat 2d array
    void(*dig)(void *self,int x,int y);//remove blocks from the map when clicking on mouse and holding Q
    void(*digNoSend)(void *self,int x,int y);//remove blocks from the map no network send 
    void(*buildNoSend)(void *self,int x,int y,int blockType);//builds blocks on the map no network send x and y in blockIndex
    void(*build)(void *self,int x,int y,int blockType);//place blocks on mp when clicking on mouse and holding E
    void(*destroyMap)(void *self);//destroy the map
    void(*show)(void *self);//debug
    int(*checkColision)(void *self,SDL_Rect dRect, SDL_FPoint *dir, float dt,int collisionType,int *blockTypePtr);

    MapManagerInstance *instance;
}MapManager;


MapManager *getMapManager();