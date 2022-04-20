#include "EntityManager.h"
#include "ArrayList.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Entity
{
    char *id;
    void *entity;
} Entity;

struct entityManagerInstance
{
    ArrayList *entityList;
};

void addInstance(void *self, char *id, void *entity)
{
    EntityManagerInstance *instance = ((EntityManager *)self)->instance;

    Entity *temp = malloc(sizeof(Entity));
    temp->id = id;
    temp->entity = entity;

    instance->entityList->add(instance->entityList, temp);
}

void *getEntityByID(void *self, char *id)
{
    ArrayList *entitylist = ((EntityManager *)self)->instance->entityList;
    int lenght = entitylist->getLength(entitylist);

    for (int i = 0; i < lenght; i++)
    {
        Entity *t = (Entity *)(entitylist->indexOf(entitylist, i));
        if (strcmp(t->id, id) == 0)
            return t->entity;
    }

    return NULL;
}

void destroyEntityManager(void *self)
{
    EntityManagerInstance *instance = ((EntityManager *)self)->instance;

    instance->entityList->destroy(instance->entityList);
    // free(instance->entityList);

    free(instance);
    printf("EntityManager Destroyed\n");
}

EntityManager *getEntityManager()
{
    static EntityManager self;

    if (self.instance != NULL)
        return &self;

    self.add = addInstance;
    self.getByID = getEntityByID;
    self.destroy = destroyEntityManager;

    self.instance = malloc(sizeof(EntityManagerInstance *));//todo ta bort * ???
    self.instance->entityList = malloc(sizeof(ArrayList *));
    self.instance->entityList = newArrayList();

    return &self;
}