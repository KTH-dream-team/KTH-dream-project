#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "SDL2/SDL_net.h"
#include "udpClient.h"

#define MAX_CLIENTS 16
#define MAX_SIZE 512
#define CLIENT_PORT 0
#define CLIENT_IP "127.0.0.1" // 127.0.0.1
#define SERVER_PORT 7000
#define SERVER_IP "127.0.0.1"

bool sendUdpPacageToServer(void *self, void *data, unsigned long len);

struct clientInstance
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
    ClientInstance *instance = ((UDPclient *)self)->instance;
    if (SDLNet_Init() < 0)
    {
        fprintf(stderr, "Init error: %s\n", SDLNet_GetError());
        return false;
    }
    if (SDLNet_ResolveHost(&(instance->serverAddress), SERVER_IP, SERVER_PORT) == -1)
    {
        fprintf(stderr, "SDLNet_ResolveHost(192.0.0.1 3000): %s\n", SDLNet_GetError());
        return false;
    }
    if (!(instance->serverSocket = SDLNet_UDP_Open(instance->port)))
    {
        fprintf(stderr, "UDP_Open error: %s", SDLNet_GetError());
        return false;
    }
    if (!((instance->packetReceived = SDLNet_AllocPacket(MAX_SIZE)) && (instance->packetSent = SDLNet_AllocPacket(MAX_SIZE))))
    {
        fprintf(stderr, "UDP_Alloc error: %s\n", SDLNet_GetError());
        return false;
    }
    instance->isRunning = true;

    return true;
}

void UDPdestroy(void *self)
{
    UDPclient *client = ((UDPclient *)self);

    SDLNet_FreePacket(client->instance->packetReceived);
    SDLNet_FreePacket(client->instance->packetSent);
    free(client->instance);
    SDLNet_Quit();
    printf("Client Has been Destroyed!\n");
}

void UDPbroadCast(void *self, void *data, unsigned long length)
{
    UDPclient *client = ((UDPclient *)self);
    client->instance->packetSent->address.host = client->instance->serverAddress.host;
    client->instance->packetSent->address.port = client->instance->serverAddress.port;
    memcpy((char *)client->instance->packetSent->data, data, length);
    client->instance->packetSent->len = length;
    SDLNet_UDP_Send(client->instance->serverSocket, -1, client->instance->packetSent);
    printf("Sent UDP packet to server\n");
}

void UDPclientListen(void *self)
{
    ClientInstance *instance = ((UDPclient *)self)->instance;
    if (SDLNet_UDP_Recv(instance->serverSocket, instance->packetReceived))
    {
        if (instance->packetReceived->len == sizeof(int))
        {
            int id = *((int *)instance->packetReceived->data);
            printf("UDP connection created successfully, client id: %d\n", id);
        }
        else if (instance->packetReceived->len == sizeof(Data))
        {
            Data data;
            char *dataRecieved = (char *)instance->packetReceived->data;
            int len = instance->packetReceived->len;
            memcpy(&data, dataRecieved, len);
            printf("got new UDP package from: %d, x: %d, y: %d\n", data.from, data.x, data.y);
        }
        else if (instance->packetReceived->len == sizeof(TestData))
        {
            TestData data;
            char *dataRecieved = (char *)instance->packetReceived->data;
            int len = instance->packetReceived->len;
            memcpy(&data, dataRecieved, len);
            printf("got new UDP package from: %d, a: %d\n", data.from, data.a);
        }
    }
}

void UDPmakeHandShake(void *self)
{
    UDPclient *client = ((UDPclient *)self);
    int flag = 1;

    if (!sendUdpPacageToServer(self, &flag, sizeof(int)))
        printf("failed to connection request\n");
    else
        printf("sent connection request!\n");
}
void UDPcloseConnection(void *self)
{
    UDPclient *client = ((UDPclient *)self);
    int flag = 0;

    if (!sendUdpPacageToServer(self, &flag, sizeof(int)))
        printf("failed to disconnect request\n");
    else
        printf("Packge sent disconnect request!\n");
}

UDPclient *getUDPclient()
{
    static UDPclient self;
    if (self.instance != NULL)
        return &self;

    self.instance = malloc(sizeof(ClientInstance));
    self.init = UDPinit;
    self.isRunning = clientIsRunning;
    self.destroy = UDPdestroy;
    self.makeHandShake = UDPmakeHandShake;
    self.closeConnection = UDPcloseConnection;
    self.broadCast = UDPbroadCast;
    self.listen = UDPclientListen;

    self.instance->numOfClients = 1;
    self.instance->isRunning = false;
    self.instance->port = CLIENT_PORT;

    return &self;
}

bool sendUdpPacageToServer(void *self, void *data, unsigned long len)
{
    UDPclient *client = ((UDPclient *)self);
    client->instance->packetSent->address.host = client->instance->serverAddress.host;
    client->instance->packetSent->address.port = client->instance->serverAddress.port;
    memcpy((char *)client->instance->packetSent->data, data, len);
    client->instance->packetSent->len = len;

    if (!SDLNet_UDP_Send(client->instance->serverSocket, -1, client->instance->packetSent))
        return false;
    return true;
}
