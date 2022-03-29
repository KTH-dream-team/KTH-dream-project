
typedef struct warriorInstance WarriorInstance;

typedef struct warrior
{
    void(*update)(void*self, float dt);
    void(*render)(void*self);
    void(*eventHandler)(void*self);
    void(*destroy)(void *self);

    WarriorInstance *instance;
}Warrior;

Warrior *createWarrior();