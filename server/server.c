#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "SDL2/SDL_net.h"
#include "network.h"

int main(int argc, char **argv)
{

    TCPserver *tcpserver = getTCPserver();
	UDPserver *udpserver = getUDPserver();
    bool r = tcpserver->init(tcpserver);
	udpserver->init(udpserver);

    if(r) printf("ok\n");

    int running = 1;
    while(running)
    {
        tcpserver->listenConnection(tcpserver);
		udpserver->listen(udpserver);
        tcpserver->instance->nrOfRdy = SDLNet_CheckSockets(tcpserver->instance->socketSet, 1000);
        if(tcpserver->instance->nrOfRdy <= 0) {
            // NOTE: none of the sockets are ready
            printf("Waiting for data from clients\n");
        } else {
            // NOTE: some number of the sockets are ready
            printf("Data from clients are ready\n");
            tcpserver->loopClients(tcpserver);
        }
    }
	tcpserver->destroy(tcpserver);
	udpserver->destroy(udpserver);
    return 0;
	printf("HELLO\n");
}