#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "SDL2/SDL_net.h"
#include "tcpClient.h"
#include "data.h"

struct TCPclientInstance
{
    TCPsocket serverSocket;
    SDLNet_SocketSet socketSet;
    void *packetReceived;
    int numOfClients; // includeing it self
    IPaddress serverAddress;
    int port;
    int id;
    bool isRunning;
};

int TCPresive (void *self, void *dest, int desireInt);
int TCPresiveID(void *self);

bool TCPinitclient(void *self)
{
    TCPClientInstance *instance = ((TCPclient*)self)->instance;
    if(SDLNet_Init()==-1)
	{
		printf("SDLNet_Init: %s\n", SDLNet_GetError());
		return false;
	}
    if(SDLNet_ResolveHost(&(instance->serverAddress), SERVER_IP, TCP_SERVER_PORT))
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
    while(TCPresiveID(instance));
    instance->isRunning = true;
    instance->numOfClients=1;//todo fix ++
    return true;
}

void TCPlisten (void *self)
{
    TCPClientInstance *instance = ((TCPclient*)self)->instance;

    while(SDLNet_CheckSockets(instance->socketSet,0) >0)
    {
        int nrOfready = SDLNet_SocketReady(instance->serverSocket);
        if(nrOfready>0)
        {
            int nrOfbytes = SDLNet_TCP_Recv(instance->serverSocket, instance->packetReceived, MAX_SIZE);
            if(nrOfbytes == 12) // position of other players
            {
                printf("inside ready == 12\n");
                DataPos otherWarrior = *(DataPos*)instance->packetReceived;
                printf("from: %d, x:%.2f, y:%.2f\n", otherWarrior.from, otherWarrior.x, otherWarrior.y);
                //printf("got TCP packet from client %d. data x: %d y: %d\n", instance->packetReceived,instance->packetReceived,instance->packetReceived);
            }
        }
    }
}

int TCPresiveID(void *self)
{
    TCPClientInstance *instance = ((TCPClientInstance*)self);

    int nrOfsocket = SDLNet_CheckSockets(instance->socketSet, 0);
    while(nrOfsocket>0)
    {
        int nrOfready = SDLNet_SocketReady(instance->serverSocket);
        if(nrOfready>0)
        {
            SDLNet_TCP_Recv(instance->serverSocket, &instance->packetReceived, sizeof(int));
            memcpy(&instance->id, &instance->packetReceived, sizeof(int));
            printf("ID: %d\n", instance->id);
            return 0 ;
        }
    }
    return 1;
}

int TCPresive (void *self, void *dest, int desireInt)
{
    TCPClientInstance *instance = ((TCPclient*)self)->instance;
    while(SDLNet_CheckSockets(instance->socketSet,0) >0)
    {
        int nrOfready = SDLNet_SocketReady(instance->serverSocket);
        if(nrOfready>0)
        {
            int nrOfBytes = SDLNet_TCP_Recv(instance->serverSocket, &instance->packetReceived, MAX_SIZE);
            printf("NrOfBytes: %d", nrOfBytes);
            if(nrOfBytes == desireInt)// DataPos
            {
                memcpy(dest, instance->packetReceived, sizeof(DataPos));
            }
        }
    }
    return -1;
}


int TCPbroadCast(void *self, void *data, int dataSize)
{
    TCPClientInstance *instance = ((TCPclient*)self)->instance;
    int amoutSent = SDLNet_TCP_Send(instance->serverSocket,data,dataSize);
    return amoutSent;
}

int TCPgetID(void *self)
{
    TCPClientInstance *instance = ((TCPclient*)self)->instance;
    return instance->id;
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
    self.getID = TCPgetID;
    self.listen = TCPlisten;
    self.recive = TCPresive;
    self.destroy = TCPClientdestroy;

    self.instance->numOfClients=0;
    self.instance->serverSocket = NULL;
    self.instance->isRunning = false;

    return &self;
}