#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "GameEngin.h"
#include "SDL2/SDL_ttf.h"
#include "menu.h"
#include "TextureManager.h"

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 500

struct ttfInstance
{
    SDL_Rect *rect;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    SDL_Surface *surface;
    SDL_Color *textColor;
    char *text;
};

SDL_Renderer *createRenderer(SDL_Window *pWindow);
void createTextBox(SDL_Renderer *renderer, int x, int y, char *text, TTF_Font *font, SDL_Texture **texture, SDL_Rect *rect);
void showRenderer(void *self);


SDL_Renderer *createRenderer(SDL_Window *pWindow)
{
    SDL_Renderer *renderer = SDL_CreateRenderer(pWindow,-1,SDL_RENDERER_ACCELERATED);
    return renderer;
}

bool ttfInit(void * self)
{
    printf("ttfInit_1\n");
    if(TTF_Init()==-1)
    {
        printf("TTF_Init and TTF_wasInit: %s\n",TTF_GetError());
        return false;
    }
    printf("ttfInit_2\n");

    TTF *ttf = ((TTF*)self);
    TTFInstance *instance = ((TTF*)self)->instance;
    GameEngin *GE = getGameEngin();
    SDL_Rect rect;
    instance->renderer = GE->getRenderer(GE);
    printf("ttfInit_3\n");
    /*
    instance->textColor->a = (Uint8) 255;
    printf("ttfInit_3.1\n");

    instance->textColor->b = (Uint8) 0;
    printf("ttfInit_3.2\n");

    instance->textColor->r = (Uint8) 0;
    printf("ttfInit_3.3\n");

    instance->textColor->g = (Uint8) 0;
    printf("ttfInit_3.4\n");
    */
    SDL_Color textColor = {0,0,0,255};

    printf("ttfInit_5\n");
    
    char *font_path;
    font_path = "./font/PaletteMosaic-Regular.ttf";
    TTF_Font *font = TTF_OpenFont(font_path, 24);
    printf("ttfInit_6\n");

    
    if (font == NULL) {
        //fprintf(stderr, "error: font not found\n");
        printf("Error: font not found\n");
        exit(EXIT_FAILURE);
        return false;
    }
    printf("ttfInit_7\n");


    static char textName[15];
    strcpy(textName,"KTH DREAM TEAM");
    printf("ttfInit_8\n");


    printf("Creating surface\n");
    instance->surface = TTF_RenderText_Solid(font, textName, textColor);
    instance->texture = SDL_CreateTextureFromSurface(GE->getRenderer(GE), instance->surface);
    printf("ttfInit_9\n");

    
    printf("Surface done\n");
    /*
    instance->rect->x = SCREEN_WIDTH / 2;
    printf("ttfInit_10\n");

    instance->rect->y = SCREEN_HEIGHT / 2;
    printf("ttfInit_11\n");

    instance->rect->w = instance->surface->w;
    printf("ttfInit_12\n");

    instance->rect->h = instance->surface->h;   
    printf("ttfInit_13\n");
    */
/*
    rect.x = SCREEN_WIDTH / 2;
    rect.y = SCREEN_HEIGHT / 2;
    */
    rect.x = 500;
    rect.y = 250;
    printf("1\n");
    rect.w = instance->surface->w;
    printf("2\n");
    rect.h = instance->surface->h;
    printf("3\n");

    *(instance->rect) = rect;
    printf("4\n");

    printf("Processing FreeSurface()\n");
    SDL_FreeSurface(instance->surface);
    printf("End of ttfInit\n");
    //createTextBox(instance->renderer,instance->rect.w, instance->rect.h,);
    int count=0;
    while(count<10000)
    {
        count++;
        ttf->showTTFMenu(ttf);
        printf("While count:%d\n",count);

    }
    printf("While done\n");
    return true;
}

void showTTFMenu(void *self)
{
    TTFInstance *instance = ((TTF*)self)->instance;
    printf("Menu is processing\n");
    SDL_SetRenderDrawColor(instance->renderer, 0, 100, 100, 255);
    SDL_RenderClear(instance->renderer);
    SDL_RenderCopy(instance->renderer, instance->texture, NULL, instance->rect);
    SDL_RenderPresent(instance->renderer);
    printf("Rendering\n");
}

bool destroyTTF(void *self)
{
    printf("destroyTTF\n");
    printf("ttf_destroyTTF_1\n");
    TTF *ttf = ((TTF*)self);
    printf("ttf_destroyTTF_2\n");
    SDL_DestroyTexture(ttf->instance->texture);
    printf("ttf_destroyTTF_3\n");
    printf("destroy texture successfull\n");
    free(ttf->instance);
    printf("ttf_destroyTTF_4\n");
    printf("ttf_destroyTTF_5\n");
    return true;
}

TTF *getMenu()
{
    static TTF self;
    if(self.instance != NULL)
        return &self;
    self.instance = malloc(sizeof(TTFInstance *));
    self.instance->rect = malloc(sizeof(SDL_Rect));
    self.ttfInit = ttfInit;
    self.showTTFMenu = showTTFMenu;
    self.destroy = destroyTTF;
    
    return &self;
}

void createTextBox(SDL_Renderer *renderer, int x, int y, char *text, TTF_Font *font, SDL_Texture **texture, SDL_Rect *rect) 
{
    int text_width;
    int text_height;
    SDL_Surface *surface;
    SDL_Color textColor = {255, 255, 255, 0};

    surface = TTF_RenderText_Solid(font, text, textColor);
    *texture = SDL_CreateTextureFromSurface(renderer, surface);
    text_width = surface->w;
    text_height = surface->h;
    SDL_FreeSurface(surface);
    rect->x = x;
    rect->y = y;
    rect->w = text_width;
    rect->h = text_height;
}