#include "Warrior.h"
#include "Animation.h"
#include "EventHandler.h"
#include "TextureManager.h"
#include "Transform.h"
#include "Rigidbody.h"
#include <stdio.h>
#include <map.h>
#include <stdbool.h>
static unsigned int currentTime;
static unsigned int lastTime;
static unsigned int tickNow;
static unsigned int tickLast;

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

<<<<<<< HEAD
<<<<<<< HEAD
    anim->draw(anim,pos->getX(pos) , pos->getY(pos));//!shiftar animatinner x och y
=======
    anim->draw(anim, pos->getX(pos), pos->getY(pos));
>>>>>>> parent of b975077 (added doc)
=======
    anim->draw(anim, pos->getX(pos), pos->getY(pos));
>>>>>>> parent of b975077 (added doc)
}
void warriorEventHandle(void*self)
{
    EventHandler *eventHandler = getEventHandler();
    eventHandler->listen(eventHandler);

    Animation *anim = ((Warrior*)self)->instance->animation;

    MapManager *mapManager = getMapManager();//!map

    Rigidbody *rig = ((Warrior*)self)->instance->rigidBody;//!rigidbody

    Transform *position =((Warrior*)self)->instance->position;

    printf("position %f \n",position->getY(position));


    if (position->getY(position)<0)
    {
        printf("y=0\n");
        rig->setForceY(rig,30);
    }
    
    if (position->getY(position)>=500)
    {
        printf("y = 500\n");
        rig->setForceY(rig,-30);
    }
    

    static bool gravity;    
    if(eventHandler->getKeyPress(eventHandler,SDL_SCANCODE_1))
    {
        tickNow=SDL_GetTicks();
        if (tickNow>tickLast+300)
        {    
            gravity =! gravity;
            if (gravity==true)
            {
                rig->setForceY(rig,30);
            }
            if (gravity==false){
                rig->setForceY(rig,-30);
            }
            tickLast=tickNow;   
        printf("Gravity: %d\n",gravity);
        }
    }
    if(eventHandler->getKeyPress(eventHandler,SDL_SCANCODE_A))
    {
        anim->set(anim, "warrior", 32, 32, 0, 13, 90, 0);
        rig->setForceX(rig,-30);//!test

    }
    if(eventHandler->getKeyPress(eventHandler,SDL_SCANCODE_S))
    {
        anim->set(anim, "warrior", 32, 32, 7, 7, 90, 0);
    }
    if(eventHandler->getKeyPress(eventHandler,SDL_SCANCODE_D))
    {
        anim->set(anim, "warrior", 32, 32, 3, 10, 90, 0);
        rig->setForceX(rig,30);//!test
    }
    if(eventHandler->getKeyPress(eventHandler,SDL_SCANCODE_2))//!stop
    {
        anim->set(anim, "warrior", 32, 32, 3, 10, 90, 0);
        rig->setForceX(rig,0);
        rig->setForceY(rig,0);
    }
    int mouse_x, mouse_y;
    if(eventHandler->getMouseState(&mouse_x,&mouse_y)==1){//!if mose event
        printf("mouse x =%d y=%d %u \n",mouse_x, mouse_y, eventHandler->getMouseState(&mouse_x,&mouse_y));//todo remove

        //currentTime=SDL_GetTicks();
     //   if (lastTime+100<currentTime)
       // {
            //lastTime=currentTime;
            if (eventHandler->getKeyPress(eventHandler,SDL_SCANCODE_E))
            {
                /* code */
                mapManager->build(mapManager,mouse_x,mouse_y,0);//!build hold E
            }
            if (eventHandler->getKeyPress(eventHandler,SDL_SCANCODE_Q))
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
    self->instance->position->set(self->instance->position, 320,138);//!x y inital position 
    self->instance->rigidBody = newRigidBody();
    self->instance->rigidBody->setForce(self->instance->rigidBody, 0 , 0);//!forces på gubben initialt

    self->update = updateWarrior;
    self->eventHandler = warriorEventHandle;
    self->destroy = destroyWarrior;
    self->render = renderWarrior;
    
    TextureManager *texterManager = getTextureManager();
    texterManager->load(texterManager, "warrior", "./assets/WariorAnim.png");

    return self;
}