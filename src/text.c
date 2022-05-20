#include "SDL2/SDL_ttf.h"
#include "text.h"
#include "string.h"
#include "GameEngin.h"
#include <stdio.h>

void renderTTF(void *self);
void destroyTTF(void *self);

struct textInstance
{
    int y;
    int x;
    int size;
    char *text;
    SDL_Color color;
    SDL_Renderer *renderer;
    TTF_Font *font;
    SDL_Surface *text_surface;
    SDL_Rect rect;
};
void centerText(void *self, SDL_Rect outerRect)
{
    TextInstance *textInstance = ((Text *)self)->instance;
    textInstance->rect.x = (outerRect.x + ((outerRect.w) / 2)) - ((textInstance->rect.w) / 2);
    textInstance->rect.y = (outerRect.y + ((outerRect.h) / 2)) - ((textInstance->rect.h) / 2);
}

void renderTTF(void *self)
{
    TextInstance *instance = ((Text *)self)->instance;
    GameEngin *GE = getGameEngin();
    SDL_Renderer *renderer = GE->getRenderer(GE);

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, instance->text_surface);

    SDL_RenderCopy(renderer, texture, NULL, &instance->rect);

    return;
}

void destroyTTF(void *self)
{
    Text *text = (Text *)self;
    SDL_DestroyRenderer(text->instance->renderer);
    free(text->instance);
    free(text);
    TTF_Quit();

    return;
}
void setTextColor(void *self, SDL_Color color)
{
    TextInstance *instance = ((Text *)self)->instance;
    instance->color = color;
    if (!(instance->text_surface = TTF_RenderText_Solid(instance->font, instance->text, instance->color)))
        printf("Oh My Goodness, render error : %s\n", TTF_GetError());
}

Text *newText(char *text, int x, int y, int size, SDL_Color color)
{
    Text *self = malloc(sizeof(Text));
    self->instance = malloc(sizeof(TextInstance));
    self->instance->text = malloc(sizeof(char) * strlen(text) + 1);

    self->render = renderTTF;
    self->destroy = destroyTTF;
    self->setColor = setTextColor;
    self->centerText = centerText;

    self->instance->color = color;
    self->instance->text = text;

    self->instance->font = TTF_OpenFont("./font/Inter-SemiBold.ttf", size);
    if (self->instance->font == NULL)
        printf("Oh My Goodness, font error : %s\n", TTF_GetError());

    if (!(self->instance->text_surface = TTF_RenderText_Solid(self->instance->font, self->instance->text, self->instance->color)))
        printf("Oh My Goodness, render error : %s\n", TTF_GetError());

    self->instance->rect.x = x;
    self->instance->rect.y = y;
    self->instance->rect.w = self->instance->text_surface->w;
    self->instance->rect.h = self->instance->text_surface->h;

    return self;
}