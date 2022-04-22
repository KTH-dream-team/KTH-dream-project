/*
    EntityManager is an Array of Entities(objects). This class is used to manage all entity on screen, every object on the game (player, bullet, guns ...) exept map. Every entitiy get added goes with its own identifier(ID). There is only one EntityManger in the whole game.

    void add(void *self, char *id, void *entity);
        this fuction add a reference of an entity to the array.
        Id: unique identifier
        entity: pointer to the entity that will be added

    void *getByID(void *self, char *id)
        return reference to the entity corresponding to the given id

    void (*destroy)(void *self);
        destroy the EntityManager object
        !TODO: destroy all the entity in the array.

    !TODO: implement render, eventhandle, update for all entities, then call it on gameEngin.

*/

#pragma onece

typedef struct entityManagerInstance EntityManagerInstance;

typedef struct entityManager
{
    void (*add)(void *self, char *id, void *entity);
    void *(*getByID)(void *self, char *id);
    void (*drop)(void *self, char*id);
    void (*destroy)(void *self);
    void (*updateAll)(void *self, float dt);
    void (*renderAll)(void *self);
    void (*handleAllEvents)(void *self);


    EntityManagerInstance *instance;
} EntityManager;

EntityManager *getEntityManager();