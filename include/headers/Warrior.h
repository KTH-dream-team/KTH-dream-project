/*
    warrior function is a test class used to test out our components
    testing animation, trasform, rigidbody
*/
typedef struct warriorInstance WarriorInstance;

typedef struct warrior
{
    void (*update)(void *self, float dt);
    void (*render)(void *self);
    void (*events)(void *self);
    void (*destroy)(void *self);

    WarriorInstance *instance;
} Warrior;

Warrior *createWarrior();