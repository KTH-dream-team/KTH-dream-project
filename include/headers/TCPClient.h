#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "SDL2/SDL_net.h"
#include "tcpClient.h"

#define MAX_CLIENTS 4
#define MAX_SIZE 512
#define CLIENT_PORT 0
#define CLIENT_IP "127.0.0.1"
#define SERVER_PORT 3000
#define SERVER_IP "127.0.0.1"

typedef struct data
{
    int x;
    int y;
    int from;
} Data;

struct TCPclientInstance
{
    TCPsocket serverSocket;
    SDLNet_SocketSet *clientSocketSet;
    Data *packetReceived;
    Data *packetSent;
    int nrOfSocketRdy; // includeing it self
    IPaddress serverAddress;
    int port;
    int id;
    bool isRunning;
};

int setRand(void *self);

typedef struct TCPclientInstance TCPClientInstance;


typedef struct {

    bool (*init)(void *self);
    bool (*isRunning)(void *self);
    void (*destroy)(void *self);
    void (*recive)(void *self);
    void (*listen)(void *self);
    int (*broadCast)(void *self, Data *data, int dataSize);
    TCPClientInstance *instance;
} TCPclient;


TCPclient *getTCPclient();