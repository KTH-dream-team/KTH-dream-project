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
};

void updateWarrior(void *self, float dt)
{
    Animation *anim = ((Warrior *)self)->instance->animation;
    anim->update(anim);

    Rigidbody *rig = ((Warrior *)self)->instance->rigidBody;
    rig->update(rig, dt);

    Transform *pos = ((Warrior *)self)->instance->position;

    SDL_FPoint PTranslate = rig->getPosition(rig);
    pos->translate(pos, PTranslate.x, PTranslate.y);
}
void renderWarrior(void *self)
{
    Animation *anim = ((Warrior *)self)->instance->animation;
    Transform *pos = ((Warrior *)self)->instance->position;

    anim->draw(anim, pos->getX(pos), pos->getY(pos), 3);
}
void warriorEventHandle(void *self)
{
    InputHandler *inputHandler = getInputHandler();
    MapManager *mapManager = getMapManager();//MAP

    Animation *anim = ((Warrior *)self)->instance->animation;

    if (inputHandler->getKeyPress(inputHandler, SDL_SCANCODE_A))
    {
        anim->set(anim, "warrior", 32, 32, 0, 13, 90, 0);
    }
    if (inputHandler->getKeyPress(inputHandler, SDL_SCANCODE_S))
    {
        anim->set(anim, "warrior", 32, 32, 7, 7, 90, 0);
    }
    if (inputHandler->getKeyPress(inputHandler, SDL_SCANCODE_D))
    {
        anim->set(anim, "warrior", 32, 32, 3, 10, 90, 0);
    }
    int mouse_x, mouse_y;
    if(inputHandler->getMouseState(&mouse_x,&mouse_y)==1){
        printf("mouse x =%d y=%d %u \n",mouse_x, mouse_y, inputHandler->getMouseState(&mouse_x,&mouse_y));//todo remove

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
    Warrior *self = malloc(sizeof(Warrior));
    self->instance = malloc(sizeof(WarriorInstance));

    TextureManager *texterManager = getTextureManager();
    texterManager->load(texterManager, "warrior", "./assets/WariorAnim.png");

    self->instance->animation = newAnimation();
    self->instance->animation->set(self->instance->animation, "warrior", 32, 32, 0, 13, 90, SDL_FLIP_NONE);

    self->instance->position = newTransform();
    //todo orginal self->instance->position->set(self->instance->position, 0, 0);
    self->instance->position->set(self->instance->position, 350,6*25 );
    self->instance->rigidBody = newRigidBody();
    self->instance->rigidBody->setForce(self->instance->rigidBody, 1 , -5);//!forces på gubben initialt

    self->update = updateWarrior;
    self->eventHandler = warriorEventHandle;
    self->destroy = destroyWarrior;
    self->render = renderWarrior;

    return self;
}