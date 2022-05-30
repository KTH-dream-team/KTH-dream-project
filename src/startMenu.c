#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include "SDL2/SDL_image.h"

#include "GameEngin.h"
#include "startMenu.h"
#include "TextButton.h"
#include "text.h"
#include "EntityManager.h"
#include "TextureManager.h"
#include "InputHandler.h"
#include "networkClient.h"

struct startmenuinstance
{
    bool isRunning;
    TextButton *connect;
    TextButton *createServer;
    // Text *IP;
    Text *input;
    char *text;
};

static bool changeWord = false;
char *substr(const char *src, int m, int n);
char *getMyIPAdress();

void userInput(void *self)
{
    StartMenuInstance *instance = ((StartMenu *)self)->instance;

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_TEXTINPUT)
        {
            strcat(instance->text, event.text.text);
            changeWord = true;
        }
        else if (event.key.keysym.sym == SDLK_BACKSPACE && strlen(instance->text) > 4)
        {
            instance->text[strlen(instance->text) - 1] = '\0';
            changeWord = true;
        }
        else if (strlen(instance->text) > 18)
        {
            return;
        }
        else if (event.type == SDL_QUIT)
        {
            GameEngin *GE = getGameEngin();
            GE->quit(GE);
        }
        else if(event.key.keysym.sym == SDLK_RETURN2){
             printf("1\n");
        NetworkClient *network = getNetworkClient();
        if (!network->init(network))
            printf("Couldn't initialize network!\n");
        instance->isRunning = false;
        network->connect(network, 2);
        printf("2\n");
        instance->isRunning = false;
        }
        
    }
}

void renderStartMenu(void *self)
{

    StartMenuInstance *instance = ((StartMenu *)self)->instance;
    GameEngin *Engine = getGameEngin();
    SDL_Renderer *renderer = Engine->getRenderer(Engine);

    TextureManager *TM = getTextureManager();
    TM->draw(TM, "startMenyBg", NULL, NULL, SDL_FLIP_NONE);

    instance->connect->render(instance->connect);
    instance->createServer->render(instance->createServer);
    // instance->IP->render(instance->IP);
    if (changeWord)
    {
        free(instance->input);
        SDL_Color txtColor = {255, 255, 255, 255};
        instance->input = newText(instance->text, 100, 300, 24, txtColor);
        changeWord = false;
    }
    instance->input->render(instance->input);

    SDL_RenderPresent(renderer);
}

char *getEnteredIP(void *self)
{
    StartMenuInstance *instance = ((StartMenu *)self)->instance;
    int length = strlen(instance->text);
    char *IP = substr(instance->text, 4, length);
    return IP;
}

void handleMouseInput(void *self)
{
    // get user input on buttons
    StartMenuInstance *instance = ((StartMenu *)self)->instance;
    instance->connect->update(instance->connect);
    instance->createServer->update(instance->createServer);
    int connectBtnState = instance->connect->getStateTextButton(instance->connect);
    int serverBtnState = instance->createServer->getStateTextButton(instance->createServer);
    if (connectBtnState == 2)
    {
        printf("1\n");
        NetworkClient *network = getNetworkClient();
        if (!network->init(network))
            printf("Couldn't initialize network!\n");
        instance->isRunning = false;
        network->connect(network, 2);
        printf("2\n");
        instance->isRunning = false;

    }

    if (serverBtnState == 2)
    {
        const char *cmd = "open -a Terminal.app ./server.o";
        system(cmd);
    }
}

void destroyStartMenu(void *self)
{
    StartMenuInstance *instance = ((StartMenu *)self)->instance;
    free(instance);
    printf("Menu destroyed\n");
}

bool startMenuIsRunning(void *self)
{
    return ((StartMenu *)self)->instance->isRunning;
}

StartMenu *getStartMenu()
{
    static StartMenu self;
    if (self.instance != NULL)
        return &self;

    self.instance = malloc(sizeof(StartMenuInstance));
    self.instance->isRunning = true;

    self.render = renderStartMenu;
    self.isRunning = startMenuIsRunning;
    self.mouseInput = handleMouseInput;
    self.destroy = destroyStartMenu;
    self.input = userInput;
    self.getIP = getEnteredIP;
    // create button
    SDL_Color txtColor = {255, 255, 255, 255};
    SDL_Color bgColor = {0, 0, 0, 255};
    SDL_Rect connectRect = {100, 100, 200, 60};
    SDL_Rect ServerRect = {100, 200, 200, 60};

    self.instance->connect = newTextButton("Connect", txtColor, bgColor, 24, connectRect);




    TextButton button1 = newTextButton("Connect",
    txtColor, bgColor, 24, connectRect);
    TextButton button2 = newTextButton("Disconnect",  
    txtColor, bgColor, 24, connectRect);
    button1->render(button1);
    button1->destroy(button2);





    self.instance->createServer = newTextButton("Create Server", txtColor, bgColor, 24, ServerRect);

    self.instance->text = malloc(sizeof(char) * 20);
    strcpy(self.instance->text, "IP: 127.0.0.1");
    self.instance->input = newText(self.instance->text, 100, 300, 24, txtColor);
    TextureManager *TM = getTextureManager();
    TM->load(TM, "startMenyBg", "assets/menu.jpg");

    return &self;
}

char *substr(const char *src, int m, int n)
{
    int len = n - m;
    char *dest = (char *)malloc(sizeof(char) * (len + 1));
    for (int i = m; i < n && (*(src + i) != '\0'); i++)
    {
        *dest = *(src + i);
        dest++;
    }
    *dest = '\0';
    return dest - len;
}
