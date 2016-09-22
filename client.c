#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <string.h>
#include <stdlib.h>
#include  <fcntl.h>
#include "../generic_hash/HashMap.h"
#include "client.h"

#define BUFF_SIZE 1024
#define IP "127.0.0.1"
#define ERROR 1
#define PORT 1030
#define NUM_OF_CLIENTS 30
#define CLIENT_NOT_CONNECTED 0

struct Clients_t
{
	int* m_socketDescriptors;
	socklen_t addr_size;
	struct sockaddr_in serverAddr;
};

static void perror(const char* _msg)
{
	/*TODO*/
	exit(ERROR);
}

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

Clients_t* ClientsCreate(size_t _numOfClients, int _portNum, const char* _IP)
{
	Clients_t* clients;
	
	clients = (int*) calloc(_numOfClients, sizeof(int));
	if(NULL == clients)
	{
		return NULL;
	}
	
	clients->serverAddr.sin_family = AF_INET;
	clients->serverAddr.sin_port = htons(_portNum);
	clients->serverAddr.sin_addr.s_addr = inet_addr(_IP);
	
	memset(clients->serverAddr.sin_zero, '\0', sizeof(clients->serverAddr.sin_zero));
	
	clients->addr_size = sizeof(serverAddr);	
	
}

int main()
{
	int socketDesc[NUM_OF_CLIENTS];
	char buffer[BUFF_SIZE];
	struct sockaddr_in serverAddr;
	socklen_t addr_size;
	char string[1024];
	int i, j;

	for(i = 0; i < NUM_OF_CLIENTS; ++i)
	{
		if((socketDesc[i] = socket(PF_INET, SOCK_STREAM, 0)) == -1)
		{
			perror("Couldn't open socket\n");
		}
	}
	
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr(IP);
	
	memset(serverAddr.sin_zero, '\0', sizeof(serverAddr.sin_zero));

	addr_size = sizeof(serverAddr);	
	
	for(i = 0; i < NUM_OF_CLIENTS; ++i)
	{
		if(connect(socketDesc[i], (struct sockaddr *) &serverAddr, addr_size) == -1)
		{
			perror("Couldn't establish connection with the server");
		}
		/*printf("Socket: %d\n", socketDesc[i]);*/
	}
	
	for(j = 0; j < 10; ++j)
	{
		for(i = 0; i < NUM_OF_CLIENTS; ++i)
		{
			sprintf(string, "Client message");			
			write(socketDesc[i], string, strlen(string) + 1);
		}
	}

	
	for(i = 0; i < NUM_OF_CLIENTS; ++i)
	{
		FinClose(socketDesc[i]);
	}
	
	
	/*read(socketDesc, buffer, BUFF_SIZE);
	
	printf("Data received by client: %s",buffer);  */
	
	return 0;
}
