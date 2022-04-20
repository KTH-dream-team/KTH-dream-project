#include "Warrior.h"
#include "Animation.h"
#include "EventHandler.h"
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

void updateWarrior(void*self, float dt)
{
    Animation *anim = ((Warrior*)self)->instance->animation;
    anim->update(anim);
    
    Rigidbody *rig = ((Warrior*)self)->instance->rigidBody;
    rig->update(rig, dt);

    Transform *pos = ((Warrior*)self)->instance->position;

    pos->translate(pos,rig->getPositionX(rig), rig->getPositionY(rig));
}
void renderWarrior(void*self)
{
    Animation *anim = ((Warrior*)self)->instance->animation;
    Transform *pos = ((Warrior*)self)->instance->position;

    anim->draw(anim,pos->getX(pos) , pos->getY(pos));//!shiftar animatinner x och y
}
void warriorEventHandle(void*self)
{
    EventHandler *eventHandler = getEventHandler();
    eventHandler->listen(eventHandler);

    Animation *anim = ((Warrior*)self)->instance->animation;

    if(eventHandler->getKeyPress(eventHandler,SDL_SCANCODE_A))
    {
        anim->set(anim, "warrior", 32, 32, 0, 13, 90, 0);
    }
    if(eventHandler->getKeyPress(eventHandler,SDL_SCANCODE_S))
    {
        anim->set(anim, "warrior", 32, 32, 7, 7, 90, 0);
        
        // staticDig();//!gräva



    }
    if(eventHandler->getKeyPress(eventHandler,SDL_SCANCODE_D))
    {
        anim->set(anim, "warrior", 32, 32, 3, 10, 90, 0);
    }
    int mouse_x, mouse_y;
    if(eventHandler->getMouseState(&mouse_x,&mouse_y)==1){
        printf("x =%d y=%d %u ",mouse_x, mouse_y, eventHandler->getMouseState(&mouse_x,&mouse_y));//todo remove

        currentTime=SDL_GetTicks();
     //   if (lastTime+100<currentTime)
       // {
            printf("hej\n");
            //lastTime=currentTime;
            if (eventHandler->getKeyPress(eventHandler,SDL_SCANCODE_D))
            {
                /* code */
                build(mouse_x,mouse_y,0);
            }
            if (eventHandler->getKeyPress(eventHandler,SDL_SCANCODE_A))
            {
                /* code */
                dig(mouse_x,mouse_y);
            }
            
            //dig(mouse_x,mouse_y);

      //  }

    }
}
void destroyWarrior(void *self)
{
    Warrior *warrior = ((Warrior*)self);

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
    
    self->instance->animation = newAnimation();
    self->instance->animation->set(self->instance->animation, "warrior", 32, 32, 0, 13, 90, 0);
    
    self->instance->position = newTransform();
    //todo orginal self->instance->position->set(self->instance->position, 0, 0);
    self->instance->position->set(self->instance->position, 350,6*25 );
    self->instance->rigidBody = newRigidBody();
    self->instance->rigidBody->setForce(self->instance->rigidBody, 1 , -5);//!forces på gubben initialt

    self->update = updateWarrior;
    self->eventHandler = warriorEventHandle;
    self->destroy = destroyWarrior;
    self->render = renderWarrior;
    
    TextureManager *texterManager = getTextureManager();
    texterManager->load(texterManager, "warrior", "./assets/WariorAnim.png");

    return self;
}