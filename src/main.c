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
#include "text.h"

#define SDL_MAIN_HANDLED



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
    network->connect(network,NUM_OF_CLIENTS);

    // while(startMenu->isRunning(startMenu))
    // {
    //     startMenu->render(startMenu);
    //     startMenu->input(startMenu);
    //     startMenu->mouseInput(startMenu);
    // }

    // startMenu->destroy(startMenu);
    // printf("1\n");
    // // SDL_Delay(2000);
    // printf("2\n");
    while(Engine->isRunning(Engine))
    {
        
        if(!GameInstanceIsInit){
        Engine->innitGameInstances(Engine);
        GameInstanceIsInit = true;
        }
        // if(PM->winner(PM)!=-1){ 
            SDL_Color green ={0,255,0,255};
            Text *text = newText("WINNER",100,100,32,green);
            text->setColor(text,green);
    
            text->render(text);
            SDL_RenderPresent(Engine->getRenderer(Engine));
            printf("not =-1\n");
        // }

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
