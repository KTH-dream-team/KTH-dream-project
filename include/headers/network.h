#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "SDL2/SDL_net.h"
#include "tcpClient.h"
#include "tcpServer.h"
#include "udpClient.h"
#include "udpServer.h"
//#include"data.h"

#define MAX_CLIENTS 4
#define MAX_SIZE 512
#define CLIENT_PORT 0
#define CLIENT_IP "127.0.0.1"
#define SERVER_PORT 3000
#define SERVER_IP "127.0.0.1"

//!TCP SERVER

typedef struct network
{
    TCPserver tcpS;
    TCPclient tcpC;
    UDPserver udpS;
    UDPclient udpC;
}Network;
