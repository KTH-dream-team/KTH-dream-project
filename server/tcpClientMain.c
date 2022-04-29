#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "SDL2/SDL_net.h"
#include "../include/headers/tcpClient.h"
#include <time.h>


int main(int argc, char **argv)
{
    srand(time(NULL));
    TCPclient *client = getTCPclient();



    if(client->init(client)) printf("innit successfully!\n");
    setRand(client);

    int choice=0;
    while(client->instance->packetSent->x!=0)
    {
        client->listen(client);
        if(client->instance->nrOfSocketRdy<=0)
        {
            printf("Waiting for data from server\n");
            printf("(1) set data \n(2) Broadcast data\n(3) Listen to server\n");
            scanf("%d%*c", &choice);
            if(choice==1){
                printf("set x: ");
                scanf("%d%*c", &client->instance->packetSent->x);
                printf("set y: ");
                scanf("%d%*c", &client->instance->packetSent->y);
                printf("set from: ");
                scanf("%d%*c", &client->instance->packetSent->from);
            }
            else if(choice==2){
                int a = client->broadCast(client, client->instance->packetSent, sizeof(TCPClientData));
            }
            else if(choice==3){
                continue;
            }
        }
        else
        {
            printf("Data from server ready\n");
            printf("Nummer of ready: %d\n", client->instance->nrOfSocketRdy);
            if(SDLNet_SocketReady(client->instance->clientSocketSet))
            {
                printf("Inside socketReady() \n");
                client->recive(client);
                client->instance->nrOfSocketRdy--;
            }
        }

    }
    client->destroy(client);

    return 0;
}