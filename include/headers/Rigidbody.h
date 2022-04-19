#pragma once
#include <SDL2/SDL.h>

typedef struct rigidbodyInstance RigidbodyInstance;

typedef struct rigidbody
{
    void (*setMass)(void *self, float mass);
    float (*getMass)(void *self);

    // Friction getter/setter
    void (*setFriction)(void *self, float fx, float fy);
    void (*setFrictionX)(void *self, float fx);
    void (*setFrictionY)(void *self, float fx);
    SDL_FPoint (*getFriction)(void *self);
    // Force Getter/seter
    void (*setForce)(void *self, float fx, float fy);
    void (*setForceX)(void *self, float fx);
    void (*setForceY)(void *self, float fy);
    SDL_FPoint (*getForce)(void *self);
    // velocity getter/setter
    void (*setVelocity)(void *self, float fx, float fy);
    void (*setVelocityX)(void *self, float fx);
    void (*setVelocityY)(void *self, float fy);
    SDL_FPoint (*getVelocity)(void *self);
    // position, accileration getter
    SDL_FPoint (*getPosition)(void *self);
    SDL_FPoint (*getAcceleration)(void *self);

    void (*update)(void *self, float dt);
    void (*destroy)(void *self);

    RigidbodyInstance *instance;
} Rigidbody;

Rigidbody *newRigidBody();