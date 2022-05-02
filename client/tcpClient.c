#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "SDL2/SDL_net.h"
#include "tcpClient.h"

struct TCPclientInstance
{
    TCPsocket serverSocket;
    SDLNet_SocketSet socketSet;
    void* packetReceived;
    int numOfClients; // includeing it self
    IPaddress serverAddress;
    int port;
    int id;
    bool isRunning;
};

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
    instance->socketSet = SDLNet_AllocSocketSet(1); 
    if (instance->socketSet == NULL)
    {
        fprintf(stderr, "TCP_AllocSocket_Set error: %s\n", SDLNet_GetError());
        return false;
    }
    if((SDLNet_TCP_AddSocket(instance->socketSet, instance->serverSocket))<0)
    {
        printf("SDLNet_AddSocket: %s\n", SDLNet_GetError());
        return false;
    }

    instance->isRunning = true;
    instance->numOfClients=1;//todo fix ++
    return true;
}
void TCPlisten (void *self)
{
    TCPClientInstance *instance = ((TCPclient*)self)->instance;

    while(SDLNet_CheckSockets(instance->socketSet,0) >0)
    {
        int nrOfbytes=SDLNet_SocketReady(instance->serverSocket);
        if(nrOfbytes>0)
        {
            if(SDLNet_TCP_Recv(instance->serverSocket, &instance->packetReceived,nrOfbytes) > 0)
            {
                printf("nr of bytes %d \n",nrOfbytes);//!remove
                // printf("got TCP packet from client %d. data x: %d y: %d\n", instance->packetReceived.from,instance->packetReceived.x,instance->packetReceived.y);
            }
        }
    }
}
int TCPbroadCast(void *self, void *data, int dataSize)
{
    TCPClientInstance *instance = ((TCPclient*)self)->instance;
    int amoutSent = SDLNet_TCP_Send(instance->serverSocket,data,dataSize);
    return amoutSent;
}

int setRand(void *self){
    TCPclient *client = ((TCPclient*)self);
    // client->instance->packetSent->x=rand()%10+1;
    // client->instance->packetSent->y=rand()%10+1;
    // client->instance->packetSent->from=rand()%10+1;
    // printf("from:%d x:%d y:%d\n",client->instance->packetSent->from,client->instance->packetSent->x,client->instance->packetSent->y);
    return 0;
}

void TCPClientdestroy(void *self){
    TCPclient *client = ((TCPclient *)self);

    SDLNet_TCP_Close(client->instance->serverSocket);
    SDLNet_TCP_DelSocket(client->instance->socketSet,client->instance->serverSocket);
    SDLNet_Quit();
    free(client->instance);
    printf("Data has been destroyed!\n");
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
    self.destroy = TCPClientdestroy;

    self.instance->numOfClients=0;
    self.instance->serverSocket = NULL;
    self.instance->isRunning = false;

    return &self;
}