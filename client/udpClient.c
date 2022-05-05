#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "SDL2/SDL_net.h"
#include "udpClient.h"
#include "data.h"
#include "map.h"


bool sendUdpPacageToServer(void *self, void* data, unsigned long len);

struct udpClientInstance
{
    UDPsocket serverSocket;
    UDPpacket *packetReceived;
    UDPpacket *packetSent;
    int numOfClients; // includeing it self
    IPaddress serverAddress;
    int port;
    int id;
    bool isRunning;
};


bool clientIsRunning(void *self)
{
    return ((UDPclient *)self)->instance->isRunning;
}

bool UDPinit(void *self)
{
    UDPclient *client = ((UDPclient *)self);
    if (SDLNet_Init() < 0)
    {
        fprintf(stderr, "Init error: %s\n", SDLNet_GetError());
        return false;
    }
    if (SDLNet_ResolveHost(&(client->instance->serverAddress), SERVER_IP, UDP_SERVER_PORT) == -1)
    {
        fprintf(stderr, "SDLNet_ResolveHost(192.0.0.1 3000): %s\n", SDLNet_GetError());
        return false;
    }
    if (!(client->instance->serverSocket = SDLNet_UDP_Open(client->instance->port)))
    {
        fprintf(stderr, "UDP_Open error: %s", SDLNet_GetError());
        return false;
    }
    if (!((client->instance->packetReceived = SDLNet_AllocPacket(MAX_SIZE)) && (client->instance->packetSent = SDLNet_AllocPacket(MAX_SIZE))))
    {
        fprintf(stderr, "UDP_Alloc error: %s\n", SDLNet_GetError());
        return false;
    }
    client->makeHandShake(client);
    SDL_Delay(2500);
    client->listen(client);
    client->instance->isRunning = true;

    return true;
}

void UDPClientdestroy(void *self)
{
    UDPclient *client = ((UDPclient *)self);

    SDLNet_FreePacket(client->instance->packetReceived);
    SDLNet_FreePacket(client->instance->packetSent);
    free(client->instance);
    SDLNet_Quit();
    printf("Client Has been Destroyed!\n");
}

void UDPbroadCast(void *self, DataClient *data, unsigned long length)
{
    UDPclient *client = ((UDPclient *)self);
    client->instance->packetSent->address.host = client->instance->serverAddress.host;
    client->instance->packetSent->address.port = client->instance->serverAddress.port;
    memcpy((char *)client->instance->packetSent->data, data, length);
    client->instance->packetSent->len = length;
    SDLNet_UDP_Send(client->instance->serverSocket, -1, client->instance->packetSent);
    printf("Data has been sent!\n");
}

void UDPclientListen(void *self)
{
    MapManager *mapManager = getMapManager();//!

    UDPClientInstance *instance = ((UDPclient *)self)->instance;   
    if (SDLNet_UDP_Recv(instance->serverSocket, instance->packetReceived))
    {
        if(instance->packetReceived->len == sizeof(int))
        {
            instance->id = *((int*)instance->packetReceived->data);
            printf("UDPid: %d\n", instance->id );
        }else if(instance->packetReceived->len == sizeof(DataClient))
        {
            DataClient data;
            char * dataRecieved = (char *)instance->packetReceived->data;
            int len = instance->packetReceived->len;
            // printf("len: %d\n", len);
            memcpy(&data, dataRecieved, len);
            printf("from: %d, x: %.2f, y: %.2f\n", data.from, data.x, data.y);
            int x = data.x;
            int y = data.y;
            printf("int convert x%d y%d\n",x,y);
            mapManager->digNoSend(mapManager,data.x,data.y);//!diging the map
            //  mapManager->dig(mapManager,5,8);//test dig
        }
        
    }
}

int getID(void *self){
    return ((UDPclient *)self)->instance->id;
}

void UDPmakeHandShake(void *self)
{
    UDPclient *client = ((UDPclient *)self);
    int flag = 1;

    if(!sendUdpPacageToServer(self, &flag, sizeof(int)))
        printf("failed to connection request\n");
    else
        printf("sent connection request!\n");
}
void UDPcloseConnection(void *self)
{
    UDPclient *client = ((UDPclient *)self);
    int flag = 0;

    if(!sendUdpPacageToServer(self, &flag, sizeof(int)))
        printf("failed to disconnect request\n");
    else
        printf("Packge sent disconnect request!\n");
}

UDPclient *getUDPclient()
{
    static UDPclient self;
    if (self.instance != NULL)
        return &self;

    self.instance = malloc(sizeof(UDPClientInstance));
    self.init = UDPinit;
    self.isRunning = clientIsRunning;
    self.destroy = UDPClientdestroy;
    self.makeHandShake = UDPmakeHandShake;
    self.closeConnection = UDPcloseConnection;
    self.broadCast = UDPbroadCast;
    self.listen = UDPclientListen;
    self.getID = getID;
    self.instance->numOfClients = 1;
    self.instance->isRunning = false;
    self.instance->port = CLIENT_PORT;

    return &self;
}

bool sendUdpPacageToServer(void *self, void*data, unsigned long len)
{
    UDPclient *client = ((UDPclient *)self);
    client->instance->packetSent->address.host = client->instance->serverAddress.host;
    client->instance->packetSent->address.port = client->instance->serverAddress.port;
    memcpy((char *)client->instance->packetSent->data, data, len);
    client->instance->packetSent->len = len;

    if(!SDLNet_UDP_Send(client->instance->serverSocket, -1, client->instance->packetSent))
        return false;
    return true;
}
