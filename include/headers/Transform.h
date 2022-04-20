<<<<<<< HEAD
=======
/*
    the main perpuse of this class is to simplify the process of transform a vector
    this class include x, y values and helper functions

    setter and getter function used to get and set x,y values

    translate functions used to translate x,y values. 
*/
#include "SDL2/SDL.h"
>>>>>>> 24fd0418550284cc5e479b351d712bd2fe1c585d

typedef struct transformInstance TransformInstance;

typedef struct transform
{
    void (*set)(void *self, float x, float y);
    void (*setX)(void *self, float x);
    void (*setY)(void *self, float y);

    float (*getX)(void *self);
    float (*getY)(void *self);

    void (*translateX)(void *self, float x);
    void (*translateY)(void *self, float y);
    void (*translate)(void *self, float x, float y);

    void (*destroy)(void *self);

    TransformInstance *instance;
} Transform;

Transform *newTransform();