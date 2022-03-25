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

bool load(void *self, char *id, char *filename)
{
    // load texture;
    SDL_Surface *surface = IMG_Load(filename);
    if (surface == NULL)
    {
        SDL_Log("Failed to load texture: %s %s", filename, SDL_GetError());
        return false;
    }

    // Create texture from surface
    GameEngin *Engine = createGameEngin();
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
    ((TextureManager *)self)->instance->textureList->destroy(((TextureManager *)self)->instance->textureList);
    free(((TextureManager *)self)->instance->textureList);
    free(((TextureManager *)self)->instance);
    printf("TextureManager destroyed\n");
}

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

void draw(void *self, char *id, SDL_Rect srcRect, SDL_Rect destRect, SDL_RendererFlip flip)
{
    GameEngin *Engine = createGameEngin();
    SDL_Renderer *ren = Engine->getRenderer(Engine);
    SDL_Texture *tex = getTextureByID(self, id);

    SDL_RenderCopyEx(ren, tex, &srcRect, &destRect, 0, NULL, flip);
}

void drawFrame(void *self, char *id, int x, int y, int w, int h, int row, int col, SDL_RendererFlip flip)
{
    SDL_Rect srcRect = {w * col, h * row, w, h};
    SDL_Rect destRect = {x, y, w / 1.5, h / 1.5};

    GameEngin *Engine = createGameEngin();
    SDL_Renderer *ren = Engine->getRenderer(Engine);
    SDL_Texture *tex = getTextureByID(self, id);

    SDL_RenderCopyEx(ren, tex, &srcRect, &destRect, 0, NULL, flip);
}

TextureManager *createTextureManager()
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