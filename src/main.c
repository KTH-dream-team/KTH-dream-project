#include <stdio.h>
#include "DataTypes.h"
#include "FpsManager.h"
#include "GameEngin.h"
#include "InputHandler.h"
#include "map.h"
#include "hitBox.h"
#include <time.h>
#include "networkClient.h"

#define SDL_MAIN_HANDLED

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 500

int main(int argc, char **argv)
{
    srand(time(0));
    NetworkClient *network = getNetworkClient();
    if (!network->init(network))
        return 1;

    network->connect(network, 2);


    GameEngin *Engine = getGameEngin();
    bool isInitSucceed = Engine->init(Engine, "Kth_dream_team", SCREEN_WIDTH, SCREEN_HEIGHT, false);
    if (!isInitSucceed)
        return 1;

    FpsManager *fpsManager = getFpsManager();
    InputHandler *inputHandler = getInputHandler();

    while (Engine->isRunning(Engine))
    {
        fpsManager->listen(fpsManager);
        fpsManager->frameRateListen(fpsManager);
        inputHandler->listen(inputHandler);
        network->listen(network);

        Engine->handleEvents(Engine);
        Engine->handleUpdates(Engine);
        Engine->handleRenders(Engine);
    }
    Engine->destroyEngine(Engine);

    return 0;
}
