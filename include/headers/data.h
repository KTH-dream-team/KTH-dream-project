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

typedef struct warriorpos
{
    float x;
    float y;
} WarriorPos;

typedef struct blockpos
{
    float x;
    float y; 
    int from;
} BlockPos;

typedef struct bulletpos
{
    float x;
    float y;
    float z;
} BulletPos;


typedef struct udpstruct //! send this using udp
{
    int from;
    WarriorPos warrior;
    BlockPos block;
    BulletPos bullet;
}UDPStruct;
//int a = sizeof(UDPStruct); // check size of struct with this