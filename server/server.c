#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "SDL2/SDL_net.h"
#include "../include/headers/networkServer.h"

int main(int argc, char **argv)
{
    NetworkServer *server =  getNetworkServer();

    server->init(server);

    printf("Hello I'm Server\n");
    int running = 1;
    while(running)
    {
        server->listen(server);
    }
    server->destroy(server);
	printf("Bye\n");
    return 0;
}