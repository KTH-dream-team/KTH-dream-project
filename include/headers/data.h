#pragma once

#define MAX_CLIENTS 4
#define MAX_SIZE 512
#define CLIENT_PORT 0
#define CLIENT_IP "127.0.0.1"
#define TCP_SERVER_PORT 1000
#define UDP_SERVER_PORT 7000
#define SERVER_IP "127.0.0.1"

typedef struct dataclient
{
    float x;
    float y;
    int from;
} DataClient;

typedef struct datapos
{
    float x;
    float y;
    int from;
} DataPos; // 12 bytes


typedef struct blockpos
{
    float x;
    float y;
    int from;
    int z;
} BlockPos;

