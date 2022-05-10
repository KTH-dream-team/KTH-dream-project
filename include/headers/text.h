#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

typedef struct textInstance TextInstance;
typedef struct text
{
    void (*init)(void *self);
    void (*render)(void *self);
    void (*destroy)(void *self);
	TextInstance *instance;
}Text;

void initTTF();
Text *newText( char *text, int x, int y, int size, Uint8 color);




