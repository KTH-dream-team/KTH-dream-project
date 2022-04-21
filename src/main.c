#include <stdio.h>
#include "DataTypes.h"
#include "FpsManager.h"
#include "GameEngin.h"
#include "InputHandler.h"

#define SDL_MAIN_HANDLED

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 500

int main(int argc, char **argv)
{

    GameEngin *Engine = getGameEngin();
    bool isInitSucceed = Engine->init(Engine, "Kth_dream_team", SCREEN_WIDTH, SCREEN_HEIGHT, false);
    if (!isInitSucceed)
    {
        printf("Failed to init the game!\n");
        return 0;
    }

    FpsManager *fpsManager = getFpsManager();
    InputHandler *inputHandler = getInputHandler();

    while (Engine->isRunning(Engine))
    {
        fpsManager->listen(fpsManager);
        // printf("fps delta %f ",fpsManager->getDelta(fpsManager));
        // printf("fps elapsed time %d \n",fpsManager->getElapsedTime(fpsManager));
        // printf(" fps = %f\n",fpsManager->getElapsedTime(fpsManager)/0.006);
        fpsManager->frameRateListen(fpsManager);
        printf("fps = %d\n",fpsManager->getFrameRate(fpsManager));
        inputHandler->listen(inputHandler);
        
        Engine->handleEvents(Engine);
        Engine->handleUpdates(Engine);
        Engine->handleRenders(Engine);
    }

    Engine->destroyEngine(Engine);
    return 0;
}
