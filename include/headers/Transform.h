/*
    the main perpuse of this class is to simplify the process of transform a vector
    this class include x, y values and helper functions

    setter and getter function used to get and set x,y values

    translate functions used to translate x,y values. 
*/
#include "SDL2/SDL.h"

typedef struct transformInstance TransformInstance;

typedef struct transform
{
    void (*set)(void *self, float x, float y);
    void (*setX)(void *self, float x);
    void (*setY)(void *self, float y);

    SDL_FPoint (*get)(void *self);
    float (*getX)(void *self);
    float (*getY)(void *self);

    void (*translateX)(void *self, float x);
    void (*translateY)(void *self, float y);
    void (*translate)(void *self, float x, float y);
    

    SDL_FPoint (*getNormalized)(void *self);

    void (*destroy)(void *self);

    TransformInstance *instance;
} Transform;

Transform *newTransform();