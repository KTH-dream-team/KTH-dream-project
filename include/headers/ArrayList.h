/*
    arrayList handle dynamic and multiple PointerTypes. It Can store multiple types of pointer att onece.

    void add(void *self, void *toAdd)
        add a void pointer to the array;

    void *indexOf(void *self, int index)
        return a pointer at a given index in the array;

    int getLength(void *self)
        return lengh of array;

    void destroy(void *self)
        clean the array;
        !TODO: destroy all element that got added to the array by call its coresponding destroy function.


    BLOCK is the amout of extra memories get added to the array when the it dynamickly get bigger;
*/

#pragma once
#define BLOCK 3

struct inst;
typedef struct inst Inst;

typedef struct arrayList
{
    void (*add)(void *self, void *toAdd);
    void (*drop)(void *self, int index);
    void *(*indexOf)(void *self, int index);
    void (*destroy)(void *self);
    int (*getLength)(void *self);

    Inst *list;
} ArrayList;

ArrayList *newArrayList(void);