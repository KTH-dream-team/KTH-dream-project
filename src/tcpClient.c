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
    void* packetReceived;
    void* packetSent;
    int numOfClients; // includeing it self
    IPaddress serverAddress;
    int port;
    int id;
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

    return true;
}

void TCPlisten(void *self)
{
    TCPClientInstance *instance = ((TCPclient *)self)->instance;

    while (SDLNet_CheckSockets(instance->socketSet, 0))
    {
        if (SDLNet_SocketReady(instance->serverSocket))
        {
            int size = SDLNet_TCP_Recv(instance->serverSocket, instance->packetReceived, sizeof(Data));
            if (size == sizeof(int))
            {
                int a = *(int*)instance->packetReceived;
                if(instance->numOfClients < a)
                    instance->numOfClients = a;
                printf("nOc:%d\n",instance->numOfClients);
            }
            else if(size == sizeof(Data))
            {
                printf("got TCP packet from client (Data).\n");
            }
        }
    }
}

int getNrOfTCPClients(void *self)
{
    return ((TCPclient *)self)->instance->numOfClients;
}

int TCPbroadCast(void *self, void *data, int dataSize)
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
    self.getNrOfClients = getNrOfTCPClients;

    self.instance->serverSocket = NULL;
    self.instance->numOfClients = 0;
    self.instance->packetReceived = malloc(MAX_SIZE);
    self.instance->packetSent = malloc(MAX_SIZE);

    return &self;
}