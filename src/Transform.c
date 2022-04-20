#include "Transform.h"
#include <stdlib.h>


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

Transform* newTransform ()
{
    Transform * self = malloc(sizeof(Transform));
    self->instance = malloc(sizeof(TransformInstance));

    self->instance->X = 0;
    self->instance->Y = 0;

    self->set = transformSet;
    self->setX = transformSetX;
    self->setY = transformSetY;

    self->getX = transformGetX;
    self->getY = transformGetY;

    self->translate = transformTranslate;
    self->translateX = transformTranslateX;
    self->translateY = transformTranslateY;

    return self;
}