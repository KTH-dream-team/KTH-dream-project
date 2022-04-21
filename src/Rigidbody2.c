#include "RigidBody2.h"



struct rigidbodyInstance
{
    float mass;
    float gravity;

    SDL_FPoint netForce;
    SDL_FPoint maxSpeed;
    SDL_FPoint velocity;
};


void rigidBodyUpdate(void *self, float dt)
{

}
void rigidBodyDestroy(void *self)
{
    RigidbodyInstance * instance = ((Rigidbody*)self)->instance;
    free(instance);
    free(self);
}


Rigidbody* CreateRigidBody()
{
    Rigidbody* self = malloc(sizeof(Rigidbody));
    self->instance = malloc(sizeof(RigidbodyInstance));

    RigidbodyInstance * instance = self->instance;

    instance->mass = 1;
    instance->gravity = 90;
    
    instance->netForce.x = 0;
    instance->netForce.y = 0;

    instance->velocity.x = 0;
    instance->velocity.y = 0;

    instance->maxSpeed.x = 10;
    instance->maxSpeed.y = 20;

    self->update = rigidBodyUpdate;
    self->destroy = rigidBodyDestroy;

    return self;
}