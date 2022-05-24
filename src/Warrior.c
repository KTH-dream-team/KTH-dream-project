#include "Warrior.h"
#include "Animation.h"
#include "InputHandler.h"
#include "TextureManager.h"
#include "Transform.h"
#include "Rigidbody.h"
#include <stdio.h>
#include <stdbool.h>
#include <map.h>
#include "Cube.h"
#include "EntityManager.h"
#include "Bullet.h"
#include "math.h"
#include "string.h"
#include "CollisionManager.h"
#include "networkClient.h"
#include "data.h"
#include "SDL2/SDL.h"
#include "audio.h"
#include "PlayerManager.h"

static unsigned int currentTime;
static unsigned int lastTime;
#define accMan 0.5
#define speedMan 2
#define PI 3.141

struct warriorInstance
{
    Animation *animation;
    Transform *position;
    Rigidbody *rigidBody;
    SDL_Rect hitBox;
    bool isLocal;
    int networkId;
    char id[20];
    int health;
    int bulletCooldown;
    bool isAlive;
    bool canJump;
};

void updateWarrior(void *self, float dt)
{
    // update animation
    Animation *anim = ((Warrior *)self)->instance->animation;
    anim->update(anim);

    if (!((Warrior *)self)->instance->isLocal)
        return;

    Warrior *warrior = ((Warrior *)self);
    WarriorInstance *instance = ((Warrior *)self)->instance;

    // update rigidBody
    Rigidbody *rig = ((Warrior *)self)->instance->rigidBody;
    rig->update(rig, dt);

    // handle collision
    MapManager *mapManager = getMapManager(); // MAP
    Transform *pos = ((Warrior *)self)->instance->position;
    SDL_Rect hitBox = ((Warrior *)self)->instance->hitBox;
    SDL_Rect dRect = {
        pos->getX(pos) + hitBox.x,
        pos->getY(pos) + hitBox.y,
        hitBox.w,
        hitBox.h,
    };
    SDL_FPoint *vel = rig->getPositionPointer(rig);
    int blockTypePtr;
    int blockType = mapManager->checkColision(mapManager, dRect, vel, dt, 1,&blockTypePtr); //! warrior collision check
    if (blockType == 6)
    {
        printf("blockType=%d\n", blockType);
    }
    
    switch (blockTypePtr) {
        case 6: warrior->setBulletCooldown(warrior,100); 
        printf("you picked up mashineGun\n"); break;
        case 9: warrior->addHealth(warrior,10);
        printf("added 10 health\n");
        break;
    }
    if (blockType!=0)
    {
        instance->canJump = true;
    }else{
        instance->canJump = false;
    }    
    // update position
    SDL_FPoint PTranslate = rig->getPosition(rig);
    SDL_FPoint acc = rig->getAcceleration(rig);
    pos->translate(pos, PTranslate.x, PTranslate.y);

}
void renderWarrior(void *self)
{
    Animation *anim = ((Warrior *)self)->instance->animation;
    Transform *pos = ((Warrior *)self)->instance->position;
    anim->draw(anim, pos->getX(pos), pos->getY(pos), 0.9);

    WarriorInstance *instance = ((Warrior *)self)->instance;
    if(!instance->isLocal)
        return;
    TextureManager *instanceTexture = getTextureManager();
    InputHandler *input = getInputHandler();
    int mouseX, mouseY;
    float angle, radian;
    input->getMouseState(&mouseX, &mouseY);

    //Ak setings
    if(instance->bulletCooldown == 100){ 
        SDL_Rect box = {
        instance->position->getX(instance->position) - 2,
        instance->position->getY(instance->position) + 5,
        instance->hitBox.w + 13,
        instance->hitBox.h + 13,
    };

    radian = atan2(mouseY - box.y, mouseX - box.x);
    box.x = box.x + (12 * cos(radian));
    box.y = box.y + (15 * sin(radian));
    angle = radian * (180 / PI);
    SDL_Rect srcRect = {0, 0, 150, 150}; // SDL_Rect srcRect = {0, 0, 70, 30};

    if (angle < 90 && angle > -90)
        instanceTexture->drawWithAngle(instanceTexture, "ak", srcRect, box, SDL_FLIP_NONE, angle);
    else
        instanceTexture->drawWithAngle(instanceTexture, "ak", srcRect, box, SDL_FLIP_VERTICAL, angle);

    }
    //Pistol setings
    if(instance->bulletCooldown == 400){
        SDL_Rect box = {
            instance->position->getX(instance->position) + 5,
            instance->position->getY(instance->position) + 10,
            instance->hitBox.w,
            instance->hitBox.h,
        };

        radian = atan2(mouseY - box.y, mouseX - box.x);
        box.x = box.x + (14 * cos(radian));
        box.y = box.y + (15 * sin(radian));
        angle = radian * (180 / PI);
        SDL_Rect srcRect = {0, 0, 100, 100}; // SDL_Rect srcRect = {0, 0, 70, 30};

        if (angle < 90 && angle > -90)
            instanceTexture->drawWithAngle(instanceTexture, "pistol", srcRect, box, SDL_FLIP_NONE, angle);
        else
            instanceTexture->drawWithAngle(instanceTexture, "pistol", srcRect, box, SDL_FLIP_VERTICAL, angle);
    }
    
    if (!((Warrior *)self)->instance->isLocal)
    return;
 // broadcast data;
    static unsigned int lastTime;
    
    unsigned int currentTime = SDL_GetTicks();
    unsigned int deltaTime = currentTime - lastTime;
    if(deltaTime < 20)
        return;
    lastTime = currentTime;

    NetworkClient *network = getNetworkClient();
    // printf("warrior current health %d \n ",instance->health);
    WarriorSnapshot wa = {network->getTCPID(network), pos->getX(pos), pos->getY(pos),instance->health};
    network->UDPbroadCast(network, &wa, sizeof(WarriorSnapshot), 3);
    

    // draw hitbox debugg
    // SDL_SetRenderDrawColor(ren, 200, 20, 20, 255);
    // SDL_RenderDrawRect(engin->getRenderer(engin), &box);
}
void warriorEventHandle(void *self)
{
    if (!(((Warrior *)self)->instance->isLocal && ((Warrior *)self)->instance->health>0))//!if dead stop controlls
        return;

    InputHandler *inputHandler = getInputHandler();
    MapManager *mapManager = getMapManager();
    Rigidbody *rig = ((Warrior *)self)->instance->rigidBody;
    EntityManager *entityManager = getEntityManager();
    Animation *anim = ((Warrior *)self)->instance->animation;
    Transform *pos = ((Warrior *)self)->instance->position;

    NetworkClient *network = getNetworkClient();

    WarriorInstance *warriorInstance = ((Warrior *)self)->instance;

    // warriorInstance->isAlive=false;
    Audio *audio = newAudio();

    rig->setVelocityX(rig, 0);
    if (inputHandler->getKeyPress(inputHandler, SDL_SCANCODE_LEFT))
    {
        rig->setVelocityX(rig, -130);
    }
    if (inputHandler->getKeyPress(inputHandler, SDL_SCANCODE_RIGHT))
    {
        rig->setVelocityX(rig, 130);
    }
    if (inputHandler->getKeyPress(inputHandler, SDL_SCANCODE_A))
    {

        anim->set(anim, "warrior", 32, 32, 0, 13, 90, 0, warriorInstance->isAlive);
        rig->setVelocityX(rig, -130);
    }
    if (inputHandler->getKeyPress(inputHandler, SDL_SCANCODE_S))
    {
        // anim->set(anim, "warrior", 32, 32, 7, 7, 90, 0, warriorInstance->isAlive); byt till nån annan
        rig->setVelocityX(rig, 50);
    }
    if (inputHandler->getKeyPress(inputHandler, SDL_SCANCODE_D))
    {
        // anim->set(anim, "warrior", 32, 32, 3, 10, 90, 0, warriorInstance->isAlive);
        rig->setVelocityX(rig, 130);
    }
    if (inputHandler->getKeyPress(inputHandler, SDL_SCANCODE_SPACE))
    {
        if (warriorInstance->canJump)
        {
            audio->playSound(audio, "jump");
            anim->set(anim, "warrior", 32, 32, 3, 10, 90, 0, warriorInstance->isAlive);
            rig->setVelocityY(rig, -200);
        }
        // anim->set(anim, "warrior", 32, 32, 3, 10, 90, 0, warriorInstance->isAlive);
        // rig->setVelocityY(rig, -100);
    }

    if (inputHandler->getKeyPress(inputHandler, SDL_SCANCODE_W))
    {
        if (warriorInstance->canJump)
        {
            audio->playSound(audio, "jump");
            anim->set(anim, "warrior", 32, 32, 3, 10, 90, 0, warriorInstance->isAlive);
            rig->setVelocityY(rig, -200);
        }
        
    }
    int mouse_x, mouse_y;
    char result[50];
    char bulletId[50] = "Bullet-";
    static int bulletCount = 0;                                              //! ongoing
    if (inputHandler->getMouseState(&mouse_x, &mouse_y) == SDL_BUTTON_LEFT) //! right mouse button pressed
    {

        static unsigned int currentTime;
        static unsigned int lastTime;
        currentTime = SDL_GetTicks(); // bullet cooldown 100ms
        if (lastTime + warriorInstance->bulletCooldown < currentTime)
        {
            int cubeX, cubeY;
            unsigned int buttons = inputHandler->getMouseState(&cubeX, &cubeY);

            //calculate bullet direction;
            float velx = cubeX - pos->getX(pos);
            float vely = cubeY - pos->getY(pos);
            float xN = velx / sqrt(velx * velx + vely * vely);
            float yN = vely / sqrt(velx * velx + vely * vely);
            SDL_FPoint vel = {xN * 15, yN * 15}; //! bullet velocity

            //create new bullet
            Bullet *bullet = newBullet(pos->get(pos), vel, -1, true);
            char *id = bullet->getID(bullet);
            entityManager->add(entityManager, id, bullet);
            lastTime = currentTime;
            printf("before playsound was called\n");
            audio->playSound(audio, "gun");
         
        }
    }

    if (inputHandler->getMouseState(&mouse_x, &mouse_y) == SDL_BUTTON_RMASK)
    {
           mapManager->build(mapManager, mouse_x, mouse_y, 0); //! build hold E
        
        // if (inputHandler->getKeyPress(inputHandler, SDL_SCANCODE_Q))
        // {
        //     mapManager->dig(mapManager, mouse_x, mouse_y); //! dig hold Q
        // }
    }
}

bool checkColisionWarriorVsBullet(void *self, SDL_Rect bulletDRect, SDL_FPoint *vel, float dt)
{
    CollisionManager *collisionManager = GetCollisionManager();
    WarriorInstance *warriorInstance = ((Warrior *)self)->instance;
    EntityManager *EM = getEntityManager();
    PlayerManager *PM = getPlayerManager();
    NetworkClient *network = getNetworkClient();
    Warrior *wa = ((Warrior *)self);
    Animation *anim = ((Warrior *)self)->instance->animation;
    Rigidbody *rig = ((Warrior *)self)->instance->rigidBody;
    rig->update(rig, dt);
    Audio *audio = newAudio();
    Transform *pos = ((Warrior *)self)->instance->position;
    SDL_Rect hitBox = ((Warrior *)self)->instance->hitBox;
    SDL_Rect warriorDRect = {
        pos->getX(pos) + hitBox.x,
        pos->getY(pos) + hitBox.y,
        hitBox.w,
        hitBox.h,
    };

    if (warriorInstance->health<=0)
    {
        printf("bullet vs Warrior collision detected warrior helth = %d\n",warriorInstance->health);
        return false;
    }    
    
    if(collisionManager->ResolveDynamicRectVsRect(bulletDRect,vel,warriorDRect,dt)){ 
        
        warriorInstance->health--;
        audio->playSound(audio, "hitWarrior");
        //brodcast healt decline
        NetworkClient *network = getNetworkClient();
        printf("id %d tcp id %d",warriorInstance->networkId, network->getTCPID(network));
        GotShot gotshot = {network->getTCPID(network), warriorInstance->networkId,1};
        network->TCPbroadCast(network, &gotshot, sizeof(GotShot), 8);
        
        
        printf("bullet vs Warrior collision detected warrior helth = %d\n",warriorInstance->health);
         anim->set(anim,"warrior",32,32,6,4,90,0, warriorInstance->isAlive);
        if (warriorInstance->health<=0)
        {
            printf("warrior died at %d\n",warriorInstance->health);
            audio->playSound(audio, "death");
             warriorInstance->isAlive=false;
             anim->set(anim, "warrior", 32, 32, 7, 7, 90, 0, warriorInstance->isAlive);

            //tims changes
            printf("Warrior died: ID: %d, Health: %d, Name %s, isLocal: %d\n", warriorInstance->networkId, warriorInstance->health, warriorInstance->id, warriorInstance->isLocal);
            warriorInstance->isAlive = false;
            anim->set(anim, "warrior", 32, 32, 7, 7, 90, 0, warriorInstance->isAlive);
            if (warriorInstance->isLocal == true)
            {
                printf("broadcast dead warrior\n");
                // broadcast player dead
                Alive alive;
                alive.from = network->getTCPID(network);
                alive.isAlive = false;
                alive.isLocal = warriorInstance->isLocal;

                PM->killed(PM, alive.from); // kill yourself
                PM->winner(PM);             // check if there is a winner
                network->TCPbroadCast(network, &alive, sizeof(Alive), 7);
            }
        }
        return true;
    }
    
    return false;
}

void addHealth(void *self, int health){
    Warrior *warrior = ((Warrior *)self);
    warrior->instance->health +=health;
}

int getHealth(void *self){
    Warrior *warrior = ((Warrior *)self);
    return warrior->instance->health;
}

void setHealth(void *self, int health){
    Warrior *warrior = ((Warrior *)self);
    warrior->instance->health = health;
}

void setBulletCooldown(void *self, int bulletCooldown){//shorter cooldown means faster shooting
    Warrior *warrior = ((Warrior *)self);
    warrior->instance->bulletCooldown =bulletCooldown;
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

void updateWarriorPosition(void *self, float x, float y)
{
    WarriorInstance *instance = ((Warrior *)self)->instance;
    instance->position->set(instance->position, x, y);

      //update health
    // instance->health = health;
}

char *getWarriorID(void *self)
{
    return ((Warrior *)self)->instance->id;
}

bool isLocalWarrior(void *self){
    return ((Warrior *)self)->instance->isLocal;
}

Warrior *createWarrior(float x, float y, int id, int networkId, bool isLocal)
{

    int warriorHight = 32;
    int warriorWidth = 32;
    int health=5;
    int bulletCooldown =400; //inital fier rate cooldown
    bool canJump;

    Warrior *self = malloc(sizeof(Warrior));
    self->instance = malloc(sizeof(WarriorInstance));

    TextureManager *texterManager = getTextureManager();
    texterManager->load(texterManager, "warrior", "./assets/WariorAnim.png");
    texterManager->load(texterManager,"pistol","./assets/pistol.png");
    texterManager->load(texterManager,"ak","./assets/ak.png");

    self->instance->isAlive = true;
    self->instance->hitBox.x = 3;
    self->instance->hitBox.y = 7;
    self->instance->hitBox.w = warriorWidth - 12;
    self->instance->hitBox.h = warriorHight - 7;
    self->instance->health = health;
    self->instance->canJump = canJump;
    self->instance->bulletCooldown= bulletCooldown;
    self->instance->isLocal = isLocal;
    self->instance->networkId = id;
    strcpy(self->instance->id, "Warrior-000");

    int a = 100;
    for (int i = 8; i < 11; i++)
    {
        self->instance->id[i] += (int)(id / a);
        id %= a;
        a /= 10;
    }

    self->instance->animation = newAnimation();
    self->instance->animation->set(self->instance->animation, "warrior", warriorWidth, warriorHight, 0, 13, 90, SDL_FLIP_NONE, true);

    self->instance->position = newTransform();
    self->instance->position->set(self->instance->position, x, y);

    self->instance->rigidBody = newRigidBody();
    self->instance->rigidBody->setForce(self->instance->rigidBody, 0, 0); //! forces på gubben initialt

    self->update = updateWarrior;
    self->events = warriorEventHandle;
    self->checkColisionWarriorVsBullet = checkColisionWarriorVsBullet;
    self->destroy = destroyWarrior;
    self->updatePossition = updateWarriorPosition;
    self->render = renderWarrior;
    self->getID = getWarriorID;
    self->addHealth = addHealth;
    self->getHealth = getHealth;
    self->setBulletCooldown=setBulletCooldown;
    self->setHealth=setHealth;
    self->isLocalWarrior = isLocalWarrior;

    return self;
}