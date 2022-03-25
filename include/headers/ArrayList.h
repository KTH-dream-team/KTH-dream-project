#pragma once
#define BLOCK 3

struct inst;
typedef struct inst Inst;

typedef struct arrayList
{
    void (*add)(void *self, void *toAdd);
    void *(*indexOf)(void *self, int index);
    void (*destroy)(void *self);
    int (*getLength)(void *self);

    Inst *list;
} ArrayList;

ArrayList *newArrayList(void);