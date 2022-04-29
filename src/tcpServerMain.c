#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "SDL2/SDL_net.h"
#include "tcpServer.h"

/*

int main(int argc, char **argv)
{
    TCPserver *server = getTCPserver();
    bool r = server->init(server);

    if(r) printf("ok\n");

    int running = 1;
    while(running)
    {
        server->listenConnection(server);
        server->instance->nrOfRdy = SDLNet_CheckSockets(server->instance->socketSet, 1000);
        if(server->instance->nrOfRdy <= 0) {
            // NOTE: none of the sockets are ready
            printf("Waiting for data from clients\n");
        } else {
            // NOTE: some number of the sockets are ready
            printf("Data from clients are ready\n");
            server->loopClients(server);
        }
    }
    return 0;
}*/