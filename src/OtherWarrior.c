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
    DataPos *datapos;
    SDL_Rect hitBox;
    int clientID;
};

void updateOtherWarrior(void *self, float dt)
{
    // update animation
    Animation *anim = ((OtherWarrior *)self)->instance->animation;
    anim->update(anim);
}
void renderOtherWarrior(void *self)
{
    Animation *anim = ((OtherWarrior *)self)->instance->animation;
    Transform *pos = ((OtherWarrior *)self)->instance->position;
    OtherWarriorInstance *instance = ((OtherWarrior *)self)->instance;
    NetworkClient *network = getNetworkClient();

    // printf("Before TCPresive\n");
    network->TCPresive(network, instance->datapos, 12); // pos warrior
    // printf("After TCPresive\n");

    anim->draw(anim, pos->getX(pos), pos->getY(pos), 1);
    GameEngin *engin = getGameEngin();

    SDL_Rect box = {
        instance->datapos->x + instance->hitBox.x,
        instance->datapos->y + instance->hitBox.y,
        instance->hitBox.w,
        instance->hitBox.h,
    };

    SDL_Renderer *ren = engin->getRenderer(engin);
    SDL_SetRenderDrawColor(ren, 200, 20, 20, 255);
    SDL_RenderDrawRect(engin->getRenderer(engin), &box);
}
void otherWarriorEventHandle(void *self)
{
    InputHandler *inputHandler = getInputHandler();
    MapManager *mapManager = getMapManager();          // MAP
    EntityManager *entityManager = getEntityManager(); //! entityManager
    Animation *anim = ((OtherWarrior *)self)->instance->animation;
    Transform *pos = ((OtherWarrior *)self)->instance->position;
}

Transform *getPos(void *self)
{
    return ((OtherWarrior *)self)->instance->position;
}
int getIDWarrior(void *self)
{
    return ((OtherWarrior *)self)->instance->clientID;
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

OtherWarrior *createOtherWarrior(int clientID)
{
    // if(clientID==0){
    //     static OtherWarrior otherwarrior0;
    //     if(otherwarrior0.instance!=NULL){
    //         return &otherwarrior0;
    //     }
    // }
    // else if(clientID==1){
    //     static OtherWarrior otherwarrior1;
    //     if(otherwarrior1.instance!=NULL){
    //         return &otherwarrior1;
    //     }
    // }

    int warriorHight = 32;
    int warriorWidth = 32;

    OtherWarrior *self = malloc(sizeof(OtherWarrior));
    self->instance = malloc(sizeof(OtherWarriorInstance));
    self->instance->datapos = malloc(sizeof(DataPos));

    TextureManager *texterManager = getTextureManager();
    texterManager->load(texterManager, "warrior", "./assets/WariorAnim.png");
    self->getPos = getPos;
    self->getID = getIDWarrior;

    self->instance->hitBox.x = 3;
    self->instance->hitBox.y = 7;
    self->instance->hitBox.w = warriorWidth - 10;
    self->instance->hitBox.h = warriorHight - 7;

    self->instance->clientID = clientID;

    self->instance->animation = newAnimation();
    self->instance->animation->set(self->instance->animation, "warrior", warriorWidth, warriorHight, 0, 13, 90, SDL_FLIP_NONE);

    self->instance->position = newTransform();
    self->instance->position->set(self->instance->position, clientID * 200, 20);

    self->update = updateOtherWarrior;
    self->events = otherWarriorEventHandle;
    self->destroy = destroyOtherWarrior;
    self->render = renderOtherWarrior;

    return self;
}

OtherWarrior *createOtherWarrior0(int clientID)
{
    static OtherWarrior otherwarrior0;
    if (otherwarrior0.instance != NULL)
        return &otherwarrior0;
    otherwarrior0.instance = malloc(sizeof(OtherWarriorInstance));
    otherwarrior0.instance->datapos = malloc(sizeof(DataPos));
    TextureManager *texterManager = getTextureManager();
    texterManager->load(texterManager, "warrior", "./assets/WariorAnim.png");
    otherwarrior0.getPos = getPos;
    otherwarrior0.getID = getIDWarrior;
    otherwarrior0.instance->hitBox.x = 3;
    otherwarrior0.instance->hitBox.y = 7;
    otherwarrior0.instance->hitBox.w = 32 - 10; // 32 warrior width and height
    otherwarrior0.instance->hitBox.h = 32 - 7;
    otherwarrior0.instance->clientID = clientID;
    otherwarrior0.instance->animation = newAnimation();
    otherwarrior0.instance->animation->set(otherwarrior0.instance->animation, "warrior", 32, 32, 0, 13, 90, SDL_FLIP_NONE);
    otherwarrior0.instance->position = newTransform();
    otherwarrior0.instance->position->set(otherwarrior0.instance->position, clientID * 200, 20);
    otherwarrior0.update = updateOtherWarrior;
    otherwarrior0.events = otherWarriorEventHandle;
    otherwarrior0.destroy = destroyOtherWarrior;
    otherwarrior0.render = renderOtherWarrior;

    return &otherwarrior0;
}
OtherWarrior *createOtherWarrior1(int clientID)
{
    static OtherWarrior otherwarrior1;
    if (otherwarrior1.instance != NULL)
        return &otherwarrior1;
    otherwarrior1.instance = malloc(sizeof(OtherWarriorInstance));
    otherwarrior1.instance->datapos = malloc(sizeof(DataPos));
    TextureManager *texterManager = getTextureManager();
    texterManager->load(texterManager, "warrior", "./assets/WariorAnim.png");
    otherwarrior1.getPos = getPos;
    otherwarrior1.getID = getIDWarrior;
    otherwarrior1.instance->hitBox.x = 3;
    otherwarrior1.instance->hitBox.y = 7;
    otherwarrior1.instance->hitBox.w = 32 - 10; // 32 warrior width and height
    otherwarrior1.instance->hitBox.h = 32 - 7;
    otherwarrior1.instance->clientID = clientID;
    otherwarrior1.instance->animation = newAnimation();
    otherwarrior1.instance->animation->set(otherwarrior1.instance->animation, "warrior", 32, 32, 0, 13, 90, SDL_FLIP_NONE);
    otherwarrior1.instance->position = newTransform();
    otherwarrior1.instance->position->set(otherwarrior1.instance->position, clientID * 200, 20);
    otherwarrior1.update = updateOtherWarrior;
    otherwarrior1.events = otherWarriorEventHandle;
    otherwarrior1.destroy = destroyOtherWarrior;
    otherwarrior1.render = renderOtherWarrior;

    return &otherwarrior1;
}
OtherWarrior *createOtherWarrior2(int clientID)
{
    static OtherWarrior otherwarrior2;
    if (otherwarrior2.instance != NULL)
        return &otherwarrior2;
    otherwarrior2.instance = malloc(sizeof(OtherWarriorInstance));
    otherwarrior2.instance->datapos = malloc(sizeof(DataPos));
    TextureManager *texterManager = getTextureManager();
    texterManager->load(texterManager, "warrior", "./assets/WariorAnim.png");
    otherwarrior2.getPos = getPos;
    otherwarrior2.getID = getIDWarrior;
    otherwarrior2.instance->hitBox.x = 3;
    otherwarrior2.instance->hitBox.y = 7;
    otherwarrior2.instance->hitBox.w = 32 - 10; // 32 warrior width and height
    otherwarrior2.instance->hitBox.h = 32 - 7;
    otherwarrior2.instance->clientID = clientID;
    otherwarrior2.instance->animation = newAnimation();
    otherwarrior2.instance->animation->set(otherwarrior2.instance->animation, "warrior", 32, 32, 0, 13, 90, SDL_FLIP_NONE);
    otherwarrior2.instance->position = newTransform();
    otherwarrior2.instance->position->set(otherwarrior2.instance->position, clientID * 200, 20);
    otherwarrior2.update = updateOtherWarrior;
    otherwarrior2.events = otherWarriorEventHandle;
    otherwarrior2.destroy = destroyOtherWarrior;
    otherwarrior2.render = renderOtherWarrior;

    return &otherwarrior2;
}
OtherWarrior *createOtherWarrior3(int clientID)
{
    static OtherWarrior otherwarrior3;
    if (otherwarrior3.instance != NULL)
        return &otherwarrior3;
    otherwarrior3.instance = malloc(sizeof(OtherWarriorInstance));
    otherwarrior3.instance->datapos = malloc(sizeof(DataPos));
    TextureManager *texterManager = getTextureManager();
    texterManager->load(texterManager, "warrior", "./assets/WariorAnim.png");
    otherwarrior3.getPos = getPos;
    otherwarrior3.getID = getIDWarrior;
    otherwarrior3.instance->hitBox.x = 3;
    otherwarrior3.instance->hitBox.y = 7;
    otherwarrior3.instance->hitBox.w = 32 - 10; // 32 warrior width and height
    otherwarrior3.instance->hitBox.h = 32 - 7;
    otherwarrior3.instance->clientID = clientID;
    otherwarrior3.instance->animation = newAnimation();
    otherwarrior3.instance->animation->set(otherwarrior3.instance->animation, "warrior", 32, 32, 0, 13, 90, SDL_FLIP_NONE);
    otherwarrior3.instance->position = newTransform();
    otherwarrior3.instance->position->set(otherwarrior3.instance->position, clientID * 200, 20);
    otherwarrior3.update = updateOtherWarrior;
    otherwarrior3.events = otherWarriorEventHandle;
    otherwarrior3.destroy = destroyOtherWarrior;
    otherwarrior3.render = renderOtherWarrior;

    return &otherwarrior3;
}