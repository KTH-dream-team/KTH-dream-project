#include "Bullet.h"
#include <stdlib.h>
#include <stdio.h>
#include "Transform.h"
#include "SDL2/SDL.h"
#include "GameEngin.h"
#include "InputHandler.h"
#include "CollisionManager.h"
#include "SDL2/SDL.h"
#include "Cube.h"
#include "EntityManager.h"
#include "Warrior.h"
#include "string.h"

void destroyBullet(void *self);

struct bulletInstance
{
    Transform *position;
    SDL_FPoint vel;
    SDL_Rect hitBox;
    char *id;
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
}
void updateBullet(void *self, float dt)
{
    BulletInstance *instance = ((Bullet *)self)->instance;

    instance->position->translate(instance->position, instance->vel.x * dt, instance->vel.y * dt);

    Transform *pos = instance->position;
    if(pos->getX(pos) > 1100 || pos->getY(pos) > 1100 || pos->getY(pos) < 0 || pos->getX(pos) < 0){
        EntityManager * EM = getEntityManager();
        printf("bullet destroyd id %st\n",instance->id);
        EM->drop(EM,instance->id);
        ((Bullet *)self)->destroy(self);
    }
}
void destroyBullet(void *self)
{
    free(self);
    free(((Bullet *)self)->instance);
}

Bullet *newBullet(char *id, SDL_FPoint pos, SDL_FPoint vel)
{
    Bullet *self = malloc(sizeof(Bullet));
    self->instance = malloc(sizeof(BulletInstance));
    self->instance->id = malloc(sizeof(char)*30);

    self->instance->position = newTransform();//!positon
    self->instance->position->set(self->instance->position, pos.x, pos.y);//!position
    //self->instance->id = id;
    strcpy(self->instance->id, id);
    printf("bullet destroyd idnew %st\n",self->instance->id);
    self->instance->vel.x = vel.x;
    self->instance->vel.y = vel.y;

    self->instance->hitBox.x = 0;
    self->instance->hitBox.y = 0;
    self->instance->hitBox.w = 5;
    self->instance->hitBox.h = 5;

    self->render = renderBullet;
    self->update = updateBullet;
    self->destroy = destroyBullet;

    return self;
}