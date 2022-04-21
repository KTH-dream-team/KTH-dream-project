#include "SDL2/SDL.h"
typedef struct rigidbodyInstance RigidbodyInstance;

typedef struct rigidbody
{
    

    void (*update)(void *self, float dt);
    void (*destroy)(void *self);

    RigidbodyInstance * instance;
}Rigidbody;

Rigidbody* CreateRigidBody();