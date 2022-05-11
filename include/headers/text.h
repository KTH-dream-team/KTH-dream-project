#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

typedef struct textInstance TextInstance;
typedef struct text
{
    void (*render)(void *self);
    void (*destroy)(void *self);
	TextInstance *instance;
}Text;

Text *newText( char *text, int x, int y, int size, SDL_Color color);




