#include "Bullet.h"
#include <stdlib.h>
#include "Transform.h"
#include "SDL2/SDL.h"
#include "GameEngin.h"
#include "InputHandler.h"
#include "CollisionManager.h"
#include "SDL2/SDL.h"

struct bulletInstance
{
    Transform *position;
    SDL_FPoint vel;
    SDL_Rect hitBox;
};

void renderBullet(void *self)
{
    BulletInstance *instance = ((Bullet *)self)->instance;
    GameEngin *engin = getGameEngin();

    SDL_Rect box = {
        instance->position->getX(instance->position) + instance->hitBox.x,
        instance->position->getY(instance->position) + instance->hitBox.y,
        instance->hitBox.w,
        instance->hitBox.h,
    };
    SDL_RenderDrawRect(engin->getRenderer(engin), &box);

    SDL_Rect SRect = {100, 200, 100, 50};
    SDL_RenderDrawRect(engin->getRenderer(engin), &SRect);
}
void updateBullet(void *self, float dt)
{
    BulletInstance *instance = ((Bullet *)self)->instance;

    CollisionManager *collision = GetCollisionManager();
    SDL_Rect DRect = {
        instance->position->getX(instance->position),
        instance->position->getY(instance->position),
        instance->hitBox.w,
        instance->hitBox.h,
    };
    SDL_FPoint vel = instance->vel;
    SDL_Rect SRect = {100, 200, 100, 50};
    SDL_FPoint nor;

    bool c = collision->ResolveDynamicRectVsRect(DRect, vel, SRect, dt);

    if (c)
    {
        instance->vel.x = 0;
        instance->vel.y = 0;
    }

    instance->position->translate(instance->position, instance->vel.x * dt, instance->vel.y * dt);
}
/*
void eventBullet(void *self)
{
    InputHandler *inputHandler = getInputHandler();
    int x, y;
    unsigned int buttons = inputHandler->getMouseState(&x, &y);

    BulletInstance *instance = ((Bullet *)self)->instance;

    if ((buttons & SDL_BUTTON_RMASK) != 0)
    {
        instance->vel.x = (x - instance->position->getX(instance->position)) / 120;
        instance->vel.y = (y - instance->position->getY(instance->position)) / 120;
    }
}*/
void destroyBullet(void *self)
{
    free(self);
    free(((Bullet *)self)->instance);
}

Bullet *newBullet(SDL_FPoint vel)
{
    Bullet *self = malloc(sizeof(Bullet));
    self->instance = malloc(sizeof(BulletInstance));

    self->instance->position = newTransform();

    self->instance->vel.x = vel.x;
    self->instance->vel.y = vel.y;

    self->instance->hitBox.x = 0;
    self->instance->hitBox.y = 0;
    self->instance->hitBox.w = 25;
    self->instance->hitBox.h = 25;

    self->render = renderBullet;
    self->update = updateBullet;
    //self->events = eventBullet;
    self->destroy = destroyBullet;

    return self;
}