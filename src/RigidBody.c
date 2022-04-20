#include "Rigidbody.h"
#include <stdlib.h>
#include <stdio.h>

struct rigidbodyInstance
{
    float mass;
    float gravity;

    SDL_FPoint friction;
    SDL_FPoint force;
    SDL_FPoint pos;
    SDL_FPoint acc;
    SDL_FPoint vel;
};

void setMass(void *self, float mass)
{
    ((Rigidbody *)self)->instance->mass = mass;
}
float getMass(void *self)
{
    return ((Rigidbody *)self)->instance->mass;
}
void setFriction(void *self, float fx, float fy)
{
    RigidbodyInstance *instance = ((Rigidbody *)self)->instance;
    instance->friction.x = fx;
    instance->friction.y = fy;
}
void setFrictionX(void *self, float fx)
{
    ((Rigidbody *)self)->instance->friction.x = fx;
}
void setFrictionY(void *self, float fy)
{
    ((Rigidbody *)self)->instance->friction.y = fy;
}
SDL_FPoint getFriction(void *self)
{
    return ((Rigidbody *)self)->instance->friction;
}
void setForce(void *self, float fx, float fy)
{
    RigidbodyInstance *instance = ((Rigidbody *)self)->instance;
    instance->force.x = fx;
    instance->force.y = fy;
}
void setForceX(void *self, float fx)
{
    ((Rigidbody *)self)->instance->force.x = fx;
}
void setForceY(void *self, float fy)
{
    ((Rigidbody *)self)->instance->force.y = fy;
}
void applyForce(void *self, float fx, float fy)
{
    RigidbodyInstance *instance = ((Rigidbody *)self)->instance;
    instance->force.x = instance->force.x + fx;
    instance->force.y = instance->force.y + fy;
}
SDL_FPoint getForce(void *self)
{
    return ((Rigidbody *)self)->instance->force;
}
void setVelocity(void *self, float fx, float fy)
{
    RigidbodyInstance *instance = ((Rigidbody *)self)->instance;
    instance->vel.x = fx;
    instance->vel.y = fy;
}
void setVelocityX(void *self, float fx)
{
    ((Rigidbody *)self)->instance->force.x = fx;
}
void setVelocityY(void *self, float fy)
{
    ((Rigidbody *)self)->instance->force.y = fy;
}
SDL_FPoint getVelocity(void *self)
{
    return ((Rigidbody *)self)->instance->force;
}
SDL_FPoint getPosition(void *self)
{
    return ((Rigidbody *)self)->instance->pos;
}
SDL_FPoint getAcceleration(void *self)
{
    return ((Rigidbody *)self)->instance->acc;
}

void updateRigidBody(void *self, float dt)
{
    RigidbodyInstance *instance = ((Rigidbody *)self)->instance;

    dt = dt / 60;

    float frictionX = instance->friction.x;
    float frictionY = instance->friction.y;
    float forceX = instance->force.x;
    float forceY = instance->force.y;
    //  acc
    float accX = (forceX + frictionX) / instance->mass;
    float accY = ((instance->gravity / instance->mass) + frictionY + forceY) / instance->mass;
    instance->acc.x = accX;
    instance->acc.y = accY;
    //  vel
    float velX = instance->vel.x + accX * dt;
    float velY = instance->vel.y + accY * dt;
    instance->vel.x = velX;
    instance->vel.y = velY;
    //  pos
    float posX = velX * dt;
    float posY = velY * dt;
    instance->pos.x = posX;
    instance->pos.y = posY;

    printf("val: %f\n", dt);
}
void destroyRigidBody(void *self)
{
    RigidbodyInstance *instance = ((Rigidbody *)self)->instance;

    free(instance);
    free(self);
    printf("RigidBody destroyed\n");
}

Rigidbody *newRigidBody()
{
    Rigidbody *self = malloc(sizeof(Rigidbody));
    self->instance = malloc(sizeof(RigidbodyInstance));

    self->instance->mass = 1;
    self->instance->gravity = 50;
    self->instance->friction.x = 0;
    self->instance->friction.y = 0;
    self->instance->force.x = 0;
    self->instance->force.y = 0;
    self->instance->pos.x = 0;
    self->instance->pos.y = 0;
    self->instance->acc.x = 0;
    self->instance->acc.y = 0;
    self->instance->vel.x = 0;
    self->instance->vel.y = 0;
    self->instance->mass = 1;//todo rest to 1
    self->instance->gravity = 0;//todo set to 5 eller 9.8

    self->getMass = getMass;
    self->setMass = setMass;

    self->setFriction = setFriction;
    self->setFrictionX = setFrictionX;
    self->setFrictionY = setFrictionY;
    self->getFriction = getFriction;

    self->setForce = setForce;
    self->setForceX = setForceX;
    self->setForceY = setForceY;
    self->applyForce = applyForce;
    self->getForce = getForce;

    self->setVelocity = setVelocity;
    self->setVelocityX = setVelocityX;
    self->setVelocityY = setVelocityY;
    self->getVelocity = getVelocity;

    self->getPosition = getPosition;
    self->getAcceleration = getAcceleration;

    self->update = updateRigidBody;
    self->destroy = destroyRigidBody;

    return self;
}
