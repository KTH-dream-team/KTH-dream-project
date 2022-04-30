#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "SDL2/SDL_net.h"
#include "networkClient.h"
#include "../include/headers/data.h"

int main(int argc, char **argv)
{
    DataClient dataToSentTCP = {1,2,3};
    DataClient dataToSentUDP = {111,222,333};

    NetworkClient * network = getNetworkClient();
    if(network->init(network))
        printf("innit successfully\n");

    while(1)
    {
        network->TCPbroadCast(network, &dataToSentTCP, sizeof(DataClient));
        network->UDPbroadCast(network, &dataToSentUDP, sizeof(DataClient));

        network->listen(network);

        SDL_Delay(500);
    }

    return 0;
}