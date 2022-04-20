#pragma once
#include "SDL2/SDL.h"
#include "DataTypes.h"
#include "ArrayList.h"

typedef struct textureManagerInstance TextureManagerInstance;

typedef struct textureManager
{
    bool (*load)(void *self, char *id, char *filename);
    void (*drop)(void *self, char *id);
    void (*destroy)(void *self);
    SDL_Texture *(*getTextureByID)(void *self, char *id);
    void (*draw)(void *self, char *id, SDL_Rect srcRect, SDL_Rect destRect, SDL_RendererFlip flip);
    void (*drawFrame)(void *self, char *id, float x, float y, int w, int h, int row, int frame, SDL_RendererFlip flip);

    TextureManagerInstance *instance;

} TextureManager;

TextureManager *getTextureManager();