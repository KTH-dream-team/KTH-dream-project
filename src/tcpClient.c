#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "SDL2/SDL_net.h"
#include "tcpClient.h"
#include "data.h"
#include "EntityManager.h"
#include "Warrior.h"
#include "Bullet.h"

#define MAX_SIZE 512
#define CLIENT_PORT 0
#define CLIENT_IP "127.0.0.1" // 127.0.0.1
#define SERVER_PORT 3000
#define SERVER_IP "127.0.0.1"

struct TCPclientInstance
{
    TCPsocket serverSocket;
    SDLNet_SocketSet socketSet;
    void* packetReceived;
    void* packetSent;
    int numOfClients; // includeing it self
    IPaddress serverAddress;
    int port;
    int id;
};

bool TCPinitclient(void *self)
{
    TCPClientInstance *instance = ((TCPclient *)self)->instance;
    if (SDLNet_Init() == -1)
    {
        printf("SDLNet_Init: %s\n", SDLNet_GetError());
        return false;
    }
    if (SDLNet_ResolveHost(&(instance->serverAddress), SERVER_IP, SERVER_PORT))
    {
        printf("SDLNet_ResolveHost: %s\n", SDLNet_GetError());
        return false;
    }
    if (!(instance->serverSocket = SDLNet_TCP_Open(&instance->serverAddress)))
    {
        fprintf(stderr, "UDP_Open error: %s", SDLNet_GetError());
        return false;
    }
    instance->socketSet = SDLNet_AllocSocketSet(1);
    if (instance->socketSet == NULL)
    {
        fprintf(stderr, "TCP_AllocSocket_Set error: %s\n", SDLNet_GetError());
        return false;
    }
    SDLNet_TCP_AddSocket(instance->socketSet, instance->serverSocket);

    return true;
}

void TCPlisten(void *self)
{
    TCPClientInstance *instance = ((TCPclient *)self)->instance;

    while (SDLNet_CheckSockets(instance->socketSet, 0))
    {
        if (SDLNet_SocketReady(instance->serverSocket))
        {
            int size = SDLNet_TCP_Recv(instance->serverSocket, instance->packetReceived, MAX_SIZE);
            printf("new TCP packet size: %d\n", size);
            int offset=0;
            while(size > offset)
            {

                if (((char*)instance->packetReceived)[offset] == (char)1)
                {
                    offset ++;
                    Connection* a = (Connection*)(instance->packetReceived+1);
                    if(instance->numOfClients < a->totalClient)
                        instance->numOfClients = a->totalClient;
                    printf("My TCP ID:%d, nOc:%d\n",a->myId, instance->numOfClients);
                    instance->id = a->myId;
                    offset += sizeof(Connection);
                }
                else if (((char*)instance->packetReceived)[offset] == (char)2)
                {
                    offset ++;
                    WarriorCreation * wa = (WarriorCreation*)(instance->packetReceived+offset);
                    printf("Warrior Created by: %d, (x:%d, y:%d), package size: %d, data size:%d.\n",wa->from, wa->x, wa->y, size, (int)sizeof(WarriorCreation));
                    
                    EntityManager *entityManager = getEntityManager();
                    Warrior *warrior = createWarrior(wa->x, wa->y, wa->from, 23, false);
                    char * id = warrior->getID(warrior);
                    entityManager->add(entityManager, id, warrior); // add to entity manager list
                    offset += sizeof(WarriorCreation);
                }
                else if (((char*)instance->packetReceived)[offset] == (char)4)
                {
                    offset ++;
                    ShootBullet * bullet = (ShootBullet*)(instance->packetReceived+offset);
                    printf("fire Bullet from: %d.\n", bullet->from);

                    SDL_FPoint velN = {bullet->velX, bullet->velY}; //! bullet velocity
                    SDL_FPoint pos = {bullet->x, bullet->y};
                    Bullet *bullet1 = newBullet(pos, velN, false);
                    char * id = bullet1->getID(bullet1);
                    EntityManager *entityManager = getEntityManager();
                    entityManager->add(entityManager, id, bullet1);
                    offset = size;
                }
                else
                {
                    offset ++;
                    printf("!got TCP packet with flag: %d.\n", ((int*)instance->packetReceived)[0]);
                    offset = size;
                }

            }
        }
    }
}

int getNrOfTCPClients(void *self)
{
    return ((TCPclient *)self)->instance->numOfClients;
}

int TCPbroadCast(void *self, void *data, int dataSize, int dataType)
{
    TCPClientInstance *instance = ((TCPclient *)self)->instance;

    char buffer[MAX_SIZE];
    buffer[0] = (char)dataType;
    memcpy(buffer+1, data, dataSize);

    int amoutSent = SDLNet_TCP_Send(instance->serverSocket, buffer, dataSize+1);
    printf("Sent TCP packet to server, amount: %d\n", amoutSent);
    return amoutSent;
}

int getTCPID (void *self)
{
    return ((TCPclient *)self)->instance->id;
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
    self.getNrOfClients = getNrOfTCPClients;
    self.getID = getTCPID;

    self.instance->serverSocket = NULL;
    self.instance->numOfClients = 0;
    self.instance->packetReceived = malloc(MAX_SIZE);
    self.instance->packetSent = malloc(MAX_SIZE);

    return &self;
}