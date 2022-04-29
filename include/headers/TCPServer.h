#pragma once
#include <stdbool.h>
#include "SDL2/SDL_net.h"
#define MAX_CLIENTS 4

typedef struct data
{
    int x;
    int y;
    int from;
} Data;

typedef struct client
{
    int id;
    IPaddress ip;
    TCPsocket socket;
    Data data;
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
    bool (*isRunning)(void *self);
    void (*destroy)(void *self);
    void (*listenConnection)(void *self);
    void (*checknrOfSockets)(void *self);
    void (*loopClients)(void *self);

    TCPServerInstance *instance;
} TCPserver;


TCPserver *getTCPserver();