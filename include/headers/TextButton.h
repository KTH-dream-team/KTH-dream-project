#include "SDL2/SDL_ttf.h"

typedef struct buttonInstance ButtonInstance;


typedef struct textButton
{
    int (*getStateTextButton)(void *self);
    void (*updateTextButton)(void *self);
    void (*render)(void *self);
    void (*destroyTextButton)(void *self);
    ButtonInstance *instance;
}TextButton;


TextButton *newTextButton(char *text, SDL_Color textColor, SDL_Color backGroundColor, int size, SDL_Rect rect); 