#include "healthBar.h"
#include "GameEngin.h"
#include "SDL2/SDL.h"

#define WIDTH 32
#define HEIGHT 4


struct healthBarInstance
{
    int x;
    int y;
    int health;
    bool isLocal;
};

void setHealthBar(void *self, int health, int x, int y, bool isLocal)
{
    HealthBarInstance *instance = ((HealthBar *)self)->instance;
    
    instance->health = health;
    instance->isLocal = isLocal;
    instance->x = x;
    instance->y = y;
}

void showHealthBar(void *self)
{
    HealthBarInstance *instance = ((HealthBar *)self)->instance;
    GameEngin *engin = getGameEngin();

    SDL_Rect box = {
        instance->x - WIDTH/2 + 12,
        instance->y - HEIGHT + 4,
        WIDTH,
        HEIGHT,
    };
    SDL_Rect hp = {
        instance->x - WIDTH/2 + 12 + 1,
        instance->y - HEIGHT + 4 + 1,
        instance->health,
        HEIGHT-2,
    };

    SDL_Renderer *ren =  engin->getRenderer(engin);
    
    SDL_SetRenderDrawColor(ren, 255,0,0,255);
    if(instance->isLocal)
        SDL_SetRenderDrawColor(ren, 0,0,255,255);


    SDL_RenderDrawRect(engin->getRenderer(engin), &box);

    if(instance->health == 0)
        return;
    SDL_SetRenderDrawColor(ren, 0,255,0,255);
    SDL_RenderDrawRect(engin->getRenderer(engin), &hp);
}

HealthBar * newHealthBar()
{
    HealthBar * self = malloc(sizeof(HealthBar));

    self->instance = malloc(sizeof(HealthBarInstance));
    self->instance->health = 100;

    self->show = showHealthBar;
    self->set = setHealthBar;

    return self;
}