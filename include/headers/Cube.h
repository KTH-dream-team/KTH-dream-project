#pragma once

typedef struct cubeInstance CubeInstance;

typedef struct cube
{
    void (*render)(void *self);
    void (*update)(void *self, float dt);
    void (*events)(void *self);
    void (*destroy)(void *self);

    CubeInstance *instance;
} Cube;

Cube *newCube();