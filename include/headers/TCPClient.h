#ifndef TCPClient_H
#define TCPClient_H
#define SELFCONNECT "127.0.0.1"
#include "SDL2/SDL_net.h"

typedef struct clientData ClientData;
typedef struct tCPClientInstance TCPClientInstance;


typedef struct TCPClient
{
	void (*initClient)();
	void (*connect)(void*self, char* IPAddress, int port);
	void (*receiveData)(void*self);
	void (*createSocketClient)(void*self);
	void (*sendData)(void *self);
	void (*destroy)(void *self);

	TCPClientInstance *instance;
} TCPClient;

TCPClient *getTCPClient();
void setData(void *self,int x,int y);

#endif