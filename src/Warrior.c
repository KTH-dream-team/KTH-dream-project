#include "Warrior.h"
#include "Animation.h"
#include "InputHandler.h"
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
static unsigned int currentTime;
static unsigned int lastTime;

struct warriorInstance
{
    Animation *animation;
    Transform *position;
    Rigidbody *rigidBody;
    SDL_Rect hitBox;
};

void updateWarrior(void*self, float dt)
{
    //update animation
    Animation *anim = ((Warrior *)self)->instance->animation;
    anim->update(anim);
    
    Rigidbody *rig = ((Warrior*)self)->instance->rigidBody;
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

    mapManager->checkColision(mapManager, dRect, vel, dt);//!warrior collision check


    //update position
    SDL_FPoint PTranslate = rig->getPosition(rig);
    //printf("get pos %d", (rig->getPosition(rig)).y);

    pos->translate(pos, PTranslate.x, PTranslate.y);


}

void renderWarrior(void*self)
{
    Animation *anim = ((Warrior*)self)->instance->animation;
    Transform *pos = ((Warrior*)self)->instance->position;


    anim->draw(anim,pos->getX(pos) , pos->getY(pos));//!shiftar animatinner x och y
}
void warriorEventHandle(void*self)
{

    InputHandler *inputHandler = getInputHandler();
    inputHandler->listen(inputHandler);

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
    if(inputHandler->getKeyPress(inputHandler,SDL_SCANCODE_1))
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
    if(inputHandler->getKeyPress(inputHandler,SDL_SCANCODE_A))
    {
        anim->set(anim, "warrior", 32, 32, 0, 13, 90, 0);
      

    }
    if(inputHandler->getKeyPress(inputHandler,SDL_SCANCODE_S))
    {
        anim->set(anim, "warrior", 32, 32, 7, 7, 90, 0);
        rig->setVelocityX(rig, 50);
    }
    if(inputHandler->getKeyPress(inputHandler,SDL_SCANCODE_D))
    {
        anim->set(anim, "warrior", 32, 32, 3, 10, 90, 0);
        rig->setForceX(rig,30);//!test
    }
    if(inputHandler->getKeyPress(inputHandler,SDL_SCANCODE_2))//!stop
    {
        anim->set(anim, "warrior", 32, 32, 3, 10, 90, 0);
        rig->setForceX(rig,0);
        rig->setForceY(rig,0);
    }
    if (inputHandler->getKeyPress(inputHandler, SDL_SCANCODE_SPACE))
    {
        anim->set(anim, "warrior", 32, 32, 3, 10, 90, 0);
          rig->setVelocityY(rig, -100);
        printf("trycker SPACE\n");
    }
    int mouse_x, mouse_y;
    if(inputHandler->getMouseState(&mouse_x,&mouse_y)==1){
       // printf("mouse x =%d y=%d %u \n",mouse_x, mouse_y, inputHandler->getMouseState(&mouse_x,&mouse_y));//todo remove

        //currentTime=SDL_GetTicks();
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

    self->instance->position->set(self->instance->position, 320,138);//!x y inital position 
    self->instance->rigidBody = newRigidBody();
    self->instance->rigidBody->setForce(self->instance->rigidBody, 0 , 0);//!forces på gubben initialt

    self->instance->position->set(self->instance->position, 0, 0);

    self->update = updateWarrior;
    self->eventHandler = warriorEventHandle;
    self->destroy = destroyWarrior;
    self->render = renderWarrior;
    

    return self;
}