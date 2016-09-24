#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include "server.h"
#include "../GenericHashMap/HashMap.h"
#include "../logger/logmngr.h"


#define IP "127.0.0.1"
#define PORT_NUM 1032
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
};

static void SaveNewClientSocket(Server_t* _server)
{
	
	socklen_t len;
	struct sockaddr_storage addr;
	char ipstr[INET6_ADDRSTRLEN];
	int port;

	int* val = (int*) malloc(sizeof(int));
	char* key = (char*) malloc(KEY_LEN * sizeof(char)); 

	len = sizeof(addr);
	getpeername(_server->m_currSocket, (struct sockaddr*)&addr, &len);

	if(addr.ss_family == AF_INET)
	{
		struct sockaddr_in *s = (struct sockaddr_in *) &addr;
		port = ntohs(s->sin_port);
		inet_ntop(AF_INET, &s->sin_addr, ipstr, sizeof(ipstr));
	}
	else
	{
		struct sockaddr_in6 *s = (struct sockaddr_in6 *) &addr;
		port = ntohs(s->sin6_port);
		inet_ntop(AF_INET6, &s->sin6_addr, ipstr, sizeof(ipstr));
	}

	sprintf(key, "%s%d%s%d", ipstr, port, _server->m_IP, _server->m_port);
	
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
	Zlog* zlog = ZlogGet("error");
	
	server = (Server_t*) malloc(sizeof(Server_t));
	
	server->m_clientSockets = HashMap_Create(HASHMAP_CAP, (HashFunction) HashFunc, (EqualityFunction) KeyEq);
	
	server->m_port = _portNum;
	server->m_IP = _IP;
	server->m_currSocket = 0;
	
	if((server->m_serverSocket = socket(PF_INET, SOCK_STREAM, 0)) == -1)
	{
		ZLOG_SEND(zlog, LOG_ERROR, "couldn't open socket %d", 1);
		exit(-1);
	}
	
	server->m_maxSocket = server->m_serverSocket;
	
	server->m_serverAddr.sin_family = AF_INET;
	server->m_serverAddr.sin_port = htons(_portNum);
	server->m_serverAddr.sin_addr.s_addr = inet_addr(_IP);
	
	memset(server->m_serverAddr.sin_zero, 0, sizeof(server->m_serverAddr.sin_zero));
	
	if(bind(server->m_serverSocket, (struct sockaddr *) &server->m_serverAddr, sizeof(server->m_serverAddr)) == -1)
	{
		ZLOG_SEND(zlog, LOG_ERROR, "couldn't open socket %d", 1);
		exit(-1);
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

static void NewClientConnection(Server_t* _server)
{
	socklen_t addrSize = sizeof(struct sockaddr);	

	_server->m_currSocket = accept(_server->m_serverSocket, (struct sockaddr*) &_server->m_serverAddr, &addrSize);
	SaveNewClientSocket(_server);
	if(_server->m_currSocket > _server->m_maxSocket)
	{
		_server->m_maxSocket = _server->m_currSocket;
	}
}

static void DeleteConnection(Server_t* _server)
{
	void* valToDestroy;	

	Zlog* zlog = ZlogGet("debug");

	close(_server->m_currSocket);
	HashMap_Remove(_server->m_clientSockets, _server->m_currSocketKey, &valToDestroy);
	ValDestraction(valToDestroy);
	ZLOG_SEND(zlog, LOG_TRACE, "Socket deleted %d", 1);
}

static void ServerIteration(Server_t* _server)
{
	int result = 0;
	int readBytesNum;
	char buffer[BUFF_SIZE];	

	Zlog* zlog = ZlogGet("error");
	Zlog* zlogTrace = ZlogGet("trace");

	FD_ZERO(&_server->m_rfds);
	FD_SET(_server->m_serverSocket, &_server->m_rfds);
	
	HashMap_ForEach(_server->m_clientSockets, (KeyValueActionFunction) FdSetFunc, _server);
	
	if(select(_server->m_maxSocket + 1, &_server->m_rfds, NULL, NULL, NULL) == -1)
	{
		ZLOG_SEND(zlog, LOG_ERROR, "select() failed %d", 1);
	}
	
	if(FD_ISSET(_server->m_serverSocket, &_server->m_rfds) == 1)
	{
		NewClientConnection(_server);				
	}
	else
	{
		HashMap_ForEach(_server->m_clientSockets, (KeyValueActionFunction) FdIsSetFunc, _server);
		readBytesNum = read(_server->m_currSocket, buffer, BUFF_SIZE);
		
		if(readBytesNum == 0)
		{
			DeleteConnection(_server);					
		}
		else if(readBytesNum > 0)
		{
			ZLOG_SEND(zlogTrace, LOG_TRACE, "Server recieved message: %s\nFrom socket %d", buffer, _server->m_currSocket);			
			/*printf("Server recieved message: %s\nFrom socket %d\n", buffer, _server->m_currSocket);*/
			/*write(_server->m_currSocket, buffer, strlen(buffer) + 1);*/
		}
		else
		{
			ZLOG_SEND(zlog, LOG_ERROR, "couldn't read from socket %d", 1);
		}
	}
}

void ServerRun(Server_t* _server)
{	
	listen(_server->m_serverSocket, NUM_OF_CONNECTIONS_WAITING);
	
	while(1)/*FIXME*/
	{
		ServerIteration(_server);
	}
}

int main() 
{	
	Server_t* server;

	ZlogInit("log_config");
	
	server = ServerCreate(PORT_NUM, IP);
	ServerRun(server);
	ServerDestroy(server);

	LogManagerDestroy();

	return 0;
}


















