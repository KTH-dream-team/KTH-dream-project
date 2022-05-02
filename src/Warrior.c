#include "Warrior.h"
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

    //printf("=========================\n");
    mapManager->checkColision(mapManager, dRect, vel, dt);//!warrior collision check


    //update position
    SDL_FPoint PTranslate = rig->getPosition(rig);

    SDL_FPoint acc = rig->getAcceleration(rig);

    //printf("vel: x:%f, y:%f, acc: x:%f, y: %f\n", PTranslate.x, PTranslate.y, acc.x, acc.y);

    pos->translate(pos, PTranslate.x, PTranslate.y);


}
void renderWarrior(void *self)
{
    Animation *anim = ((Warrior *)self)->instance->animation;
    Transform *pos = ((Warrior *)self)->instance->position;
    WarriorInstance * instance = ((Warrior *)self)->instance;
    NetworkClient *network = getNetworkClient();
    anim->draw(anim, pos->getX(pos), pos->getY(pos), 1);
    
    GameEngin *engin = getGameEngin();

    SDL_Rect box = {
        instance->position->getX(instance->position) + instance->hitBox.x,
        instance->position->getY(instance->position) + instance->hitBox.y,
        instance->hitBox.w,
        instance->hitBox.h,
    };
    SDL_Renderer *ren =  engin->getRenderer(engin);
    SDL_SetRenderDrawColor(ren, 200,20,20,255);
    SDL_RenderDrawRect(engin->getRenderer(engin), &box);

    SDL_FPoint warriorPos = pos->get(pos);
    DataPos wariorSendPos ={warriorPos.x, warriorPos.y, 1};
    network->UDPbroadCast(network,&wariorSendPos,sizeof(DataPos));//todo fixa size
    
}
void warriorEventHandle(void *self)
{
    InputHandler *inputHandler = getInputHandler();
    MapManager *mapManager = getMapManager();//MAP
    Rigidbody *rig = ((Warrior *)self)->instance->rigidBody;
    EntityManager* entityManager=getEntityManager();//!entityManager
    Animation *anim = ((Warrior *)self)->instance->animation;
    Transform *pos = ((Warrior *)self)->instance->position;
    // CollisionManager *collisionManager = getCollisionManager(collisionManager);//!collisionManager

    rig->setVelocityX(rig, 0);
    if (inputHandler->getKeyPress(inputHandler, SDL_SCANCODE_LEFT)){
        rig->setVelocityX(rig,-130);
    }
    if (inputHandler->getKeyPress(inputHandler, SDL_SCANCODE_RIGHT)){
        rig->setVelocityX(rig,130);
    }

    if (inputHandler->getKeyPress(inputHandler, SDL_SCANCODE_A))
    {
        anim->set(anim, "warrior", 32, 32, 0, 13, 90, 0);
        rig->setVelocityX(rig,-130);
        
    }
    if (inputHandler->getKeyPress(inputHandler, SDL_SCANCODE_S))
    {
        anim->set(anim, "warrior", 32, 32, 7, 7, 90, 0);
        rig->setVelocityX(rig, 50);
    }
    if (inputHandler->getKeyPress(inputHandler, SDL_SCANCODE_D))
    {
        anim->set(anim, "warrior", 32, 32, 3, 10, 90, 0);
        rig->setVelocityX(rig,130);

    }
    if (inputHandler->getKeyPress(inputHandler, SDL_SCANCODE_SPACE))
    {
        
        anim->set(anim, "warrior", 32, 32, 3, 10, 90, 0);
        rig->setVelocityY(rig, -100);
    }
    if (inputHandler->getKeyPress(inputHandler, SDL_SCANCODE_W))
    {
        anim->set(anim, "warrior", 32, 32, 3, 10, 90, 0);
        rig->setVelocityY(rig, -100);
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
            Bullet *bullet1 = newBullet("Bullet-1", pos->get(pos), velN);
            entityManager->add(entityManager,"Bullet-1", bullet1);
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

    self->instance->hitBox.x = 3;
    self->instance->hitBox.y = 7;
    self->instance->hitBox.w = warriorWidth-10;
    self->instance->hitBox.h = warriorHight-7;

    self->instance->animation = newAnimation();
    self->instance->animation->set(self->instance->animation, "warrior", warriorWidth, warriorHight, 0, 13, 90, SDL_FLIP_NONE);

    self->instance->position = newTransform();
    self->instance->position->set(self->instance->position, 0,10);
    
    self->instance->rigidBody = newRigidBody();
    self->instance->rigidBody->setForce(self->instance->rigidBody, 0 , 0);//!forces på gubben initialt

    self->update = updateWarrior;
    self->events = warriorEventHandle;
    self->destroy = destroyWarrior;
    self->render = renderWarrior;

    return self;
}