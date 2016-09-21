#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include "server.h"
#include "../generic_hash/HashMap.h"

#define PORT 1026
#define URL "127.0.0.1"
#define BUFF_SIZE 1025
#define ERROR 1
#define NUM_OF_CLIENTS 3
#define TIMEOUT 5
#define HASHMAP_CAP 1000

struct Server_t
{
	int m_serverSocket;
	HashMap* m_clientSockets;
	fd_set m_rfds;
	struct sockaddr_in m_serverAddr;
	/*socklen_t m_addr_size;*/
	/*struct sockaddr_storage serverStorage;*/
};

static int HashFunc(char *str)
{
	int hash = 5381;
	int c;

	while (c = *(str++))
	hash = ((hash << 5) + hash) + c; 

	return hash;
}

static int KeyEq(char* _str1, char* _str2)
{
	return (strcmp(_str1, _str2) == 0);
}

Server_t* ServerCreate(int _portNum)
{
	Server_t* server;
	
	server = (Server_t*) malloc(sizeof(Server_t));
	
	server->m_clientSockets = HashMap_Create(HASHMAP_CAP, (HashFunction) HashFunc, (EqualityFunction) KeyEq);
	
	if((server->m_serverSocket = socket(PF_INET, SOCK_STREAM, 0)) == -1)
	{
		/*TODO error here*/
	}
	
	server->m_serverAddr.sin_family = AF_INET;
	server->m_serverAddr.sin_port = htons(PORT);
	server->m_serverAddr.sin_addr.s_addr = inet_addr(URL);
	
	memset(server->m_serverAddr.sin_zero, 0, sizeof(server->m_serverAddr.sin_zero));
	
	if(bind(server->m_serverSocket, (struct sockaddr *) &server->m_serverAddr, sizeof(server->m_serverAddr)) == -1)
	{
		/*FIXME*/
	}
	
	FD_ZERO(&server->m_rfds);
	FD_SET(server->m_serverSocket, &server->m_rfds);
	
	return server;	
}

void ServerDestroy(Server_t* _server)
{
	close(_server->m_serverSocket);
	free(_server);
}

int main() 
{

	return 0;
}


















