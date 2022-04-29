#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "SDL2/SDL_net.h"
#include "udpServer.h"

int main(int argc, char **argv)
{
    UDPserver *server = getUDPserver();
    server->init(server);

    while (server->isRunning)
    {
        server->listen(server);
    }

    server->destroy(server);
    return 0;
}