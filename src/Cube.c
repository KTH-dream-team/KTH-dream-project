#include "Cube.h"
#include <stdlib.h>
#include "Transform.h"
#include "SDL2/SDL.h"
#include "GameEngin.h"
#include "InputHandler.h"
#include "CollisionManager.h"
#include "SDL2/SDL.h"

struct cubeInstance
{
    Transform *position;
    SDL_FPoint vel;
    SDL_Rect hitBox;
};

void renderCube(void *self)
{
    CubeInstance *instance = ((Cube *)self)->instance;
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
void updateCube(void *self, float dt)
{
    CubeInstance *instance = ((Cube *)self)->instance;

    CollisionManager *collision = GetCollisionManager();
    SDL_Rect DRect = {
        instance->position->getX(instance->position),
        instance->position->getY(instance->position),
        instance->hitBox.w,
        instance->hitBox.h,
    };
    SDL_FPoint *vel = &(instance->vel);
    SDL_Rect SRect = {100, 200, 100, 50};
    SDL_FPoint nor;

    bool c = collision->ResolveDynamicRectVsRect(DRect, vel, SRect, dt);

    instance->position->translate(instance->position, instance->vel.x * dt, instance->vel.y * dt);
}
void eventCube(void *self)
{
    InputHandler *inputHandler = getInputHandler();
    int x, y;
    unsigned int buttons = inputHandler->getMouseState(&x, &y);

    CubeInstance *instance = ((Cube *)self)->instance;

    if ((buttons & SDL_BUTTON_LMASK) != 0)
    {
        instance->vel.x = (x - instance->position->getX(instance->position)) / 120;
        instance->vel.y = (y - instance->position->getY(instance->position)) / 120;
    }
}
void destroyCube(void *self)
{
    free(self);
    free(((Cube *)self)->instance);
}

Cube *newCube()
{
    Cube *self = malloc(sizeof(Cube));
    self->instance = malloc(sizeof(CubeInstance));

    self->instance->position = newTransform();

    self->instance->vel.x = 1;
    self->instance->vel.y = 1;

    self->instance->hitBox.x = 0;
    self->instance->hitBox.y = 0;
    self->instance->hitBox.w = 50;
    self->instance->hitBox.h = 70;

    self->render = renderCube;
    self->update = updateCube;
    self->events = eventCube;
    self->destroy = destroyCube;

    return self;
}