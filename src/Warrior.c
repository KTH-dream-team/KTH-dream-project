#include "Warrior.h"
#include "Animation.h"
#include "InputHandler.h"
#include "TextureManager.h"
#include "Transform.h"
#include "Rigidbody.h"
#include <stdio.h>
#include <map.h>
static unsigned int currentTime;
static unsigned int lastTime;

struct warriorInstance
{
    Animation *animation;
    Transform *position;
    Rigidbody *rigidBody;
    SDL_Rect hitBox;
};

void updateWarrior(void *self, float dt)
{
    //update animation
    Animation *anim = ((Warrior *)self)->instance->animation;
    anim->update(anim);

    Rigidbody *rig = ((Warrior *)self)->instance->rigidBody;
    rig->update(rig, dt);

    //handle collision
    MapManager *mapManager = getMapManager();//MAP
    Transform *pos = ((Warrior *)self)->instance->position;
    SDL_Rect hitBox = ((Warrior *)self)->instance->hitBox;
    SDL_Rect dRect ={
        pos->getX(pos)+hitBox.x,
        pos->getY(pos)+hitBox.y,
        hitBox.w,
        hitBox.h,
    };
    SDL_FPoint *vel = rig->getPositionPointer(rig);

    mapManager->checkColision(mapManager, dRect, vel, dt);


    //update position
    SDL_FPoint PTranslate = rig->getPosition(rig);
    //printf("get pos %d", (rig->getPosition(rig)).y);

    pos->translate(pos, PTranslate.x, PTranslate.y);


}
void renderWarrior(void *self)
{
    Animation *anim = ((Warrior *)self)->instance->animation;
    Transform *pos = ((Warrior *)self)->instance->position;
    

    anim->draw(anim, pos->getX(pos), pos->getY(pos), 1);
}
void warriorEventHandle(void *self)
{
    InputHandler *inputHandler = getInputHandler();
    MapManager *mapManager = getMapManager();//MAP
    Rigidbody *rig = ((Warrior *)self)->instance->rigidBody;

    Animation *anim = ((Warrior *)self)->instance->animation;

    if (inputHandler->getKeyPress(inputHandler, SDL_SCANCODE_A))
    {
        anim->set(anim, "warrior", 32, 32, 0, 13, 90, 0);
        rig->setVelocityY(rig, -100);
        printf("trycker AAAAA\n");

    }
    if (inputHandler->getKeyPress(inputHandler, SDL_SCANCODE_S))
    {
        anim->set(anim, "warrior", 32, 32, 7, 7, 90, 0);
        rig->setVelocityX(rig, 50);
    }
    if (inputHandler->getKeyPress(inputHandler, SDL_SCANCODE_D))
    {
        anim->set(anim, "warrior", 32, 32, 3, 10, 90, 0);
    }
    int mouse_x, mouse_y;
    if(inputHandler->getMouseState(&mouse_x,&mouse_y)==1){
       // printf("mouse x =%d y=%d %u \n",mouse_x, mouse_y, inputHandler->getMouseState(&mouse_x,&mouse_y));//todo remove

        currentTime=SDL_GetTicks();
     //   if (lastTime+100<currentTime)
       // {
            //lastTime=currentTime;
            if (inputHandler->getKeyPress(inputHandler,SDL_SCANCODE_E))
            {
                /* code */
                mapManager->build(mapManager,mouse_x,mouse_y,0);//!build hold E
            }
            if (inputHandler->getKeyPress(inputHandler,SDL_SCANCODE_Q))
            {
                /* code */
                mapManager->dig(mapManager,mouse_x,mouse_y);//!dig hold Q
            }
            
            //dig(mouse_x,mouse_y);

      //  }

    }
}
void destroyWarrior(void *self)
{
    Warrior *warrior = ((Warrior *)self);

    warrior->instance->animation->destroy(warrior->instance->animation);
    warrior->instance->position->destroy(warrior->instance->position);
    warrior->instance->rigidBody->destroy(warrior->instance->rigidBody);

    free(warrior->instance);
    free(warrior);

    printf("Warrior destroyed\n");
}

Warrior *createWarrior()
{
    int warriorHight=32;
    int warriorWidth=32;

    Warrior *self = malloc(sizeof(Warrior));
    self->instance = malloc(sizeof(WarriorInstance));

    TextureManager *texterManager = getTextureManager();
    texterManager->load(texterManager, "warrior", "./assets/WariorAnim.png");

    self->instance->hitBox.x = 0;
    self->instance->hitBox.y = 0;
    self->instance->hitBox.w = warriorWidth;
    self->instance->hitBox.y = warriorHight;

    self->instance->animation = newAnimation();
    self->instance->animation->set(self->instance->animation, "warrior", warriorWidth, warriorHight, 0, 13, 90, SDL_FLIP_NONE);

    self->instance->position = newTransform();
    //todo orginal self->instance->position->set(self->instance->position, 0, 0);
    self->instance->position->set(self->instance->position, 0,10);
    self->instance->rigidBody = newRigidBody();
    self->instance->rigidBody->setForce(self->instance->rigidBody, 20 , 0);//!forces på gubben initialt

    self->update = updateWarrior;
    self->eventHandler = warriorEventHandle;
    self->destroy = destroyWarrior;
    self->render = renderWarrior;

    return self;
}