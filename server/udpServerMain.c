#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "SDL2/SDL_net.h"
#include "udpServer.h"

int main(int argc, char **argv)
{
    UDPserver *udpserver = getUDPserver();
    udpserver->init(udpserver);

    while (udpserver->isRunning)
    {
        udpserver->listen(udpserver);
    }

    udpserver->destroy(udpserver);
    return 0;
}