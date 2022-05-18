#include <SDL2/SDL.h>
#include <stdbool.h>
#include "GameEngin.h"
#include "startMenu.h"
#include "TextButton.h"
#include "text.h"
#include "EntityManager.h"
#include "TextureManager.h"
#include "SDL2/SDL_image.h"
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <ifaddrs.h>
#include <string.h>
#include <InputHandler.h>

#include "networkClient.h"
struct startmenuinstance
{
    bool isRunning;
    SDL_Surface *surface;
    TextButton *connect;
    TextButton *createServer;
    Text *IP;
    Text *input;
    char text[16];
};
void userInput(void *self)
{
    StartMenuInstance *instance = ((StartMenu *)self)->instance;
    SDL_StartTextInput();
    while (strlen(instance->text) < 16)
    {
        SDL_Event event;
        if (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_TEXTINPUT:
                strcat(instance->text, event.text.text);
                system("clear");
                printf("%s\n", instance->text);
                break;
            default:
                break;
            }
        }
    }

    SDL_StopTextInput();
}

void renderStartMenu(void *self)
{
    StartMenuInstance *instance = ((StartMenu *)self)->instance;
    GameEngin *Engine = getGameEngin();

    SDL_Renderer *renderer = Engine->getRenderer(Engine);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, instance->surface);

    SDL_RenderCopy(renderer, texture, NULL, NULL); //! ordning spelar roll på renderingen
    instance->connect->render(instance->connect);
    instance->createServer->render(instance->createServer);
    instance->IP->render(instance->IP);

    // SDL_Color txtColor = {255, 255, 255, 255};
    // instance->input = newText(instance->text, 300, 300, 24, txtColor);
    // instance->input->render(instance->input);

    SDL_RenderPresent(renderer);
}

void updateStartMenu(void *self)
{
    // get user input on buttons
    StartMenuInstance *instance = ((StartMenu *)self)->instance;
    instance->connect->update(instance->connect);
    instance->createServer->update(instance->createServer);

    int connectBtnState = instance->connect->getStateTextButton(instance->connect);
    int serverBtnState = instance->createServer->getStateTextButton(instance->createServer);
    if (connectBtnState == 2)
    {
        NetworkClient *network = getNetworkClient();
        if (!network->init(network))
            printf("Couldn't initialize network!\n");
        network->connect(network, 2);
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
    SDL_FreeSurface(instance->surface);

    free(instance);
    printf("Menu destroyed\n");
}

bool startMenuIsRunning(void *self)
{
    return ((StartMenu *)self)->instance->isRunning;
}

char *getMyIPAdress()
{
    // hämtad från stack overflow
    // https://stackoverflow.com/questions/4139405/how-can-i-get-to-know-the-ip-address-for-interfaces-in-c
    struct ifaddrs *ifap, *ifa;
    struct sockaddr_in *sockAddr;
    char *addr;
    getifaddrs(&ifap);
    for (ifa = ifap; ifa; ifa = ifa->ifa_next)
    {
        if (ifa->ifa_addr && ifa->ifa_addr->sa_family == AF_INET)
        {
            sockAddr = (struct sockaddr_in *)ifa->ifa_addr;
            addr = inet_ntoa(sockAddr->sin_addr);
        }
    }
    freeifaddrs(ifap);
    return addr;
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
    self.update = updateStartMenu;
    self.destroy = destroyStartMenu;
    self.input = userInput;

    // create button
    SDL_Color txtColor = {255, 255, 255, 255};
    SDL_Color bgColor = {0, 0, 0, 255};
    SDL_Rect connectRect = {100, 100, 200, 60};
    SDL_Rect ServerRect = {100, 200, 200, 60};
    self.instance->connect = newTextButton("Connect", txtColor, bgColor, 24, connectRect);
    self.instance->createServer = newTextButton("Create Server", txtColor, bgColor, 24, ServerRect);
    // create IP text
    char *ip = getMyIPAdress();
    self.instance->IP = newText(ip, 760, 440, 24, txtColor);
    // load background
    self.instance->surface = IMG_Load("assets/menu.jpg");
    strcpy(self.instance->text, "");

    return &self;
}