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
#include "define.h"

#define SDL_MAIN_HANDLED

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 500

int main(int argc, char **argv)
{
    NetworkClient *network = getNetworkClient();
      if (!network->init(network))
          return 1;

    GameEngin *Engine = getGameEngin();
    bool isInitSucceed = Engine->init(Engine, "Kth_dream_team", SCREEN_WIDTH, SCREEN_HEIGHT, false);
    if (!isInitSucceed) return 1;
    InputHandler *inputHandler = getInputHandler();
    // StartMenu *startMenu = getStartMenu();
    FpsManager *fpsManager = getFpsManager();
    bool GameInstanceIsInit = false;
    PlayerManager *PM = getPlayerManager();
    network->connect(network,2);

    // while(startMenu->isRunning(startMenu))
    // {
    //     startMenu->render(startMenu);
    //     startMenu->input(startMenu);
    //     startMenu->mouseInput(startMenu);
    // }

    // startMenu->destroy(startMenu);
    printf("1\n");
    SDL_Delay(2000);
    printf("2\n");
    
    while(Engine->isRunning(Engine))
    {
        if(!GameInstanceIsInit){
        Engine->innitGameInstances(Engine);
        GameInstanceIsInit = true;
        }
        network->listen(network);
        inputHandler->listen(inputHandler);
        fpsManager->listen(fpsManager);
        fpsManager->frameRateListen(fpsManager);
        Engine->handleEvents(Engine);
        Engine->handleUpdates(Engine);
        Engine->handleRenders(Engine);
    }

    Engine->destroyEngine(Engine);

    return 0;
}
