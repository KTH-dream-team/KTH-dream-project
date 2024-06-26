#include "Transform.h"
#include <stdlib.h>
#include <math.h>


struct transformInstance
{
    float X;
    float Y;
};

void transformSet(void *self, float x, float y)
{
    TransformInstance* instance = ((Transform*)self)->instance;
    instance->X = x;
    instance->Y = y;
}
void transformSetX(void *self, float x)
{
    TransformInstance* instance = ((Transform*)self)->instance;
    instance->X = x;
}
void transformSetY(void *self, float y)
{
    TransformInstance* instance = ((Transform*)self)->instance;
    instance->Y = y;
}
SDL_FPoint transformGet(void *self)
{
    TransformInstance* instance = ((Transform*)self)->instance;
    SDL_FPoint r = {
        instance->X,
        instance->Y
    };
    return r;
}
float transformGetX(void *self){
    return ((Transform*)self)->instance->X;
}
float transformGetY(void *self){
    return ((Transform*)self)->instance->Y;
}
void transformTranslateX(void *self, float x)
{
    TransformInstance* instance = ((Transform*)self)->instance;
    instance->X += x;
}
void transformTranslateY(void *self, float y)
{
    TransformInstance* instance = ((Transform*)self)->instance;
    instance->Y += y;
}
void transformTranslate(void *self, float x, float y)
{
    TransformInstance* instance = ((Transform*)self)->instance;
    instance->X += x;
    instance->Y += y;
}
void destroyTransform(void *self)
{
    free(((Transform*)self)->instance);
    free((Transform*)self);
}

SDL_FPoint getNormalized(void *self)
{
    TransformInstance* instance = ((Transform*)self)->instance;
    float x = instance->X;
    float y = instance->Y;
    float xN =  x / sqrt(x*x+y*y);
    float yN =  y / sqrt(x*x+y*y);
    
    SDL_FPoint ret = {xN, yN};
    
    return ret;
}

Transform* newTransform ()
{
    Transform * self = malloc(sizeof(Transform));
    self->instance = malloc(sizeof(TransformInstance));

    self->instance->X = 0;
    self->instance->Y = 0;

    self->set = transformSet;
    self->setX = transformSetX;
    self->setY = transformSetY;

    self->get = transformGet;
    self->getX = transformGetX;
    self->getY = transformGetY;

    self->translate = transformTranslate;
    self->translateX = transformTranslateX;
    self->translateY = transformTranslateY;

    self->destroy = destroyTransform;

    return self;
}