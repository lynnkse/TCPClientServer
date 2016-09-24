#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <string.h>
#include <stdlib.h>
#include  <fcntl.h>
#include "../GenericHashMap/HashMap.h"
#include "client.h"
#include "../logger/logmngr.h"

#define BUFF_SIZE 1024
#define IP "127.0.0.1"
#define ERROR 1
#define PORT 1032
#define NUM_OF_CLIENTS 30
#define CLIENT_NOT_CONNECTED 0

struct Clients_t
{
	int* m_socketDescriptors;
	socklen_t addr_size;
	struct sockaddr_in serverAddr;
	size_t m_numOfClients;
};

static void ResetClose(int _socketDesc)
{
	int i;
	struct linger so_linger;
	so_linger.l_onoff = 1;
	so_linger.l_linger = 0;
	
	setsockopt(_socketDesc, SOL_SOCKET, SO_LINGER, &so_linger, sizeof(so_linger));
	close(_socketDesc);
}

static void FinClose(int _socketDesc)
{
	close(_socketDesc);
}

static void SilentClose(int _socketDesc)
{
	int num = 1;
	setsockopt(_socketDesc, SOL_SOCKET, TCP_REPAIR, &num, sizeof(int));
}

static void CreateNewConnection(int* _socket, Clients_t* _clients)
{
	if((*_socket = socket(PF_INET, SOCK_STREAM, 0)) == -1)
	{
		/*FIXME fix this!!!*/
	}
	
	if(connect(*_socket, (struct sockaddr *) &_clients->serverAddr, _clients->addr_size) == -1)
	{
		/*FIXME fix this!!!*/
		printf("Couldn't establish connection with the server");
	}
}

static void SendMessageToServer(int _socket)
{
	char buffer[BUFF_SIZE];
	sprintf(buffer, "Client message");			
	write(_socket, buffer, strlen(buffer) + 1);
}

static void DisconnectClient(int* _socket)
{
	int opt = rand() % 3;
	
	switch(opt)
	{
		case 0:
			FinClose(*_socket);
			break;
		case 1:
			ResetClose(*_socket);
			break;
		case 2:
			SilentClose(*_socket);
			break;
	} 
	*_socket = 0;
}

static void RecieveMessage(int _socket)
{
	char buffer[BUFF_SIZE];
	
	read(_socket, buffer, BUFF_SIZE);
	
	printf("Client recieved message: %s\n", buffer);
}

Clients_t* ClientsCreate(size_t _numOfClients, int _portNum, const char* _IP)
{
	Clients_t* clients;
	
	clients = (Clients_t*) malloc(sizeof(Clients_t));
	if(NULL == clients)
	{
		return NULL;
	}
	
	clients->m_socketDescriptors = (int*) calloc(_numOfClients, sizeof(int));
	if(NULL == clients->m_socketDescriptors)
	{
		free(clients);
		return NULL;
	}
	
	clients->serverAddr.sin_family = AF_INET;
	clients->serverAddr.sin_port = htons(_portNum);
	clients->serverAddr.sin_addr.s_addr = inet_addr(_IP);
	
	memset(clients->serverAddr.sin_zero, '\0', sizeof(clients->serverAddr.sin_zero));
	
	clients->addr_size = sizeof(clients->serverAddr);
	clients->m_numOfClients = _numOfClients;	
	
	return clients;
}

void ClientsRun(Clients_t* _clients)
{
	int* socket;
	
	while(1)
	{
		socket = &_clients->m_socketDescriptors[rand() % _clients->m_numOfClients];
	
		printf("%d\n", *socket);
	
		if(*socket == 0)
		{
			CreateNewConnection(socket, _clients);
		}
		else
		{
			if(rand() % 2 == 0)
			{
				SendMessageToServer(*socket);
				/*RecieveMessage(*socket);*/
			}
			else
			{
				DisconnectClient(socket);
			}
		}
	}
}

void ClientsDestroy(Clients_t* _clients)
{
	int sockIdx, numOfSocks;
	
	numOfSocks = _clients->m_numOfClients;
	
	for(sockIdx = 0; sockIdx < numOfSocks; ++sockIdx)
	{
		if(_clients->m_socketDescriptors[sockIdx])
		{
			close(_clients->m_socketDescriptors[sockIdx]);
		}
	}
	
	free(_clients->m_socketDescriptors);
	free(_clients);
}

int main()
{
	Clients_t* clients;
	
	clients = ClientsCreate(100, PORT, IP);
	ClientsRun(clients);
	ClientsDestroy(clients);
	
	return 0;
}















