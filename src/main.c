#include <stdio.h>
#include "DataTypes.h"
#include "FpsManager.h"
#include "GameEngin.h"
#include "InputHandler.h"
#include "map.h"
#include "hitBox.h"
#include "networkClient.h"
#include "audio.h"
#include "menu.h"
#include <stdlib.h>

#define SDL_MAIN_HANDLED

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 500

int main(int argc, char **argv)
{
    // system("clear");
    NetworkClient *network = getNetworkClient();
    network->init(network);
    printf("Main_1\n");
    GameEngin *Engine = getGameEngin();
    bool isInitSucceed = Engine->init(Engine, "Kth_dream_team", SCREEN_WIDTH, SCREEN_HEIGHT, false);
    if (!isInitSucceed)
        return 1;
    printf("Main_2\n");

    TTF *ttf = getMenu();
    printf("getMenu Successfull\n");
    ttf->ttfInit(ttf);
    printf("Init Successfull\n");
    ttf->destroy(ttf);
    printf("destroy Successfull\n");

    FpsManager *fpsManager = getFpsManager();
    InputHandler *inputHandler = getInputHandler();
    printf("Main_3\n");

    while (Engine->isRunning(Engine))
    {
        fpsManager->listen(fpsManager);
        fpsManager->frameRateListen(fpsManager);
        inputHandler->listen(inputHandler);

        network->listen(network);//! problem here
        // printf("fps %d\n", fpsManager->getFrameRate(fpsManager));
        Engine->handleEvents(Engine);
        Engine->handleUpdates(Engine);
        Engine->handleRenders(Engine);
    }
    Engine->destroyEngine(Engine);

    
    return 0;
}
