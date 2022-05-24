#pragma once
#include <SDL2/SDL.h>


typedef struct cameraInstance CameraInstance;

typedef struct camera
{
    void (*update)(void *self);
    void (*set)(void *self, int x, int y);
    SDL_Point (*getCameraOffset)(void *self);
    void (*destroy)(void *self);

    CameraInstance *instance;
} Camera;

Camera *getCamera();