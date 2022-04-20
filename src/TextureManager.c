#include "TextureManager.h"
#include "ArrayList.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL.h"
#include "GameEngin.h"
#include "DataTypes.h"
#include <stdio.h>

typedef struct textureMap
{
    char *id;
    SDL_Texture *texture;
} TextureMap;

struct textureManagerInstance
{
    ArrayList *textureList;
};

SDL_Texture *getTextureByID(void *self, char *id)
{
    ArrayList *textureList = ((TextureManager *)self)->instance->textureList;
    int lenght = textureList->getLength(textureList);

    for (int i = 0; i < lenght; i++)
    {
        TextureMap *t = (TextureMap *)(textureList->indexOf(textureList, i));
        if (strcmp(t->id, id) == 0)
            return t->texture;
    }
    return NULL;
}

bool load(void *self, char *id, char *filename)
{
    //check if texture is already loaded;
    SDL_Texture *t = getTextureByID(self, id);
    if(t != NULL)
        return false;   

    // load texture;
    SDL_Surface *surface = IMG_Load(filename);
    if (surface == NULL)
    {
        SDL_Log("Failed to load texture: %s %s", filename, SDL_GetError());
        return false;
    }

    // Create texture from surface
    GameEngin *Engine = getGameEngin();
    SDL_Renderer *ren = Engine->getRenderer(Engine);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(ren, surface);
    if (texture == NULL)
    {
        SDL_Log("Failed to load texture from surface: %s %s", filename, SDL_GetError());
        return false;
    }

    // Add texture to TextureManager
    TextureMap *temp = malloc(sizeof(TextureMap));
    temp->id = id;
    temp->texture = texture;
    ((TextureManager *)self)->instance->textureList->add(((TextureManager *)self)->instance->textureList, temp);

    return true;
}

void drop(void *self, char *id)
{
}

void destroy(void *self)
{
    TextureManagerInstance *instance = ((TextureManager *)self)->instance;
    instance->textureList->destroy(instance->textureList);
    // free(instance->textureList);
    free(instance);
    printf("TextureManager destroyed\n");
}

void draw(void *self, char *id, SDL_Rect srcRect, SDL_Rect destRect, SDL_RendererFlip flip)
{
    GameEngin *Engine = getGameEngin();
    SDL_Renderer *ren = Engine->getRenderer(Engine);
    SDL_Texture *tex = getTextureByID(self, id);

    SDL_RenderCopyEx(ren, tex, &srcRect, &destRect, 0, NULL, flip);
}

void drawFrame(void *self, char *id, float x, float y, int w, int h, int row, int frame, SDL_RendererFlip flip)
{
    SDL_Rect srcRect = {w * frame, h * row, w, h};
    SDL_Rect destRect = {x, y, w, h};

    GameEngin *Engine = getGameEngin();
    SDL_Renderer *ren = Engine->getRenderer(Engine);
    SDL_Texture *tex = getTextureByID(self, id);

    SDL_RenderCopyEx(ren, tex, &srcRect, &destRect, 0, NULL, flip);
}

TextureManager *getTextureManager()
{
    static TextureManager self;

    if (self.instance != NULL)
        return &self;

    self.instance = malloc(sizeof(TextureManagerInstance *));
    self.instance->textureList = malloc(sizeof(ArrayList *));
    self.instance->textureList = newArrayList();
    self.load = load;
    self.drop = drop;
    self.destroy = destroy;
    self.draw = draw;
    self.drawFrame = drawFrame;
    self.getTextureByID = getTextureByID;
    printf("Initialize TextureManager \n");

    return &self;
}