#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "SDL2/SDL_net.h"
#include "tcpServer.h"
#include "data.h"
#define MAX_CLIENTS 16
#define MAX_SIZE 512
#define SERVER_PORT 3000
#define SERVER_IP "127.0.0.1"

typedef struct client
{
    int id;
    IPaddress ip;
    TCPsocket socket;
    void* data;
} Client;

struct tcpServerInstance
{
    Client clients[MAX_CLIENTS];
    SDLNet_SocketSet socketSet;
    TCPsocket serverSocket;
    IPaddress serverAddress;
    int numOfClients;
    int currentID;
    bool isRunning;
};

void broadcastData(void *self, Client sender, void *data, int dataSize);

bool TCPinitServer(void *self)
{
    TCPServerInstance *instance = ((TCPserver *)self)->instance;
    if (SDLNet_Init() == -1)
    {
        printf("SDLNet_Init: %s\n", SDLNet_GetError());
        return false;
    }
    if (SDLNet_ResolveHost(&(instance->serverAddress), NULL, SERVER_PORT))
    {
        printf("SDLNet_ResolveHost: %s\n", SDLNet_GetError());
        return false;
    }
    instance->socketSet = SDLNet_AllocSocketSet(MAX_CLIENTS);
    if (instance->socketSet == NULL)
    {
        fprintf(stderr, "TCP_AllocSocket_Set error: %s\n", SDLNet_GetError());
        return false;
    }
    if (!(instance->serverSocket = SDLNet_TCP_Open(&instance->serverAddress)))
    {
        fprintf(stderr, "UDP_Open error: %s", SDLNet_GetError());
        return false;
    }

    instance->isRunning = true;
    return true;
}

void TCPlisten(void *self)
{
    TCPsocket tmpSock;
    TCPServerInstance *instance = ((TCPserver *)self)->instance;

    // listen for new connections;
    tmpSock = SDLNet_TCP_Accept(instance->serverSocket);
    if (tmpSock != NULL && instance->numOfClients <= MAX_CLIENTS)
    {
        printf("new Client\n");
        instance->clients[instance->numOfClients].socket = tmpSock;
        instance->clients[instance->numOfClients].ip = *SDLNet_TCP_GetPeerAddress(tmpSock);
        instance->clients[instance->numOfClients].id = instance->currentID++;
        instance->clients[instance->numOfClients].socket = tmpSock;
        SDLNet_TCP_AddSocket(instance->socketSet, tmpSock);
        instance->numOfClients++;
    }
    // listen for incomming packages from all clients
    while (SDLNet_CheckSockets(instance->socketSet, 0) > 0)
    {
        for (int i = 0; i < instance->numOfClients; i++)
        {
            if (SDLNet_SocketReady(instance->clients[i].socket))
            {
                Client *client = &instance->clients[i];
                int size = SDLNet_TCP_Recv(instance->clients[i].socket, &client->data, MAX_SIZE);
                if (size > 0)
                {
                    printf("new package from clientID %d\n", client->id);
                    broadcastData(self, *client, &client->data, size);
                }
            }
        }
    }
}

void broadcastData(void *self, Client sender, void *data, int dataSize)
{
    TCPServerInstance *instance = ((TCPserver *)self)->instance;

    for (int i = 0; i < instance->numOfClients; i++)
    {
        if (sender.id == instance->clients[i].id)
            continue;
        int r = SDLNet_TCP_Send(instance->clients[i].socket, data, dataSize);
        printf("Sent %d bite tO client %d\n", r, instance->clients[i].id);
    }
}

void TCPdestroy(void *self)
{
    TCPserver *server = ((TCPserver *)self);
    SDLNet_FreeSocketSet(server->instance->socketSet);
    free(server->instance);
    SDLNet_Quit();
    printf("Data has been destroyed!\n");
}

TCPserver *getTCPserver()
{
    static TCPserver self;
    if (self.instance != NULL)
        return &self;
    self.instance = malloc(sizeof(TCPServerInstance));
    self.init = TCPinitServer;
    self.listen = TCPlisten;

    self.instance->serverSocket = NULL;
    self.instance->numOfClients = 0;
    self.instance->currentID = 0;
    self.instance->isRunning = false;

    return &self;
}