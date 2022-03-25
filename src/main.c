#include <stdio.h>
#include "../include/headers/DataTypes.h"

#define SDL_MAIN_HANDLED

#include "../include/headers/GameEngin.h"

#define SCREEN_WIDTH 500
#define SCREEN_HEIGHT 500

int main(int argc, char **argv)
{

    GameEngin *Engine = createGameEngin();
    int a = Engine->init(&Engine, "Kth_dream", 500, 500, false);
    printf("%d", Engine->isRunning(&Engine));
    while (Engine->isRunning(&Engine))
    {
        Engine->handleEvents(&Engine);
        Engine->handleUpdates(&Engine);
        Engine->handleRenders(&Engine);
    }

    Engine->destroyEngine(&Engine);
    return 0;
}
