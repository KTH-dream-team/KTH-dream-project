#include "EntityManager.h"
#include "ArrayList.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Warrior.h"
#include "Cube.h"
#include "Bullet.h"
#include "TextButton.h"

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
    EntityManager *EM = (EntityManager *)self;
    if (EM->getByID(EM, id) != NULL)
    {
        printf("Allready added: %s\n", id);
        return;
    }

    EntityManagerInstance *instance = EM->instance;

    Entity *temp = malloc(sizeof(Entity));
    temp->id = id;
    temp->entity = entity;

    instance->entityList->add(instance->entityList, temp);

    // printf("Added: %s\n", id);
}

void *getEntityByID(void *self, char *id)
{
    ArrayList *entitylist = ((EntityManager *)self)->instance->entityList;
    int lenght = entitylist->getLength(entitylist);

    for (int i = 0; i < lenght; i++)
    {
        Entity *t = (Entity *)(entitylist->indexOf(entitylist, i));
        if (strcmp(t->id, id) == 0)
        {
            return t->entity;
        }
    }

    return NULL;
}
void dropEntity(void *self, char *id)
{
    ArrayList *entitylist = ((EntityManager *)self)->instance->entityList;
    int lenght = entitylist->getLength(entitylist);

    for (int i = 0; i < lenght; i++)
    {
        Entity *t = (Entity *)(entitylist->indexOf(entitylist, i));
        if (strcmp(t->id, id) == 0)
        {
            if (strstr(t->id, "Bullet-") != NULL)
            {
                ((Bullet *)t->entity)->destroy((Bullet *)t->entity);
                entitylist->drop(entitylist, i);
            }
            break;
        }
    }
}

void renderAllEntities(void *self)
{
    ArrayList *entitylist = ((EntityManager *)self)->instance->entityList;
    int lenght = entitylist->getLength(entitylist);

    for (int i = 0; i < lenght; i++)
    {
        Entity *t = (Entity *)(entitylist->indexOf(entitylist, i));
        if (strstr(t->id, "Warrior-") != NULL)
        {
            ((Warrior *)t->entity)->render((Warrior *)t->entity);
        }
        else if (strstr(t->id, "Cube-") != NULL)
        {
            ((Cube *)t->entity)->render((Cube *)t->entity);
        }
        else if (strstr(t->id, "Bullet-") != NULL)
        {
            ((Bullet *)t->entity)->render((Bullet *)t->entity);
        }
        else if (strstr(t->id, "TextButton-") != NULL)
        {
            ((TextButton *)t->entity)->render((TextButton *)t->entity);
        }
    }
}
void updateAllEntities(void *self, float dt)
{
    ArrayList *entitylist = ((EntityManager *)self)->instance->entityList;
    int lenght = entitylist->getLength(entitylist);

    for (int i = 0; i < lenght; i++)
    {
        Entity *t = (Entity *)(entitylist->indexOf(entitylist, i));
        if (strstr(t->id, "Warrior-") != NULL)
        {
            ((Warrior *)t->entity)->update((Warrior *)t->entity, dt);
        }
        else if (strstr(t->id, "Cube-") != NULL)
        {
            ((Cube *)t->entity)->update((Cube *)t->entity, dt);
        }
        else if (strstr(t->id, "Bullet-") != NULL)
        {
            ((Bullet *)t->entity)->update((Bullet *)t->entity, dt);
        }
        else if (strstr(t->id, "TextButton-") != NULL)
        {
            ((TextButton *)t->entity)->update((TextButton *)t->entity, dt);
        }
    }
}
void handleAllEvents(void *self)
{
    ArrayList *entitylist = ((EntityManager *)self)->instance->entityList;
    int lenght = entitylist->getLength(entitylist);

    for (int i = 0; i < lenght; i++)
    {
        Entity *t = (Entity *)(entitylist->indexOf(entitylist, i));
        if (strstr(t->id, "Warrior-") != NULL)
        {
            ((Warrior *)t->entity)->events((Warrior *)t->entity);
        }
    }
}

void destroyEntityManager(void *self)
{
    ArrayList *entitylist = ((EntityManager *)self)->instance->entityList;
    int lenght = entitylist->getLength(entitylist);
    for (int i = 0; i < lenght; i++)
    {
        Entity *t = (Entity *)(entitylist->indexOf(entitylist, i));
        if (strstr(t->id, "Warrior-") != NULL)
        {
            ((Warrior *)t->entity)->destroy((Warrior *)t->entity);
        }
        else if (strstr(t->id, "Cube-") != NULL)
        {
            ((Cube *)t->entity)->destroy((Cube *)t->entity);
            printf("update Cube: %s\n", t->id);
        }
        else if (strstr(t->id, "Bullet-") != NULL)
        {
            ((Bullet *)t->entity)->destroy((Bullet *)t->entity);
        }
        else if (strstr(t->id, "TextButton-") != NULL)
        {
            ((TextButton *)t->entity)->destroy((TextButton *)t->entity);
        }
    }

    EntityManagerInstance *instance = ((EntityManager *)self)->instance;
    instance->entityList->destroy(instance->entityList);
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
    self.drop = dropEntity;
    self.destroy = destroyEntityManager;

    self.updateAll = updateAllEntities;
    self.renderAll = renderAllEntities;
    self.handleAllEvents = handleAllEvents;

    self.instance = malloc(sizeof(EntityManagerInstance));
    self.instance->entityList = malloc(sizeof(ArrayList *));
    self.instance->entityList = newArrayList();

    return &self;
}