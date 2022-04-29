#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "SDL2/SDL_net.h"
#include "tcpClient.h"



bool TCPinitclient(void *self)
{
    TCPClientInstance *instance = ((TCPclient*)self)->instance;
    if(SDLNet_Init()==-1)
	{
		printf("SDLNet_Init: %s\n", SDLNet_GetError());
		return false;
	}
    if(SDLNet_ResolveHost(&(instance->serverAddress), SERVER_IP, SERVER_PORT))
    {
        printf("SDLNet_ResolveHost: %s\n", SDLNet_GetError());
		return false;
    }
    if (!(instance->serverSocket = SDLNet_TCP_Open(&instance->serverAddress)))
    {
        fprintf(stderr, "TCP_Open error: %s", SDLNet_GetError());
        return false;
    }
    *instance->clientSocketSet = SDLNet_AllocSocketSet(MAX_CLIENTS + 1); // +1 for server
    if (*instance->clientSocketSet == NULL)
    {
        fprintf(stderr, "TCP_AllocSocket_Set error: %s\n", SDLNet_GetError());
        return false;
    }
    if((SDLNet_TCP_AddSocket(*instance->clientSocketSet, instance->serverSocket))<0)
    {
        printf("SDLNet_AddSocket: %s\n", SDLNet_GetError());
        return false;
    }

    instance->isRunning = true;
    return true;
}
void TCPlisten (void *self)
{
    TCPClientInstance *instance = ((TCPclient*)self)->instance;
    instance->nrOfSocketRdy = SDLNet_CheckSockets(*instance->clientSocketSet, 1000);
}
void TCPrecive (void *self)
{
    TCPClientInstance *instance = ((TCPclient*)self)->instance;
    if(SDLNet_TCP_Recv(instance->serverSocket, instance->packetReceived, sizeof(TCPClientData)))
    {
        printf("got message from client %d. \ndata \nx: %d \ny: %d\n", instance->packetReceived->from,instance->packetReceived->x,instance->packetReceived->y);
    }
}

int TCPbroadCast(void *self, TCPClientData *data, int dataSize)
{
    TCPClientInstance *instance = ((TCPclient*)self)->instance;
    int amoutSent = SDLNet_TCP_Send(instance->serverSocket,data,dataSize);
    return amoutSent;
}

int setRand(void *self){
    TCPclient *client = ((TCPclient*)self);
    client->instance->packetSent->x=rand()%10+1;
    client->instance->packetSent->y=rand()%10+1;
    client->instance->packetSent->from=rand()%10+1;
    printf("from:%d x:%d y:%d\n",client->instance->packetSent->from,client->instance->packetSent->x,client->instance->packetSent->y);
    return 0;
}

void TCPClientdestroy(void *self){
    TCPclient *client = ((TCPclient *)self);

    SDLNet_TCP_Close(client->instance->serverSocket);
    SDLNet_TCP_DelSocket(*client->instance->clientSocketSet,client->instance->serverSocket);
    SDLNet_Quit();
    free(client->instance->packetReceived);
    free(client->instance->packetSent);
    free(client->instance);
    printf("Data has been destroyed!\n");
}

TCPclient *getTCPclient()
{
    static TCPclient self;
    if (self.instance != NULL)
        return &self;
    self.instance = malloc(sizeof(TCPClientInstance));
    self.instance->packetReceived = malloc(sizeof(TCPClientData));
    self.instance->packetSent = malloc(sizeof(TCPClientData));
    self.instance->clientSocketSet = malloc(sizeof(SDLNet_SocketSet));
    
    self.init = TCPinitclient;
    self.broadCast = TCPbroadCast;
    self.listen = TCPlisten;
    self.recive = TCPrecive;
    self.destroy = TCPClientdestroy;

    self.instance->serverSocket = NULL;
    self.instance->nrOfSocketRdy = 0;
    self.instance->isRunning = false;


    return &self;
}