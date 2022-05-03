#include "networkClient.h"
#include "tcpClient.h"
#include "udpClient.h"
#include "data.h"


struct networkClientInstance
{
    UDPclient * UDP;
    TCPclient * TCP;
};

bool networkClientInnit(void *self)
{
    NetworkClientInstance *instance = ((NetworkClient*)self)->instance;

    if(!instance->TCP->init(instance->TCP))
    {
        printf("Failed to init TCP client\n");
        return false;
    }
    if(!instance->UDP->init(instance->UDP))
    {
        printf("Failed to init UDP client\n");
        return false;
    }
    instance->UDP->makeHandShake(instance->UDP);
    
    return true;
}

void networkClientListen (void *self)
{
    NetworkClientInstance *instance = ((NetworkClient*)self)->instance;
    instance->TCP->listen(instance->TCP); //! THIS BLOCKS game very hard!!!
    instance->UDP->listen(instance->UDP);
}

int networkTCPbroadcast(void *self, void *data, int dataSize)
{
    NetworkClientInstance *instance = ((NetworkClient*)self)->instance;
    return instance->TCP->broadCast(instance->TCP, data, dataSize);
}

void networkUDPbroadcast(void *self, void *data, int dataSize)
{
    NetworkClientInstance *instance = ((NetworkClient*)self)->instance;
    instance->UDP->broadCast(instance->UDP, data, dataSize);
}

void networkClientDestory (void *self)
{
    NetworkClientInstance *instance = ((NetworkClient*)self)->instance;
    instance->TCP->destroy(instance->TCP);
    instance->UDP->destroy(instance->UDP);
}

int networkTCPresive(void *self, void *dest)
{
    printf("Inside NetworkTCPresive\n");
    NetworkClientInstance *instance = ((NetworkClient*)self)->instance;
    return instance->TCP->recive(instance->TCP,dest);
}

NetworkClient * getNetworkClient()
{
    static NetworkClient self;
    if(self.instance != NULL)
        return &self;

    self.instance = malloc(sizeof(NetworkClientInstance));
    self.instance->UDP = getUDPclient();
    self.instance->TCP = getTCPclient();

    
    self.init = networkClientInnit;
    self.listen = networkClientListen;
    self.TCPresive = networkTCPresive;
    self.TCPbroadCast = networkTCPbroadcast;
    self.UDPbroadCast = networkUDPbroadcast;
    self.destroy = networkClientDestory;
    return &self;
}
