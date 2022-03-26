#pragma onece

typedef struct entityManagerInstance EntityManagerInstance;

typedef struct entityManager
{
    void (*add)(void *self, char *id, void *entity);
    void *(*getByID)(void *self, char *id);
    void (*destroy)(void *self);

    EntityManagerInstance *instance;
} EntityManager;

EntityManager *getEntityManager();