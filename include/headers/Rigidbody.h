#pragma once
#include "Vector.h"


typedef struct rigidbodyInstance RigidbodyInstance;


typedef struct rigidbody
{
    void (*setMass)(void*self, float mass);
    float (*getMass)(void*self);

    void (*setFriction)(void*self, float fx, float fy);
    void (*setFrictionX)(void*self, float fx);
    void (*setFrictionY)(void*self, float fx);

    void (*setForce)(void*self, float x, float y);
    void (*setForceX)(void*self, float x);
    void (*setForceY)(void*self, float y);

    Vector *(*getPosition)(void*self);
    float (*getPositionX)(void*self);
    float (*getPositionY)(void*self);
    Vector *(*getAcceleration)(void*self);
    Vector *(*getVelocity)(void*self);

    void (*update)(void*self, float dt);
    void (*destroy)(void *self);

    RigidbodyInstance *instance;
} Rigidbody;


Rigidbody * newRigidBody();