/*
    cube is a test object, which render a rectangle to the screen. it could move around by click on the winow. It have position, velocity and a hitbox. this object used to test colition detection.
*/

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