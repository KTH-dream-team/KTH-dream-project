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
#include "TextureManager.h"
#include "map.h"
void destroyBullet(void *self);

struct bulletInstance
{
    Transform *position;
    SDL_FPoint vel;
    SDL_Rect hitBox;
    char *id;
    bool isLocal;
};

void renderBullet(void *self)
{
    BulletInstance *instance = ((Bullet *)self)->instance;
    GameEngin *engin = getGameEngin();
    TextureManager *textureManager = getTextureManager();
    textureManager->load(textureManager, "bullet", "./assets/bullet.png"); //! if not null ta befintlig
    SDL_Rect srcRec = {0, 0, 6, 6};                                      //! skapa 32x32 source rectangel

    SDL_Rect box = {
        instance->position->getX(instance->position) + instance->hitBox.x,
        instance->position->getY(instance->position) + instance->hitBox.y,
        instance->hitBox.w,
        instance->hitBox.h,
    };
    textureManager->draw(textureManager, "bullet", srcRec, box, 1);
    SDL_RenderDrawRect(engin->getRenderer(engin), &box);//todo remove
}
void updateBullet(void *self, float dt)
{
    BulletInstance *instance = ((Bullet *)self)->instance;
    instance->position->translate(instance->position, instance->vel.x * dt, instance->vel.y * dt);

    // check collition with map tiles
    MapManager *mapManager = getMapManager();//MAP
    SDL_Rect hitBox = ((Bullet*)self)->instance->hitBox;
    SDL_Rect bulletDRect ={
        instance->position->getX(instance->position) + instance->hitBox.x,
        instance->position->getY(instance->position) + instance->hitBox.y,
        instance->hitBox.w,
        instance->hitBox.h,
    };
    EntityManager *EM = getEntityManager();
    if (mapManager->checkColision(mapManager, bulletDRect, &instance->vel, dt,2))
    {
        EM->drop(EM,instance->id);
        return;
    }
    
    //check collision with warrior 
    Warrior *warrior000 = EM->getByID(EM,"Warrior-000");
    if(warrior000->checkColisionWarriorVsBullet(warrior000,bulletDRect,&instance->vel,dt)){

        EM->drop(EM,instance->id);
        return;
    }
    Warrior *warrior001 = EM->getByID(EM,"Warrior-001");
    if(warrior001->checkColisionWarriorVsBullet(warrior001,bulletDRect,&instance->vel,dt)){

        EM->drop(EM,instance->id);
        return;
    }

    Warrior *warrior002 = EM->getByID(EM,"Warrior-002");
    if(warrior002->checkColisionWarriorVsBullet(warrior002,bulletDRect,&instance->vel,dt)){

        EM->drop(EM,instance->id);
        return;
    }
    Warrior *warrior003 = EM->getByID(EM,"Warrior-003");
    if(warrior003->checkColisionWarriorVsBullet(warrior003,bulletDRect,&instance->vel,dt)){

        EM->drop(EM,instance->id);
        return;
    }


    


    // self destroy;
    Transform *pos = instance->position;
    if (pos->getX(pos) > 1100 || pos->getY(pos) > 1100 || pos->getY(pos) < 0 || pos->getX(pos) < 0)
    {
        EM->drop(EM, instance->id);
        return;
    }

    
}
void destroyBullet(void *self)
{
    free(((Bullet *)self)->instance->id);
    free(((Bullet *)self)->instance);
    free(self);
}

char *getBulletID(void *self)
{
    return ((Bullet *)self)->instance->id;
}

Bullet *newBullet(SDL_FPoint pos, SDL_FPoint vel, bool isLocal)
{
    Bullet *self = malloc(sizeof(Bullet));
    self->instance = malloc(sizeof(BulletInstance));
    self->instance->id = malloc(sizeof(char) * 11);

    self->instance->position = newTransform();                             //! positon
    self->instance->position->set(self->instance->position, pos.x, pos.y); //! position

    // randomize bullet id;
    strcpy(self->instance->id, "Bullet-000");
    for (int i = 7; i < 10; i++)
    {
        int r = rand() % 10;
        self->instance->id[i] += r;
    }
    self->instance->id[11] = '\0';

    self->instance->vel.x = vel.x;
    self->instance->vel.y = vel.y;
    self->instance->isLocal = isLocal;

    self->instance->hitBox.x = 10;
    self->instance->hitBox.y = 15;
    self->instance->hitBox.w = 6;
    self->instance->hitBox.h = 6;

    self->render = renderBullet;
    self->update = updateBullet;
    self->destroy = destroyBullet;
    self->getID = getBulletID;

    return self;
}