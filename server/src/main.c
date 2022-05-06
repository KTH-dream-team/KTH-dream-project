#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "SDL2/SDL_net.h"
#include "networkServer.h"

int main(int argc, char **argv)
{
    NetworkServer *network = getNetworkServer();
    if (network->init(network))
        printf("innit successfully\n");

    while (1)
    {
        network->listen(network);
    }

    network->destroy(network);

    return 0;
}