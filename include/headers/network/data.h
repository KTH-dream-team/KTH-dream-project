#pragma once

typedef struct // datatype: 1
{
    int totalClient;
    int myId;
} Connection;

typedef struct // datatype: 2
{
    int from;
    int x;
    int y;
} WarriorCreation;

typedef struct //datatype: 3
{
    int from;
    int x;
    int y;
    int health;

} WarriorSnapshot;

typedef struct // datatype: 4
{
    int from;
    int intId;
    float x;
    float y;
    float velX;
    float velY;
} ShootBullet;

typedef struct // datatype:5
{
    int from;
    int x; //testa float;
    int y;
} BlockDestroy;

typedef struct // datatype:6
{
    int from;
    int x; //testa float;
    int y;
    int blockType;
} BlockBuild;

typedef struct // datatype:7
{
    int from;
    int intId;
} DestroyBullet;

typedef struct { //datatype:8
    int from;
    int id;
    int healthDecline;
} GotShot;