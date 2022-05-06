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
#include "map.h"

#define MAX_SIZE 512
#define CLIENT_PORT 0
#define CLIENT_IP "127.0.0.1" // 127.0.0.1//130.229.166.16
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
            int offset=0;
            EntityManager *entityManager = getEntityManager();
            MapManager * map = getMapManager();
            while(size > offset)
            {
                switch(((char*)instance->packetReceived)[offset])
                {
                    case (char)1:  
                        offset ++;
                        Connection* a = (Connection*)(instance->packetReceived+offset);
                        if(instance->numOfClients < a->totalClient)
                            instance->numOfClients = a->totalClient;
                        instance->id = a->myId;
                        printf("TCP Client id: %d, total Client:%d\n", a->myId, a->totalClient);
                        offset += sizeof(Connection);
                    break;
                    case (char)2:
                        offset ++;
                        WarriorCreation * wa = (WarriorCreation*)(instance->packetReceived+offset);
                        Warrior *warrior = createWarrior(wa->x, wa->y, wa->from, wa->from, false);//!check if correct
                        char * idWarrior = warrior->getID(warrior);
                        entityManager->add(entityManager, idWarrior, warrior); // add to entity manager list
                        offset += sizeof(WarriorCreation);
                    break;
                    case (char)4:
                        offset ++;
                        ShootBullet * bullet = (ShootBullet*)(instance->packetReceived+offset);
                        SDL_FPoint velN = {bullet->velX, bullet->velY}; //! bullet velocity
                        SDL_FPoint pos = {bullet->x, bullet->y};
                        Bullet *bullet1 = newBullet(pos, velN, false);
                        char * idBullet = bullet1->getID(bullet1);
                        entityManager->add(entityManager, idBullet, bullet1);
                        offset += sizeof(ShootBullet);
                    break;
                    case (char)5:
                        offset ++;
                        BlockDestroy * blockDestroyed = (BlockDestroy*)(instance->packetReceived+offset);
                        map->digNoSend(map, blockDestroyed->x, blockDestroyed->y);
                        offset += sizeof(BlockDestroy);
                    break;
                    case (char)6:
                        offset ++;
                        BlockBuild * blockBuilt = (BlockBuild*)(instance->packetReceived+offset);
                        map->buildNoSend(map, blockBuilt->x, blockBuilt->y,blockBuilt->blockType);
                        offset += sizeof(BlockBuild);
                    break;
                    default:
                    offset ++;
                    printf("!got TCP packet with flag: %d.\n", ((int*)instance->packetReceived)[0]);
                    offset = size;
                    break;
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