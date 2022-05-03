#pragma once
#include <stdbool.h>
#include "SDL2/SDL_net.h"
#include "data.h"


typedef struct tcpserverdata
{
    float x;
    float y;
    int from;
} TCPServerData;

typedef struct client
{
    int id;
    IPaddress ip;
    TCPsocket socket;
    TCPServerData data;
} Client;

struct tcpServerInstance
{
    Client clients[MAX_CLIENTS];
    SDLNet_SocketSet socketSet;
    TCPsocket serverSocket;
    TCPsocket clientSocket;
    IPaddress serverAddress;
    int numOfClients;
    int currentID;
    int nrOfRdy;
    bool isRunning;
};

typedef struct tcpServerInstance TCPServerInstance;

typedef struct {

    bool (*init)(void *self);
    void (*listen)(void *self);
    void (*destroy)(void *self);

    TCPServerInstance *instance;
} TCPserver;


TCPserver *getTCPserver();