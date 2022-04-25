#include "Warrior.h"
#include "Animation.h"
#include "InputHandler.h"
#include "TextureManager.h"
#include "Transform.h"
#include "Rigidbody.h"
#include <stdio.h>
#include <map.h>
#include "cube.h"
#include "EntityManager.h"
#include "Bullet.h"
#include "math.h"
#include "string.h"


static unsigned int currentTime;
static unsigned int lastTime;
#define accMan 0.5
#define speedMan 2

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

    mapManager->checkColision(mapManager, dRect, vel, dt);//!warrior collision check


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
    EntityManager* entityManager=getEntityManager();//!entityManager
    Animation *anim = ((Warrior *)self)->instance->animation;
    Transform *pos = ((Warrior *)self)->instance->position;

    float x, y;
    x = pos->getX(pos);
    if (inputHandler->getKeyPress(inputHandler, SDL_SCANCODE_LEFT)){
        printf("left go\n");
        pos->setX(pos, (pos->getX(pos)-speedMan));
    }
    if (inputHandler->getKeyPress(inputHandler, SDL_SCANCODE_RIGHT)){
        printf("right go\n");
        pos->setX(pos, (pos->getX(pos)+speedMan));
    }


    if (inputHandler->getKeyPress(inputHandler, SDL_SCANCODE_A))
    {
        anim->set(anim, "warrior", 32, 32, 0, 13, 90, 0);
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
    if (inputHandler->getKeyPress(inputHandler, SDL_SCANCODE_SPACE))
    {
        anim->set(anim, "warrior", 32, 32, 3, 10, 90, 0);
          rig->setVelocityY(rig, -100);
        printf("trycker SPACE\n");
    }
    int mouse_x, mouse_y;
    char result[50];
    char bulletId[50]="Bullet-";
    static int bulletCount = 0;//!ongoing
    if (inputHandler->getMouseState(&mouse_x,&mouse_y)==SDL_BUTTON_RMASK)
    {
        static unsigned int currentTime;
        static unsigned int lastTime;
        currentTime=SDL_GetTicks();//bullet cooldown 100ms
        if (lastTime+100<currentTime){  
            int cubeX, cubeY;
            unsigned int buttons = inputHandler->getMouseState(&cubeX, &cubeY);
            float velx = cubeX - pos->getX(pos);
            float vely = cubeY - pos->getY(pos);
            float xN =  velx / sqrt(velx*velx+vely*vely);
            float yN =  vely / sqrt(velx*velx+vely*vely);
            SDL_FPoint velN = {xN*5, yN*5};

             sprintf(result, "%d", bulletCount);
             strcat(bulletId,result);
            // Bullet *bullet1 = newBullet(bulletId, pos->get(pos), velN);
            // char *id = malloc(sizeof(char)*10);
            // id = "Bullet-a";
            static int count = 0;
            Bullet *bullet1 = newBullet(bulletId, pos->get(pos), velN);
            // Bullet *bullet2 = newBullet(bulletId, pos->get(pos), velN);
            // Bullet *bullet3 = newBullet(bulletId, pos->get(pos), velN);
           
            entityManager->add(entityManager,"Bullet-1", bullet1);
            // entityManager->add(entityManager,"Bullet-2", bullet2);
            // entityManager->add(entityManager,"Bullet-3", bullet3);
            // entityManager->add(entityManager,"Bullet-3", bullet3);
             bulletCount++;
            // id[7]+=count;
            // count ++;
             //printf("%s\n",bulletId);
            lastTime = currentTime;
        }
    }
    
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
    self->instance->rigidBody->setForce(self->instance->rigidBody, 0 , 0);//!forces på gubben initialt

    self->update = updateWarrior;
    self->events = warriorEventHandle;
    self->destroy = destroyWarrior;
    self->render = renderWarrior;

    return self;
}