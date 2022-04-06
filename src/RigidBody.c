#include "Rigidbody.h"
#include <stdlib.h>
#include <stdio.h>


struct rigidbodyInstance
{
    float mass;
    float gravity;

    Vector * friction;
    Vector * force;
    Vector * pos;
    Vector * acc;
    Vector * vel;
};

void setMass(void*self, float mass)
{
    RigidbodyInstance *instance = ((Rigidbody*)self)->instance;
    instance->mass = mass;
}
float getMass(void*self)
{
    RigidbodyInstance *instance = ((Rigidbody*)self)->instance;
    return instance->mass;
}

void setFriction(void*self, float fx, float fy)
{
    RigidbodyInstance *instance = ((Rigidbody*)self)->instance;
    instance->friction->set(instance->friction, fx, fy);
}
void setFrictionX(void*self, float fx)
{
    RigidbodyInstance *instance = ((Rigidbody*)self)->instance;
    instance->friction->setX(instance->friction, fx);
}
void setFrictionY(void*self, float fy)
{
    RigidbodyInstance *instance = ((Rigidbody*)self)->instance;
    instance->friction->setY(instance->friction, fy);
}

    
void setForce(void*self, float x, float y)
{
    RigidbodyInstance *instance = ((Rigidbody*)self)->instance;
    instance->force->set(instance->force, x, y);
}
void setForceX(void*self, float x)
{
    RigidbodyInstance *instance = ((Rigidbody*)self)->instance;
    instance->force->setX(instance->force, x);
}
void setForceY(void*self, float y)
{
    RigidbodyInstance *instance = ((Rigidbody*)self)->instance;
    instance->force->setY(instance->force, y);
}

Vector *getPosition(void*self)
{
    return ((Rigidbody*)self)->instance->pos;
}
float getPositionX(void*self)
{
    Vector *pos = ((Rigidbody*)self)->instance->pos;
    return pos->getX(pos);
}
float getPositionY(void*self)
{
    Vector *pos = ((Rigidbody*)self)->instance->pos;
    return pos->getY(pos);
}
Vector *getAcceleration(void*self)
{
    return ((Rigidbody*)self)->instance->acc;
}
Vector *getVelocity(void*self)
{
    return ((Rigidbody*)self)->instance->vel;
}


void updateRigidBody(void*self, float dt)
{
    RigidbodyInstance *instance = ((Rigidbody*)self)->instance;

    dt = dt/60;//ca 60 fps

    float frictionX = instance->friction->getX(instance->friction);
    float frictionY = instance->friction->getY(instance->friction);
    float forceX = instance->force->getX(instance->force);
    float forceY = instance->force->getY(instance->force);
    //acc
    float accX = (forceX + frictionX)/instance->mass;
    float accY = ((instance->gravity/instance->mass) + frictionY + forceY)/instance->mass;
    instance->acc->set(instance->acc, accX, accY);
    //vel
    float velX = instance->vel->getX(instance->vel) + accX*dt;
    float velY = instance->vel->getY(instance->vel) + accY*dt;
    instance->vel->set(instance->vel, velX, velY);
    //pos
    float posX = velX*dt;
    float posY = velY*dt;
    instance->pos->set(instance->pos, posX, posY);

   //todo sett på igen printf("posX: %f, posY: %f\n",posX, posY);

}
void destroyRigidBody(void *self)
{
    RigidbodyInstance *instance = ((Rigidbody*)self)->instance;

    instance->friction->destroy(instance->friction);
    free(instance->friction);

    instance->pos->destroy(instance->pos);
    free(instance->pos);

    instance->force->destroy(instance->force);
    free(instance->force);

    instance->acc->destroy(instance->acc);
    free(instance->acc);

    instance->vel->destroy(instance->vel);
    free(instance->vel);

    free(instance);

    free(self);
    printf("RigidBody destroyed\n");
}

Rigidbody * newRigidBody()
{
    Rigidbody *self = malloc(sizeof(Rigidbody));
    self->instance = malloc(sizeof(RigidbodyInstance));

    self->instance->mass = 1;//todo rest to 1
    self->instance->gravity = 5;//todo set to 5 eller 9.8

    self->instance->friction = malloc(sizeof(Vector));
    self->instance->friction = newVector();
    self->instance->pos = malloc(sizeof(Vector));
    self->instance->pos = newVector();
    self->instance->force = malloc(sizeof(Vector));
    self->instance->force = newVector();
    self->instance->acc = malloc(sizeof(Vector));
    self->instance->acc = newVector();
    self->instance->vel = malloc(sizeof(Vector));
    self->instance->vel = newVector();

    self->getMass = getMass;
    self->setMass = setMass;

    self->setFriction = setFriction;
    self->setFrictionX = setFrictionX;
    self->setFrictionY = setFrictionY;

    self->setForce = setForce;
    self->setForceX = setForceX;
    self->setForceY = setForceY;

    self->getAcceleration = getAcceleration;
    self->getPosition = getPosition;
    self->getPositionX = getPositionX;
    self->getPositionY = getPositionY;
    self->getVelocity = getVelocity;

    self->update = updateRigidBody;
    self->destroy = destroyRigidBody;

    return self;
}


