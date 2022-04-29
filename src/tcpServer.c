#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "SDL2/SDL_net.h"
#include "tcpServer.h"
#define MAX_CLIENTS 4
#define MAX_SIZE 512
#define SERVER_PORT 3000
#define SERVER_IP "127.0.0.1"


void broadcastData(void *self, Client sender, Data *data, int dataSize);

bool TCPinitServer (void *self)
{
    TCPServerInstance *instance = ((TCPserver*)self)->instance;
    if(SDLNet_Init()==-1)
	{
		printf("SDLNet_Init: %s\n", SDLNet_GetError());
		return false;
	}
    if(SDLNet_ResolveHost(&(instance->serverAddress), NULL, SERVER_PORT))
    {
        printf("SDLNet_ResolveHost: %s\n", SDLNet_GetError());
		return false;
    }
    instance->socketSet = SDLNet_AllocSocketSet(MAX_CLIENTS + 1);
    if (instance->socketSet == NULL)
    {
        fprintf(stderr, "TCP_AllocSocket_Set error: %s\n", SDLNet_GetError());
        return false;
    }
    if (!(instance->serverSocket = SDLNet_TCP_Open(&instance->serverAddress)))
    {
        fprintf(stderr, "TCP_Open error: %s", SDLNet_GetError());
        return false;
    }

    instance->isRunning = true;
    return true;
}

void checknrOfSockets(void *self){
    TCPServerInstance *instance = ((TCPserver*)self)->instance;
   // printf("checking for number of sockets\n");

    int nrOfActiveSocket=SDLNet_CheckSockets(instance->socketSet,1000);
    if(nrOfActiveSocket==-1)
    {
        printf("SDLNet_checksocket: %s\n",SDLNet_GetError());
    }
    else if(nrOfActiveSocket)
    {
        printf("%d sockets with activity\n",nrOfActiveSocket);
        if(SDLNet_SocketReady(instance->serverSocket))
        {
            TCPsocket tmpClient=SDLNet_TCP_Accept(instance->serverSocket);
            printf("Inside socketReady() \n");
        }
    }
    return;
}

void listenConnection (void *self)
{
    TCPsocket tmpSock;
    TCPServerInstance *instance = ((TCPserver*)self)->instance;

    //listen for new connections;
    tmpSock = SDLNet_TCP_Accept(instance->serverSocket);

    if(tmpSock != NULL)
    {
        printf("new Client\n");
        instance->clients[instance->numOfClients].socket = tmpSock;
        instance->clients[instance->numOfClients].ip = *SDLNet_TCP_GetPeerAddress(tmpSock);
        instance->clients[instance->numOfClients].id = instance->currentID++;
        SDLNet_TCP_AddSocket(instance->socketSet, tmpSock);
        instance->numOfClients++;
    }
}

void loopClients(void *self){
    printf("loopClients\n");
    TCPServerInstance *instance = ((TCPserver*)self)->instance;
    //listen for incomming packages from all clients
    printf("before for\n");
    for (int i = 0; i < instance->numOfClients; i++)
    {
        if(instance->clients[i].socket==NULL){
            continue;
        }
        if(instance->nrOfRdy<=0){
            break;
        }
        else if (SDLNet_SocketReady(instance->clients[i].socket)) 
        {
            if(SDLNet_TCP_Recv(instance->clients[i].socket, &instance->clients[i].data, sizeof(Data)) > 0)
            {
                printf("new package from tempClientID %d  (x:%d, y:%d, from:%d)\n", instance->clients[i].id, instance->clients[i].data.x, instance->clients[i].data.y, instance->clients[i].data.from);
                instance->nrOfRdy--;//! ready tempClient in main -1
                //broadcast data to all tempClients exept sender
                instance->clients[i].data.from = instance->clients[i].id;
                broadcastData(self, instance->clients[i], &instance->clients[i].data, sizeof(Data));
            }
        }
    }
}


void broadcastData(void *self, Client sender, Data *data, int dataSize){
    TCPServerInstance *instance = ((TCPserver*)self)->instance;

    for (int i = 0; i < instance->numOfClients; i++)
    {
        if(sender.socket==instance->clients[i].socket) continue;
        int r = SDLNet_TCP_Send(instance->clients[i].socket,data,dataSize);
        printf("Sent %d byte to client %d\n",r,instance->clients[i].id);
    }
    
}

void TCPServerdestroy(void *self)
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
    self.listenConnection = listenConnection;
    self.checknrOfSockets = checknrOfSockets;
    self.loopClients = loopClients;
    self.destroy = TCPServerdestroy;

    self.instance->serverSocket = NULL;
    self.instance->numOfClients = 0;
    self.instance->currentID = 0;
    self.instance->isRunning = false;

    return &self;
}






