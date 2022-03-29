#include <stdio.h>
#include "DataTypes.h"

#define SDL_MAIN_HANDLED

#include "GameEngin.h"

#define SCREEN_WIDTH 500
#define SCREEN_HEIGHT 500

int main(int argc, char **argv)
{

    GameEngin *Engine = getGameEngin();
    Engine->init(Engine, "Kth_dream_team", 500, 500, false);

    while (Engine->isRunning(Engine))
    {
        Engine->handleEvents(Engine);
        Engine->handleUpdates(Engine);
        Engine->handleRenders(Engine);
    }

    Engine->destroyEngine(Engine);
    return 0;
}
