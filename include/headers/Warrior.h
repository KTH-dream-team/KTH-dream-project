/*
    warrior function is a test class used to test out our components
    testing animation, trasform, rigidbody
*/
#pragma once
#include <stdbool.h>

typedef struct warriorInstance WarriorInstance;

typedef struct warrior
{
    void (*update)(void *self, float dt);
    void (*render)(void *self);
    void (*events)(void *self);
    void (*destroy)(void *self);

    WarriorInstance *instance;
} Warrior;

Warrior *createWarrior(float x, float y, char * id, int networkId, bool isLocal);