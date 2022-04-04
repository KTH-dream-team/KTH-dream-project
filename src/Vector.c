#include "Vector.h"
#include <stdlib.h>
#include <stdio.h>

struct vectorInstance
{
    float X;
    float Y;
};

void vectorSet(void*self, float x, float y)
{
    ((Vector*)self)->instance->X = x;
    ((Vector*)self)->instance->Y = y;
}
void vectorSetX(void*self, float x)
{
    ((Vector*)self)->instance->X = x;
}
void vectorSetY(void*self, float y)
{
    ((Vector*)self)->instance->Y = y;
}
float vectorGetY(void*self)
{
    return ((Vector*)self)->instance->Y;
}
float vectorGetX(void*self)
{
    return ((Vector*)self)->instance->X;
}
void destroyVector(void*self)
{
    free(((Vector*)self)->instance);
    free(self);
    printf("Vector Destroyed\n");
}

Vector *newVector()
{
    Vector *self = malloc(sizeof(Vector));
    self->instance = malloc(sizeof(VectorInstance));

    self->instance->X = 0;
    self->instance->Y = 0;

    self->set = vectorSet;
    self->setX = vectorSetX;
    self->setY = vectorSetY;
    self->getX = vectorGetX;
    self->getY = vectorGetY;

    self->destroy = destroyVector;

    return self;
}