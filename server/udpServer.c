#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "SDL2/SDL_net.h"
#include "udpServer.h"
#include "data.h"


bool sendUdpPacageToClient(void *self, void *data, IPaddress destIP, UDPsocket destSoc, unsigned long len);
bool isClientExit(void *self, IPaddress address);

typedef struct data
{
    float x;
    float y;
    int from;
} Data;

typedef struct client
{
    int id;
    IPaddress ip;
    UDPsocket socket;
    Data data;
} Client;

struct udpServerInstance
{
    Client clients[MAX_CLIENTS];
    SDLNet_SocketSet socketSet;
    UDPsocket serverSocket;
    UDPpacket *packetReceived;
    UDPpacket *packetSent;
    IPaddress IP_Adress;
    int numOfClients;
    int currentID;
    int serverPort;
    bool isRunning;
};

bool serverIsRunning(void *self)
{
    return ((UDPserver *)self)->instance->isRunning;
}

bool UDPinitServer(void *self)
{
    UDPServerInstance *instance = ((UDPserver *)self)->instance;
    if (SDLNet_Init() < 0)
    {
        fprintf(stderr, "Init error: %s\n", SDLNet_GetError());
        return false;
    }
    if (!(instance->serverSocket = SDLNet_UDP_Open(instance->serverPort)))
    {
        fprintf(stderr, "UDP_Open error: %s", SDLNet_GetError());
        return false;
    }
    if (SDLNet_ResolveHost(&(instance->IP_Adress), NULL, UDP_SERVER_PORT) == -1)
    {
        fprintf(stderr, "SDLNet_ResolveHost(192.0.0.1 3000): %s\n", SDLNet_GetError());
        return false;
    }
    instance->socketSet = SDLNet_AllocSocketSet(MAX_CLIENTS);
    if (instance->socketSet == NULL)
    {
        fprintf(stderr, "UDP_AllocSocket_Set error: %s\n", SDLNet_GetError());
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

void UDPlisten(void *self)
{
    // listen to new connections
    UDPServerInstance *instance = ((UDPserver *)self)->instance;

    if (SDLNet_UDP_Recv(instance->serverSocket, instance->packetReceived))
    {

        if (instance->packetReceived->len == sizeof(int))
        {
            if (*((int *)instance->packetReceived->data) == 1)
            {
                Client *client = &(instance->clients[instance->numOfClients]);

                client->ip = instance->packetReceived->address;
                client->socket = SDLNet_UDP_Open(client->ip.port);
                SDLNet_UDP_AddSocket(instance->socketSet, client->socket);
                client->id = instance->currentID++;
                instance->numOfClients++;

                if (!sendUdpPacageToClient(self, &(client->id), client->ip, client->socket, sizeof(int)))
                    printf("faied to send package\n");
                else
                {
                    printf("new connection created\n");
                    for (int i = 0; i < instance->numOfClients; i++)
                    {
                        printf("client: %d, port: %d, ip: %d\n", instance->clients[i].id, instance->clients[i].ip.port, instance->clients[i].ip.host);
                    }
                }
            }
            else if (*((int *)instance->packetReceived->data) == 0)
            {
                printf("closed\n");
            }
        }
        else if (instance->packetReceived->len == sizeof(Data))
        {

            if (isClientExit(self, instance->packetReceived->address))
            {
                for (int i = 0; i < instance->numOfClients; i++)
                {
                    if (instance->clients[i].ip.host == instance->packetReceived->address.host && instance->clients[i].ip.port == instance->packetReceived->address.port)
                    {
                        memcpy(&instance->clients[i].data, (char *)instance->packetReceived->data, instance->packetReceived->len);
                        instance->clients[i].data.from = instance->clients[i].id;
                        printf("Data sent to all clients is: %.2f %.2f from client: %d\n", instance->clients[i].data.x, instance->clients[i].data.y, instance->clients[i].data.from);
                        continue;
                    }

                    if (sendUdpPacageToClient(self, instance->packetReceived->data, instance->clients[i].ip, instance->clients[i].socket, instance->packetReceived->len))
                        printf("send package to %d\n", instance->clients[i].id);
                }
            }
            else
            {
                printf("client not exist\n");
            }
        }
        //check clients data
        // printf("Clients data\n");
        // printf("----------------\n");
        // for (int i = 0; i < instance->numOfClients; i++)
        //     printf("Data client %d is: %d %d\n", instance->clients[i].id, instance->clients[i].data.x, instance->clients[i].data.y);
        // printf("----------------\n");
    }
}

void UDPdestroy(void *self)
{
    UDPserver *server = ((UDPserver *)self);
    SDLNet_FreeSocketSet(server->instance->socketSet);
    SDLNet_FreePacket(server->instance->packetReceived);
    SDLNet_FreePacket(server->instance->packetSent);
    free(server->instance);
    SDLNet_Quit();
    printf("Data has been destroyed!\n");
}

UDPserver *getUDPserver()
{
    static UDPserver self;
    if (self.instance != NULL)
        return &self;
    self.instance = malloc(sizeof(UDPServerInstance));
    self.init = UDPinitServer;
    self.listen = UDPlisten;
    self.isRunning = serverIsRunning;
    self.destroy = UDPdestroy;
    self.instance->serverPort = UDP_SERVER_PORT;
    self.instance->serverSocket = NULL;
    self.instance->numOfClients = 0;
    self.instance->currentID = 0;
    self.instance->isRunning = false;

    return &self;
}

bool sendUdpPacageToClient(void *self, void *data, IPaddress destIP, UDPsocket destSoc, unsigned long len)
{
    UDPServerInstance *instance = ((UDPserver *)self)->instance;
    instance->packetSent->address.host = destIP.host;
    instance->packetSent->address.port = destIP.port;
    memcpy((char *)instance->packetSent->data, data, len);
    instance->packetSent->len = len;

    if (!SDLNet_UDP_Send(destSoc, -1, instance->packetSent))
        return false;
    return true;
}

bool isClientExit(void *self, IPaddress adress)
{
    UDPServerInstance *instance = ((UDPserver *)self)->instance;
    for (int i = 0; i < instance->numOfClients; i++)
    {
        if (adress.host == instance->clients[i].ip.host && adress.port == instance->clients[i].ip.port)
            return 1;
    }
    return 0;
}