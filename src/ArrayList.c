#include "ArrayList.h"
#include <stdlib.h>
#include <stdio.h>

struct inst
{
    void **arr;
    int lenght;
    int total;
};

void add(void *self, void *toAdd)
{
    Inst *list = ((ArrayList *)self)->list;
    list->arr[list->lenght++] = toAdd;

    if (list->lenght < list->total)
        return;

    void **temp = malloc(sizeof(void *) * (list->total + BLOCK));
    list->total = list->total + BLOCK;
    for (int i = 0; i < list->lenght; i++)
        temp[i] = list->arr[i];

    free(list->arr);
    list->arr = temp;
}

void *indexOf(void *self, int index)
{
    return ((ArrayList *)self)->list->arr[index];
}

int getLength(void *self)
{
    return ((ArrayList *)self)->list->lenght;
}

void clean(void *self)
{
    for (int i = 0; i < ((ArrayList *)self)->list->lenght; i++)
        free(((ArrayList *)self)->indexOf(((ArrayList *)self), i));

    free(((ArrayList *)self)->list->arr);
    free(((ArrayList *)self)->list);
    free((ArrayList *)self);
    printf("ArrayList Destroyed\n");
}

ArrayList *newArrayList(void)
{
    ArrayList *self = malloc(sizeof(ArrayList));

    self->list = malloc(sizeof(Inst *));
    self->list->arr = malloc(sizeof(void *) * BLOCK);
    self->list->lenght = 0;
    self->list->total = BLOCK;

    self->add = add;
    self->indexOf = indexOf;
    self->getLength = getLength;
    self->destroy = clean;

    return self;
}