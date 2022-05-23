#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "SDL2/SDL_net.h"
#include "udpClient.h"
#include "EntityManager.h"
#include "Warrior.h"
#include "string.h"
#include "networkClient.h"
#include "startMenu.h"


#define MAX_CLIENTS 16
#define MAX_SIZE 512
#define CLIENT_PORT 0
#define CLIENT_IP "127.0.0.1" // 127.0.0.1
#define SERVER_PORT 7000
#define SERVER_IP "127.0.0.1"

bool sendUdpPacageToServer(void *self, void *data, unsigned long len, int dataType);

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
    // StartMenu *startMenu = getStartMenu();
    // char *serverIP = startMenu->getIP(startMenu);
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
    instance->isRunning = true;//!remove this

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

void UDPbroadCast(void *self, void *data, unsigned long length, int dataType)
{
    sendUdpPacageToServer(self, data, length, dataType);
}

void UDPclientListen(void *self)
{
    ClientInstance *instance = ((UDPclient *)self)->instance;
    if (SDLNet_UDP_Recv(instance->serverSocket, instance->packetReceived))
    {
        if (((char *)instance->packetReceived->data)[0] == (char)1)
        {
            Connection *data = (Connection *)(instance->packetReceived->data + 1);
            if (instance->numOfClients < data->totalClient)
                instance->numOfClients = data->totalClient;
            printf("UDP Client id: %d, total Client:%d\n", data->myId, data->totalClient);
        }
        else if (((char *)instance->packetReceived->data)[0] == (char)3)
        {
            // printf("Update Warrior from: %d (x:%d, y:%d) size: %d, %d\n", ws->from, ws->x, ws->y, instance->packetReceived->len, (int)sizeof(WarriorSnapshot));
            WarriorSnapshot *ws = (WarriorSnapshot *)(instance->packetReceived->data + 1);
            EntityManager *entityManager = getEntityManager();
            // NetworkClient *networkClient = getNetworkClient();
            // int idInt = networkClient->getTCPID(networkClient);
            int idInt = ws->from;
            char id[15];
            strcpy(id, "Warrior-000");
            int a = 100;
            for (int i = 8; i < 11; i++)
            {
                id[i] += (int)(idInt / a);
                idInt %= a;
                a /= 10;
            }
            Warrior *wa = entityManager->getByID(entityManager, id);
            if (wa != NULL)
                wa->updatePossition(wa, ws->x, ws->y);
                wa->setHealth(wa,ws->health);

        }
        else
        {
            printf("!got new UDP package with flag: %d\n", ((int *)instance->packetReceived->data)[0]);
        }
    }
}

void UDPmakeHandShake(void *self)
{
    UDPclient *client = ((UDPclient *)self);
    Connection data = {3, 6};

    if (!sendUdpPacageToServer(self, &data, sizeof(Connection), 1))
        printf("failed to connection request\n");
    else
        printf("sent connection request!\n");
}
void UDPcloseConnection(void *self)
{
    UDPclient *client = ((UDPclient *)self);
    int flag = 0;

    if (!sendUdpPacageToServer(self, &flag, sizeof(int), 2))
        printf("failed to disconnect request\n");
    else
        printf("Packge sent disconnect request!\n");
}

int getUDPNrOfClients(void *self)
{
    return ((UDPclient *)self)->instance->numOfClients;
}

int getUDPID(void *self)
{
    return ((UDPclient *)self)->instance->id;
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
    self.getNrOfClients = getUDPNrOfClients;
    self.getID = getUDPID;

    self.instance->numOfClients = 1;
    self.instance->isRunning = false;
    self.instance->port = CLIENT_PORT;

    return &self;
}

bool sendUdpPacageToServer(void *self, void *data, unsigned long len, int dataType)
{
    UDPclient *client = ((UDPclient *)self);
    client->instance->packetSent->address.host = client->instance->serverAddress.host;
    client->instance->packetSent->address.port = client->instance->serverAddress.port;

    ((char *)client->instance->packetSent->data)[0] = (char)dataType;
    memcpy((char *)(client->instance->packetSent->data + 1), data, len);
    client->instance->packetSent->len = len + 1;

    if (!SDLNet_UDP_Send(client->instance->serverSocket, -1, client->instance->packetSent))
        return false;
    return true;
}
