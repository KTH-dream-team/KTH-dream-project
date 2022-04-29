#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "SDL2/SDL_net.h"
#include "client.h"
#include "time.h"
#include "data.h"




int main(int argc, char **argv)
{
    UDPclient *client = getUDPclient();
    client->init(client);
    srand(time(NULL));
    int r = rand()%20;
    client->makeHandShake(client);
    DataClient data = {r,r*2, 1};

    SDL_Delay(1000);
    client->broadCast(client, &data, sizeof(DataClient)+1);

    while (client->isRunning)
    {
        client->listen(client);
    }

    client->destroy(client);

    return 0;
}