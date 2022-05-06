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
    bool isLocal;
    int networkId;
    char id[20];
};

void updateWarrior(void *self, float dt)
{
    // update animation
    Animation *anim = ((Warrior *)self)->instance->animation;
    anim->update(anim);

    if (!((Warrior *)self)->instance->isLocal)
        return;

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
    mapManager->checkColision(mapManager, dRect, vel, dt, 1); //! warrior collision check

    // update position
    SDL_FPoint PTranslate = rig->getPosition(rig);
    SDL_FPoint acc = rig->getAcceleration(rig);
    pos->translate(pos, PTranslate.x, PTranslate.y);

    // broadcast data;
    static unsigned int lastTime;

    unsigned int currentTime = SDL_GetTicks();
    unsigned int deltaTime = currentTime - lastTime;
    if(deltaTime < 20)
        return;
    lastTime = currentTime;

    NetworkClient *network = getNetworkClient();
    WarriorSnapshot wa = {network->getTCPID(network), pos->getX(pos), pos->getY(pos)};
    network->UDPbroadCast(network, &wa, sizeof(WarriorSnapshot), 3);
}
void renderWarrior(void *self)
{
    Animation *anim = ((Warrior *)self)->instance->animation;
    Transform *pos = ((Warrior *)self)->instance->position;
    WarriorInstance *instance = ((Warrior *)self)->instance;

    anim->draw(anim, pos->getX(pos), pos->getY(pos), 1);

    GameEngin *engin = getGameEngin();

    SDL_Rect box = {
        instance->position->getX(instance->position) + instance->hitBox.x,
        instance->position->getY(instance->position) + instance->hitBox.y,
        instance->hitBox.w,
        instance->hitBox.h,
    };
    SDL_Renderer *ren = engin->getRenderer(engin);

    // draw hitbox debugg
    SDL_SetRenderDrawColor(ren, 200, 20, 20, 255);
    SDL_RenderDrawRect(engin->getRenderer(engin), &box);
}
void warriorEventHandle(void *self)
{
    if (!((Warrior *)self)->instance->isLocal)
        return;

    InputHandler *inputHandler = getInputHandler();
    MapManager *mapManager = getMapManager(); // MAP
    Rigidbody *rig = ((Warrior *)self)->instance->rigidBody;
    EntityManager *entityManager = getEntityManager(); //! entityManager
    Animation *anim = ((Warrior *)self)->instance->animation;
    Transform *pos = ((Warrior *)self)->instance->position;

    NetworkClient *network = getNetworkClient();
    WarriorSnapshot test = {2, 32, 134};

    rig->setVelocityX(rig, 0);
    if (inputHandler->getKeyPress(inputHandler, SDL_SCANCODE_LEFT))
    {
        rig->setVelocityX(rig, -130);
        // network->UDPbroadCast(network, &test, sizeof(WarriorSnapshot), 3);
    }
    if (inputHandler->getKeyPress(inputHandler, SDL_SCANCODE_RIGHT))
    {
        rig->setVelocityX(rig, 130);
        // network->TCPbroadCast(network, &test, sizeof(WarriorSnapshot), 3);
    }

    if (inputHandler->getKeyPress(inputHandler, SDL_SCANCODE_A))
    {
        anim->set(anim, "warrior", 32, 32, 0, 13, 90, 0);
        rig->setVelocityX(rig, -130);
    }
    if (inputHandler->getKeyPress(inputHandler, SDL_SCANCODE_S))
    {
        anim->set(anim, "warrior", 32, 32, 7, 7, 90, 0);
        rig->setVelocityX(rig, 50);
    }
    if (inputHandler->getKeyPress(inputHandler, SDL_SCANCODE_D))
    {
        anim->set(anim, "warrior", 32, 32, 3, 10, 90, 0);
        rig->setVelocityX(rig, 130);
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
    char bulletId[50] = "Bullet-";
    static int bulletCount = 0;                                              //! ongoing
    if (inputHandler->getMouseState(&mouse_x, &mouse_y) == SDL_BUTTON_RMASK) //! right mouse button pressed
    {
        static unsigned int currentTime;
        static unsigned int lastTime;
        currentTime = SDL_GetTicks(); // bullet cooldown 100ms
        if (lastTime + 100 < currentTime)
        {
            int cubeX, cubeY;
            unsigned int buttons = inputHandler->getMouseState(&cubeX, &cubeY);
            float velx = cubeX - pos->getX(pos);
            float vely = cubeY - pos->getY(pos);
            float xN = velx / sqrt(velx * velx + vely * vely);
            float yN = vely / sqrt(velx * velx + vely * vely);
            SDL_FPoint velN = {xN * 15, yN * 15}; //! bullet velocity
            Bullet *bullet1 = newBullet(pos->get(pos), velN, true);
            char *id = bullet1->getID(bullet1);
            entityManager->add(entityManager, id, bullet1);
            lastTime = currentTime;

            // broadcast bullet1
            NetworkClient *network = getNetworkClient();
            SDL_FPoint p = pos->get(pos);
            ShootBullet dataToSend = {
                network->getTCPID(network),
                p.x,
                p.y,
                velN.x,
                velN.y};
            network->TCPbroadCast(network, &dataToSend, sizeof(ShootBullet), 4);
        }
    }

    if (inputHandler->getMouseState(&mouse_x, &mouse_y) == SDL_BUTTON_LEFT)
    {

        if (inputHandler->getKeyPress(inputHandler, SDL_SCANCODE_E))
        {
            mapManager->build(mapManager, mouse_x, mouse_y, 0); //! build hold E
        }
        if (inputHandler->getKeyPress(inputHandler, SDL_SCANCODE_Q))
        {
            mapManager->dig(mapManager, mouse_x, mouse_y); //! dig hold Q
        }
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

void updateWarriorPosition(void *self, float x, float y)
{
    WarriorInstance *instance = ((Warrior *)self)->instance;
    instance->position->set(instance->position, x, y);
}

char *getWarriorID(void *self)
{
    return ((Warrior *)self)->instance->id;
}

Warrior *createWarrior(float x, float y, int id, int networkId, bool isLocal)
{

    int warriorHight = 32;
    int warriorWidth = 32;

    Warrior *self = malloc(sizeof(Warrior));
    self->instance = malloc(sizeof(WarriorInstance));

    TextureManager *texterManager = getTextureManager();
    texterManager->load(texterManager, "warrior", "./assets/WariorAnim.png");

    self->instance->hitBox.x = 3;
    self->instance->hitBox.y = 7;
    self->instance->hitBox.w = warriorWidth - 10;
    self->instance->hitBox.h = warriorHight - 7;
    self->instance->isLocal = isLocal;
    self->instance->networkId = networkId;
    strcpy(self->instance->id, "Warrior-000");

    int a = 100;
    for (int i = 8; i < 11; i++)
    {
        self->instance->id[i] += (int)(id / a);
        id %= a;
        a /= 10;
    }

    self->instance->animation = newAnimation();
    self->instance->animation->set(self->instance->animation, "warrior", warriorWidth, warriorHight, 0, 13, 90, SDL_FLIP_NONE);

    self->instance->position = newTransform();
    self->instance->position->set(self->instance->position, x, y);

    self->instance->rigidBody = newRigidBody();
    self->instance->rigidBody->setForce(self->instance->rigidBody, 0, 0); //! forces på gubben initialt

    self->update = updateWarrior;
    self->events = warriorEventHandle;
    self->destroy = destroyWarrior;
    self->updatePossition = updateWarriorPosition;
    self->render = renderWarrior;
    self->getID = getWarriorID;

    return self;
}