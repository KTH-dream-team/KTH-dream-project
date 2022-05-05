#include "OtherWarrior.h"
#include "Animation.h"
#include "InputHandler.h"
#include "TextureManager.h"
#include "Transform.h"
#include "Rigidbody.h"
#include <stdio.h>
#include <map.h>
#include "Cube.h"
#include "EntityManager.h"
#include "Bullet.h"
#include "math.h"
#include "string.h"
#include "CollisionManager.h"
#include "networkClient.h"
#include "data.h"

static unsigned int currentTime;
static unsigned int lastTime;
#define accMan 0.5
#define speedMan 2

struct otherWarriorInstance
{
    Animation *animation;
    Transform *position;
    WarriorPos *datapos;
    SDL_Rect hitBox;
    int clientID;
};

void updateOtherWarrior(void *self, float dt)
{
    //update animation
    Animation *anim = ((OtherWarrior *)self)->instance->animation;
    anim->update(anim);

}
void renderOtherWarrior(void *self)
{
    Animation *anim = ((OtherWarrior *)self)->instance->animation;
    Transform *pos = ((OtherWarrior *)self)->instance->position;
    OtherWarriorInstance * instance = ((OtherWarrior *)self)->instance;
    NetworkClient *network = getNetworkClient();

    // printf("Before TCPresive\n");
    network->TCPresive(network,instance->datapos, 12); // pos warrior
    // printf("After TCPresive\n");
    
    anim->draw(anim, pos->getX(pos), pos->getY(pos), 1);
    GameEngin *engin = getGameEngin();

    SDL_Rect box = {
        instance->datapos->x + instance->hitBox.x,
        instance->datapos->y + instance->hitBox.y,
        instance->hitBox.w,
        instance->hitBox.h,
    };

    SDL_Renderer *ren =  engin->getRenderer(engin);
    SDL_SetRenderDrawColor(ren, 200,20,20,255);
    SDL_RenderDrawRect(engin->getRenderer(engin), &box);

}
void otherWarriorEventHandle(void *self)
{
    InputHandler *inputHandler = getInputHandler();
    MapManager *mapManager = getMapManager();//MAP
    EntityManager* entityManager=getEntityManager();//!entityManager
    Animation *anim = ((OtherWarrior *)self)->instance->animation;
    Transform *pos = ((OtherWarrior *)self)->instance->position;

}
void destroyOtherWarrior(void *self)
{
    OtherWarrior *warrior = ((OtherWarrior *)self);

    warrior->instance->animation->destroy(warrior->instance->animation);
    warrior->instance->position->destroy(warrior->instance->position);

    free(warrior->instance);
    free(warrior);

    printf("OtherWarrior destroyed\n");
}

OtherWarrior *createOtherWarrior(int clientID, int x, int y)
{
    
    int warriorHight=32;
    int warriorWidth=32;

    OtherWarrior *self = malloc(sizeof(OtherWarrior));
    self->instance = malloc(sizeof(OtherWarriorInstance));
    self->instance->datapos = malloc(sizeof(WarriorPos));
   
    TextureManager *texterManager = getTextureManager();
    texterManager->load(texterManager, "warrior", "./assets/WariorAnim.png");

    self->instance->hitBox.x = 3;
    self->instance->hitBox.y = 7;
    self->instance->hitBox.w = warriorWidth-10;
    self->instance->hitBox.h = warriorHight-7;

    self->instance->clientID=0;

    self->instance->animation = newAnimation();
    self->instance->animation->set(self->instance->animation, "warrior", warriorWidth, warriorHight, 0, 13, 90, SDL_FLIP_NONE);

    self->instance->position = newTransform();
    self->instance->position->set(self->instance->position, x*20,y*20);
    

    self->update = updateOtherWarrior;
    self->events = otherWarriorEventHandle;
    self->destroy = destroyOtherWarrior;
    self->render = renderOtherWarrior;

    return self;
}