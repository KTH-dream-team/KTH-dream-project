#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "SDL2/SDL_net.h"
#include "tcpServer.h"
#include "data.h"



void TCPListen(void *self);
void broadcastData(void *self, Client sender, TCPServerData *data, int dataSize);
void listenConnection (void *self);
void checkSockets(void *self);
void readySocket(void *self);


bool TCPinitServer (void *self)
{
    TCPServerInstance *instance = ((TCPserver*)self)->instance;
    if(SDLNet_Init()==-1)
	{
		printf("SDLNet_Init: %s\n", SDLNet_GetError());
		return false;
	}
    if(SDLNet_ResolveHost(&(instance->serverAddress), NULL, TCP_SERVER_PORT))
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
        fprintf(stderr, "TCP_Open error: %s", SDLNet_GetError());
        return false;
    }

    instance->isRunning = true;
    return true;
}

void checkSockets(void *self){
    TCPServerInstance *instance = ((TCPserver*)self)->instance;
    instance->nrOfRdy+=SDLNet_CheckSockets(instance->socketSet,1000);
}

void TCPlisten(void *self)
{
    TCPserver *server = ((TCPserver*)self);
    listenConnection(server);
    checkSockets(server);
    readySocket(server);
}

void listenConnection (void *self)
{
    TCPsocket tmpSock;
    TCPServerInstance *instance = ((TCPserver*)self)->instance;

    //listen for new connections;
    tmpSock = SDLNet_TCP_Accept(instance->serverSocket);

    if(tmpSock != NULL)
    {
        instance->clients[instance->numOfClients].socket = tmpSock;
        instance->clients[instance->numOfClients].ip = *SDLNet_TCP_GetPeerAddress(tmpSock);
        instance->clients[instance->numOfClients].id = instance->currentID++;
        SDLNet_TCP_AddSocket(instance->socketSet, tmpSock);
        instance->numOfClients++;
        printf("---TCPClient: %d---\n", instance->clients[instance->numOfClients].id);
    }
}

void readySocket(void *self){
    TCPServerInstance *instance = ((TCPserver*)self)->instance;
    //listen for incomming packages from all clients
    for (int i = 0; i < instance->numOfClients; i++)
    {
        if(instance->nrOfRdy<=0){
            break;
        }
        else if (SDLNet_SocketReady(instance->clients[i].socket)) 
        {
            if(SDLNet_TCP_Recv(instance->clients[i].socket, &instance->clients[i].data, sizeof(BlockPos)) == 16)
            {
                printf("package from ClientID %d removed block (x:%.1f, y:%.1f,removed from:%d)\n", instance->clients[i].id, instance->clients[i].data.x, instance->clients[i].data.y, instance->clients[i].data.from);
                instance->nrOfRdy--;
                //broadcast data to all tempClients exept sender
                instance->clients[i].data.from = instance->clients[i].id;
                broadcastData(self, instance->clients[i], &instance->clients[i].data, sizeof(BlockPos));
            }
          
            
            if(SDLNet_TCP_Recv(instance->clients[i].socket, &instance->clients[i].data, sizeof(DataPos)) == 12)
            {
                printf("package from ClientID %d positon (x:%.2f, y:%.2f, from:%d)\n", instance->clients[i].id, instance->clients[i].data.x, instance->clients[i].data.y, instance->clients[i].data.from);
                instance->nrOfRdy--;

                //broadcast data to all tempClients exept sender
                instance->clients[i].data.from = instance->clients[i].id;
                broadcastData(self, instance->clients[i], &instance->clients[i].data, sizeof(DataPos));
            }
        }
    }
}


void broadcastData(void *self, Client sender, TCPServerData *data, int dataSize){
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
    self.listen = TCPlisten;
    self.destroy = TCPServerdestroy;

    self.instance->serverSocket = NULL;
    self.instance->numOfClients = 0;
    self.instance->currentID = 0;
    self.instance->isRunning = false;

    return &self;
}