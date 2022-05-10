#include "SDL2/SDL_ttf.h"
#include "text.h"
#include "string.h"


void init(void *self);
void render(void *self);
void destroy(void *self);

struct textInstance
{
    int y;
    int x;
    int size;
    char *text;
    Uint8 color;
    SDL_Renderer *renderer;
	TTF_Font *font;
};

void initTTF(void *self)
{
    if (!TTF_WasInit() && TTF_Init() == -1)
    {
        printf("TTF_Init: %s\n", TTF_GetError());
        exit(1);
    }

    return;
}

void render(void *self)
{
    TextInstance *instance = ((Text *)self)->instance;

    return;
}

void destroyTTF(void *self)
{

    TTF_Quit();

    return;
}

Text *newText(char *text, int x, int y, int size, Uint8 color)
{
    Text *self = malloc(sizeof(Text));
    self->instance = malloc(sizeof(TextInstance));
	self->instance->text = malloc(sizeof(char) * strlen(text) +1);

    self->render = render;
    self->destroy = destroyTTF;

    self->instance->y = y;
    self->instance->x = x;
    self->instance->color = color;
    self->instance->text = text;
	self->instance->font = TTF_OpenFont("../font/font.ttf", size);

    return self;
}