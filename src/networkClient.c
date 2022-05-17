#include "networkClient.h"
#include "tcpClient.h"
#include "udpClient.h"

struct networkClientInstance
{
    UDPclient *UDP;
    TCPclient *TCP;
    bool isAllClientJoined;
};

bool networkClientInnit(void *self)
{
    NetworkClientInstance *instance = ((NetworkClient *)self)->instance;

    if (!instance->TCP->init(instance->TCP))
    {
        printf("Failed to init TCP client\n");
        return false;
    }
    if (!instance->UDP->init(instance->UDP))
    {
        printf("Failed to init UDP client\n");
        return false;
    }
    instance->UDP->makeHandShake(instance->UDP);

    return true;
}

void networkClientListen(void *self)
{
    NetworkClientInstance *instance = ((NetworkClient *)self)->instance;
    instance->TCP->listen(instance->TCP);
    instance->UDP->listen(instance->UDP);
}

int networkTCPbroadcast(void *self, void *data, int dataSize, int dataType)
{
    NetworkClientInstance *instance = ((NetworkClient *)self)->instance;
    return instance->TCP->broadCast(instance->TCP, data, dataSize, dataType);
}

void networkUDPbroadcast(void *self, void *data, int dataSize, int dataType)
{
    NetworkClientInstance *instance = ((NetworkClient *)self)->instance;
    instance->UDP->broadCast(instance->UDP, data, dataSize, dataType);
}

void connect(void *self, int totalPlayer)
{
    NetworkClientInstance *instance = ((NetworkClient *)self)->instance;

    UDPclient *UDP = instance->UDP;
    TCPclient *TCP = instance->TCP;
    while (TCP->getNrOfClients(TCP) < totalPlayer || UDP->getNrOfClients(UDP) < totalPlayer)
    {
        networkClientListen(self);
    }
    instance->isAllClientJoined = true;
    printf("All client joined!\n");
}

bool getIsAllClientJoined(void *self)
{
    return ((NetworkClient *)self)->instance->isAllClientJoined;
}

int getNrOfClientsTCP(void *self)
{
    NetworkClientInstance *instance = ((NetworkClient *)self)->instance;
    TCPclient *TCP = instance->TCP;
    return TCP->getNrOfClients(TCP);
}

int networkGetTCPID(void *self)
{
    TCPclient *TCP = ((NetworkClient *)self)->instance->TCP;
    return TCP->getID(TCP);
}

NetworkClient *getNetworkClient()
{
    static NetworkClient self;
    if (self.instance != NULL)
        return &self;

    self.instance = malloc(sizeof(NetworkClientInstance));
    self.instance->UDP = getUDPclient();
    self.instance->TCP = getTCPclient();
    self.instance->isAllClientJoined = false;

    self.init = networkClientInnit;
    self.listen = networkClientListen;
    self.TCPbroadCast = networkTCPbroadcast;
    self.UDPbroadCast = networkUDPbroadcast;
    self.connect = connect;
    self.getTCPID = networkGetTCPID;
    self.getNrOfClientTCP = getNrOfClientsTCP;
    self.getIsAllClientJoined = getIsAllClientJoined;

    return &self;
}
