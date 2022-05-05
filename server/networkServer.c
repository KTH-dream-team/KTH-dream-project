#include <stdio.h>
#include "networkServer.h"
#include "tcpServer.h"
#include "udpServer.h"


struct networkServerInstance
{
    UDPserver * UDP;
    TCPserver * TCP;
};

bool networkServerInnit(void *self)
{
    NetworkServerInstance *instance = ((NetworkServer*)self)->instance;

    if(!instance->UDP->init(instance->UDP))
    {
        printf("Failed to init UDP server\n");
        return false;
    }
    if(!instance->TCP->init(instance->TCP))
    {
        printf("Failed to init TCP server\n");
        return false;
    }
    
    return true;
}

void networkListen(void*self)
{
    NetworkServerInstance *instance = ((NetworkServer*)self)->instance;

    //printf("Listening for UDP and TCP\n");
    instance->UDP->listen(instance->UDP);
    instance->TCP->listen(instance->TCP);
}

void networkDestroy(void *self)
{
    NetworkServerInstance *instance = ((NetworkServer*)self)->instance;

    //printf("Listening for UDP and TCP\n");
    instance->UDP->destroy(instance->UDP);
    instance->TCP->destroy(instance->TCP);
}

NetworkServer * getNetworkServer()
{
    static NetworkServer self;
    if(self.instance != NULL)
        return &self;

    self.instance = malloc(sizeof(NetworkServerInstance));
    self.instance->UDP = getUDPserver();
    self.instance->TCP = getTCPserver();

    self.init = networkServerInnit;
    self.listen = networkListen;

    return &self;
}
