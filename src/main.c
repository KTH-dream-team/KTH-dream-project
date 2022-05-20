#include <stdio.h>
#include "FpsManager.h"
#include "GameEngin.h"
#include "InputHandler.h"
#include "map.h"
#include "hitBox.h"
#include <time.h>
#include "networkClient.h"
#include "startMenu.h"
#include "EntityManager.h"
#include "PlayerManager.h"

#define SDL_MAIN_HANDLED

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 500

int main(int argc, char **argv)
{
    srand(time(0));
    NetworkClient *network = getNetworkClient();

    GameEngin *Engine = getGameEngin();
    bool isInitSucceed = Engine->init(Engine, "Kth_dream_team", SCREEN_WIDTH, SCREEN_HEIGHT, false);
    if (!isInitSucceed)
        return 1;

    InputHandler *inputHandler = getInputHandler();

    StartMenu * startMenu = getStartMenu();
    char *test = startMenu->getIP(startMenu);
            printf("\nTest IP: %s\n", test);

    FpsManager *fpsManager = getFpsManager();

    bool GameInstanceIsInit = false;

    PlayerManager *PM = getPlayerManager();
    while (Engine->isRunning(Engine))
    {
        if (startMenu->isRunning(startMenu))
        {

            inputHandler->listen(inputHandler);
            startMenu->render(startMenu);
            startMenu->mouseInput(startMenu);
            startMenu->input(startMenu);
        }
        else
        {
            if (!GameInstanceIsInit)
            {
                Engine->innitGameInstances(Engine);
                GameInstanceIsInit = true;
            }

            fpsManager->listen(fpsManager);
            fpsManager->frameRateListen(fpsManager);
            inputHandler->listen(inputHandler);
            network->listen(network);

            Engine->handleEvents(Engine);
            Engine->handleUpdates(Engine);
            Engine->handleRenders(Engine);
        }
    }
    Engine->destroyEngine(Engine);

    return 0;
}
