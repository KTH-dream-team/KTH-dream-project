#pragma once

typedef struct ttfInstance TTFInstance;

typedef struct ttf
{
    bool (*ttfInit)(void *self);
    void (*showTTFMenu)(void *self);
   // void (*createTextBox)(void *self);
    bool (*destroy)(void *self);

    TTFInstance *instance;
}TTF;

TTF *getMenu();