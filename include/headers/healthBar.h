#pragma once
#include <stdbool.h>

typedef struct healthBarInstance HealthBarInstance;

typedef struct healthBar
{
    void (*set)(void*self, int health, int x, int y, bool isLocal);
    void (*show)(void *self);
    void (*destroy)(void *self);

    HealthBarInstance * instance;
} HealthBar;

HealthBar * newHealthBar();