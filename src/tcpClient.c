#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "SDL2/SDL_net.h"
#include "tcpClient.h"
#include "data.h"

#define MAX_SIZE 512
#define CLIENT_PORT 0
#define CLIENT_IP "127.0.0.1" // 127.0.0.1
#define SERVER_PORT 3000
#define SERVER_IP "127.0.0.1"

struct TCPclientInstance
{
    TCPsocket serverSocket;
    SDLNet_SocketSet socketSet;
    Data packetReceived;
    Data packetSent;
    int numOfClients; // includeing it self
    IPaddress serverAddress;
    int port;
    int id;
    bool isRunning;
};

bool TCPinitclient(void *self)
{
    TCPClientInstance *instance = ((TCPclient *)self)->instance;
    if (SDLNet_Init() == -1)
    {
        printf("SDLNet_Init: %s\n", SDLNet_GetError());
        return false;
    }
    if (SDLNet_ResolveHost(&(instance->serverAddress), SERVER_IP, SERVER_PORT))
    {
        printf("SDLNet_ResolveHost: %s\n", SDLNet_GetError());
        return false;
    }
    if (!(instance->serverSocket = SDLNet_TCP_Open(&instance->serverAddress)))
    {
        fprintf(stderr, "UDP_Open error: %s", SDLNet_GetError());
        return false;
    }
    instance->socketSet = SDLNet_AllocSocketSet(1);
    if (instance->socketSet == NULL)
    {
        fprintf(stderr, "TCP_AllocSocket_Set error: %s\n", SDLNet_GetError());
        return false;
    }
    SDLNet_TCP_AddSocket(instance->socketSet, instance->serverSocket);

    instance->isRunning = true;
    return true;
}

void TCPlisten(void *self)
{
    TCPClientInstance *instance = ((TCPclient *)self)->instance;

    while (SDLNet_CheckSockets(instance->socketSet, 0))
    {
        if (SDLNet_SocketReady(instance->serverSocket))
        {
            if (SDLNet_TCP_Recv(instance->serverSocket, &instance->packetReceived, sizeof(Data)) > 0)
            {
                printf("got TCP packet from client %d. data x: %d y: %d\n", instance->packetReceived.from, instance->packetReceived.x, instance->packetReceived.y);
            }
        }
    }
}

int TCPbroadCast(void *self, Data *data, int dataSize)
{
    TCPClientInstance *instance = ((TCPclient *)self)->instance;
    int amoutSent = SDLNet_TCP_Send(instance->serverSocket, data, dataSize);
    printf("Sent TCP packet to server\n");
    return amoutSent;
}

TCPclient *getTCPclient()
{
    static TCPclient self;
    if (self.instance != NULL)
        return &self;
    self.instance = malloc(sizeof(TCPClientInstance));

    self.init = TCPinitclient;
    self.broadCast = TCPbroadCast;
    self.listen = TCPlisten;

    self.instance->serverSocket = NULL;
    self.instance->numOfClients = 0;
    self.instance->isRunning = false;

    return &self;
}