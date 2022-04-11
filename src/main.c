#include <stdio.h>
#include "DataTypes.h"
#include "FpsManager.h"
#include "GameEngin.h"
#include "map.h"
#include "hitBox.h"


#define SDL_MAIN_HANDLED

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 500

int main(int argc, char **argv)
{

    GameEngin *Engine = getGameEngin();
    Engine->init(Engine, "Kth_dream_team", SCREEN_WIDTH, SCREEN_HEIGHT, false);

    FpsManager *fpsManager = getFpsManager();
    //!init network
    while (Engine->isRunning(Engine))
    {
        fpsManager->listen(fpsManager);
        Engine->handleEvents(Engine);
        Engine->handleUpdates(Engine);
        Engine->handleRenders(Engine);
    }
    Engine->destroyEngine(Engine);
    // MapManager *mapManager = getMapManager();
    // mapManager->show(mapManager);

    return 0;
}
