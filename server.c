#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include "server.h"
#include "../generic_hash/HashMap.h"


#define IP "127.0.0.1"
#define PORT_NUM 1030
#define BUFF_SIZE 1024
#define ERROR 1
#define NUM_OF_CLIENTS 3
#define TIMEOUT 5
#define HASHMAP_CAP 1000
#define KEY_LEN 128
#define NUM_OF_CONNECTIONS_WAITING 1

struct Server_t
{
	int m_serverSocket;
	HashMap* m_clientSockets;
	fd_set m_rfds;
	struct sockaddr_in m_serverAddr;
	int m_currSocket;
	int m_maxSocket;
	char* m_currSocketKey;
	int m_port;
	char* m_IP;
	/*socklen_t m_addr_size;*/
	/*struct sockaddr_storage serverStorage;*/
};

static void SaveNewClientSocket(Server_t* _server)
{
	int* val = (int*) malloc(sizeof(int));
	char* key = (char*) malloc(KEY_LEN * sizeof(char)); 
	/*FIXME*/
	key[0] = 97 + rand() % 22;
	key[1] = 97 + rand() % 22;
	key[2] = 97 + rand() % 22;
	key[3] = 97 + rand() % 22;
	key[4] = '\0';
	
	*val = _server->m_currSocket;
	
	HashMap_Insert(_server->m_clientSockets, key, val);
}

static int HashFunc(char *str)
{
	int hash = 5381;
	int c;

	while (c = *(str++))
	hash = ((hash << 5) + hash) + c; 

	return hash;
}

static void KeyDestraction(void* _key)
{
	free(_key);
}

static void ValDestraction(void* _val)
{
	free(_val);
}

static int FdSetFunc(char* _key, int* _value, Server_t* _server)
{
	FD_SET(*_value, &_server->m_rfds);

	return 1;
}

static int FdIsSetFunc(char* _key, int* _value, Server_t* _server)
{
	if(FD_ISSET(*_value, &_server->m_rfds) == 0)
	{
		return 1;
	}
	else
	{
		_server->m_currSocket = *_value;
		_server->m_currSocketKey = _key;
		return 0;
	}
	
	return (FD_ISSET(*_value, &_server->m_rfds) == 0 ? 1 : 0);
}

static int KeyEq(char* _str1, char* _str2)
{
	return (strcmp(_str1, _str2) == 0);
}

Server_t* ServerCreate(int _portNum, const char* _IP)
{
	Server_t* server;
	
	server = (Server_t*) malloc(sizeof(Server_t));
	
	server->m_clientSockets = HashMap_Create(HASHMAP_CAP, (HashFunction) HashFunc, (EqualityFunction) KeyEq);
	
	server->m_port = _portNum;
	server->m_IP = _IP;
	server->m_currSocket = 0;
	
	if((server->m_serverSocket = socket(PF_INET, SOCK_STREAM, 0)) == -1)
	{
		/*TODO error here*/
		printf("Error: couldn't open socket\n");
	}
	/*printf("Socket: %d\n", server->m_serverSocket);*/
	
	server->m_maxSocket = server->m_serverSocket;
	
	server->m_serverAddr.sin_family = AF_INET;
	server->m_serverAddr.sin_port = htons(_portNum);
	server->m_serverAddr.sin_addr.s_addr = inet_addr(_IP);/*htonl(INADDR_ANY);*/
	
	memset(server->m_serverAddr.sin_zero, 0, sizeof(server->m_serverAddr.sin_zero));
	
	if(bind(server->m_serverSocket, (struct sockaddr *) &server->m_serverAddr, sizeof(server->m_serverAddr)) == -1)
	{
		printf("Error: couldn't bind\n");
		/*FIXME*/
	}
	
	FD_ZERO(&server->m_rfds);
	FD_SET(server->m_serverSocket, &server->m_rfds);
	
	return server;	
}

void ServerDestroy(Server_t* _server)
{
	close(_server->m_serverSocket);
	HashMap_Destroy(&_server->m_clientSockets, KeyDestraction, ValDestraction);
	free(_server);
}

void ServerRun(Server_t* _server)
{
	int result = 0;
	void* valToDestroy;
	int readBytesNum;
	char buffer[BUFF_SIZE];
	socklen_t addrSize = sizeof(struct sockaddr);
	
	listen(_server->m_serverSocket, NUM_OF_CONNECTIONS_WAITING);
	
	while(1)/*FIXME*/
	{
		FD_ZERO(&_server->m_rfds);
		FD_SET(_server->m_serverSocket, &_server->m_rfds);
		
		HashMap_ForEach(_server->m_clientSockets, (KeyValueActionFunction) FdSetFunc, _server);
		
		result = select(_server->m_maxSocket + 1, &_server->m_rfds, NULL, NULL, NULL);
		
		if(result > 0)
		{
			if(FD_ISSET(_server->m_serverSocket, &_server->m_rfds) == 1)
			{
				_server->m_currSocket = accept(_server->m_serverSocket, (struct sockaddr*) &_server->m_serverAddr, &addrSize);
				SaveNewClientSocket(_server);
				if(_server->m_currSocket > _server->m_maxSocket)
				{
					_server->m_maxSocket = _server->m_currSocket;
				}
			}
			else
			{
				HashMap_ForEach(_server->m_clientSockets, (KeyValueActionFunction) FdIsSetFunc, _server);
				readBytesNum = read(_server->m_currSocket, buffer, BUFF_SIZE);/*FIXME better use recv here*/
				
				if(readBytesNum == 0)
				{
					close(_server->m_currSocket);
					HashMap_Remove(_server->m_clientSockets, _server->m_currSocketKey, &valToDestroy);
					ValDestraction(valToDestroy);
					printf("I destroy socket now %d\n", _server->m_currSocket);
				}
				else if(readBytesNum > 0)
				{
					printf("Server recieved message: %s\nFrom socket %d\n", buffer, _server->m_currSocket);
					/*write(_server->m_currSocket, buffer, strlen(buffer) + 1);*/
				}
				else
				{
					/*FIXME this should't happen, add log or whatever here*/
				}
			}
		}
	}
}

int main() 
{
	Server_t* server;
	
	server = ServerCreate(PORT_NUM, IP);
	ServerRun(server);
	ServerDestroy(server);

	return 0;
}


















