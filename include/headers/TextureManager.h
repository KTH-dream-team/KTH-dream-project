/*
    Texture manager is a central place to manage all textures. There is only one instance of TextureManager in the 
    whole program. All texture should be loaded in here before use to avoid texture duplication and can be destroyed
    when program closed. 

    bool load(void *self, char *id, char *filename)
        load a texture to TextureManager, all unique texture should go with it own unique id (recommened to use fileName
        as ID so one file can only be loaded once). If this function get call twice with the same ID the texture will not
        be load again. 
        id: unique id of the texture (Recommended to use fileName as ID)
        filename: path to texture file.

    void drop(void *self, char *id)
        delete a specific texture from textureManager
        id: ID of texture to be deleted
        !TODO: need to be implement

    SDL_Texture *getTextureByID(void *self, char *id)
        return a SDL_Texture that corresponding to a given ID;

    void draw(void *self, char *id, SDL_Rect srcRect, SDL_Rect destRect, SDL_RendererFlip flip)
        draw a static texture to window.
        id: identifier to a texture.
        srcRect: source rect, indicate whick part of the texture get drawn
        destRect: destination rect, position and size of the texture on window. 
        flip: flip direction of the texture(horizontal, vertical or none)

    void drawFrame(void *self, char *id, float x, float y, int w, int h, int row, int frame, float scale, SDL_RendererFlip flip);
        this function is almost like draw function above but get desiged specific for drawing sprite animation.
        sprite texture should be designed as each row contain all frame to one animation, and one texture should contain all animation
        of one object.
        id: identifier to a texture.
        x, y: codinate of destination rect (where on window)
        w, h: size of each sprite/frame in pixels
        row: which animation to play
        frame: which frame to draw
        scale: scale factor(1 equal 1:1 ratio, 2 equal 2:1 ratio)
        flip: flip direction of texture

    void (*destroy)(void *self)
        this function should be call on program end. 
*/



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
    void (*drawFrame)(void *self, char *id, float x, float y, int w, int h, int row, int frame, float scale, SDL_RendererFlip flip);

    TextureManagerInstance *instance;
} TextureManager;

TextureManager *getTextureManager();