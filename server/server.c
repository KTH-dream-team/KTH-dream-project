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

    tcpserver->init(tcpserver);
	udpserver->init(udpserver);
    printf("Hello\n");
    int running = 1;
    while(running)
    {
        tcpserver->TCPlisten(tcpserver);
		udpserver->listen(udpserver);
    }
	tcpserver->destroy(tcpserver);
	udpserver->destroy(udpserver);
    return 0;
	printf("Bye\n");
}