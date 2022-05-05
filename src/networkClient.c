#include "networkClient.h"
#include "tcpClient.h"
#include "udpClient.h"

struct networkClientInstance
{
    UDPclient *UDP;
    TCPclient *TCP;
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

int networkTCPbroadcast(void *self, Data *data, int dataSize)
{
    NetworkClientInstance *instance = ((NetworkClient *)self)->instance;
    return instance->TCP->broadCast(instance->TCP, data, dataSize);
}

void networkUDPbroadcast(void *self, void *data, int dataSize)
{
    NetworkClientInstance *instance = ((NetworkClient *)self)->instance;
    instance->UDP->broadCast(instance->UDP, data, dataSize);
}

void connect(void*self)
{
    UDPclient * UDP = ((NetworkClient *)self)->instance->UDP;
    TCPclient * TCP = ((NetworkClient *)self)->instance->TCP;
    while(TCP->getNrOfClients(TCP) < 3)
    {
        networkClientListen(self);
    }
    printf("All client joined!\n");
}

NetworkClient *getNetworkClient()
{
    static NetworkClient self;
    if (self.instance != NULL)
        return &self;

    self.instance = malloc(sizeof(NetworkClientInstance));
    self.instance->UDP = getUDPclient();
    self.instance->TCP = getTCPclient();

    self.init = networkClientInnit;
    self.listen = networkClientListen;
    self.TCPbroadCast = networkTCPbroadcast;
    self.UDPbroadCast = networkUDPbroadcast;
    self.connect = connect;

    return &self;
}
