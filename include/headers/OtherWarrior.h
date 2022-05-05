/*
    warrior function is a test class used to test out our components
    testing animation, trasform, rigidbody
*/
#include "Transform.h"
typedef struct otherWarriorInstance OtherWarriorInstance;

typedef struct otherwarrior
{
    void (*update)(void *self, float dt);
    void (*render)(void *self);
    void (*events)(void *self);
    int (*getID)(void *self);
    Transform *(*getPos)(void *self);

    void (*destroy)(void *self);


    OtherWarriorInstance *instance;
} OtherWarrior;

OtherWarrior *createOtherWarrior();
