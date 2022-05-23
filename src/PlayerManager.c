#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "data.h"
#include "define.h"
#include "PlayerManager.h"
#include "networkClient.h"

typedef struct player
{
	int id;
	bool alive;
	char name[PLAYERNAME];
}Player;

struct playerManagerInstance
{
	Player players[MAXPLAYER];
	int nrOfPlayers;
	int nrOfPlayerAlive;
};

void initPlayerManager(void *self)
{
	printf("init\n");
	PlayerManager *PM = getPlayerManager();
	NetworkClient *network = getNetworkClient();
	PM->instance->nrOfPlayers = network->getNrOfClientTCP(network);
	PM->instance->nrOfPlayerAlive = network->getNrOfClientTCP(network);

	for(int i=0;i<PM->instance->nrOfPlayers;i++){
		PM->instance->players[i].alive = true;
		PM->instance->players[i].id = i;
		printf("playerID: %d, alive: %d\n", PM->instance->players[i].id, PM->instance->players[i].alive);
	}
	return;
}

int winnerPlayerManager(void *self)
{	
	//printf("winner\n");
	PlayerManager *PM = getPlayerManager();
	if(PM->instance->nrOfPlayerAlive>1)
	{
		//printf("More than 1 player alive!\n");
		return -1;
	}
	else if(PM->instance->nrOfPlayerAlive==0)
	{
		//printf("Draw 0 players alive!\n");
		return -2;
	}

	for(int i=0;i<PM->instance->nrOfPlayers;i++)
	{
		if(PM->instance->players[i].alive==true)
		{
			//printf("Winner is: %d!!!\n", PM->instance->players[i].id);
			return PM->instance->players[i].id;
		}
	}
	return -1;
}

void killedPlayerManager(void *self, int id)
{
	PlayerManager *PM = getPlayerManager();
	PM->instance->players[id].alive=false;
	PM->instance->nrOfPlayerAlive--;
	printf("NrOfPlayers: %d, NrOfPlayersAlive: %d, killed playerID: %d, playerStatus: %d\n", PM->instance->nrOfPlayers, PM->instance->nrOfPlayerAlive, PM->instance->players[id].id, PM->instance->players[id].alive);
}

PlayerManager *getPlayerManager()
{
	static PlayerManager self;
	if(self.instance!=NULL) 
		return &self;

	self.instance = malloc(sizeof(PlayerManagerInstance));

	self.init = initPlayerManager;
	self.winner = winnerPlayerManager;
	self.killed = killedPlayerManager;

	return &self;
}