#ifndef H_network
#define H_network
#define SELFCONNECT "127.0.0.1"
#include <SDL2/SDL_net.h>

typedef struct serverData ServerData;
typedef struct tCPServerInstance TCPServerInstance;


int checkTCPAccept(void*self);//!Hjälp funktion

typedef struct TCPServer
{
	void (*initNetwork)(void*self);
	void (*TCPAccept)(void*self);
	void (*receiveData)(void*self);
	void (*sendData)(void*self);
	void (*createSocketServer)(void*self);
	void (*destroy)(void *self);
	void (*setInstance)(void*self);

	int serverRunning;
	TCPServerInstance *instance;
} TCPServer;

TCPServer *getTCPServer();

#endif