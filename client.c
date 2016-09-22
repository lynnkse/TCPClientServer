#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include  <fcntl.h>
#include "../generic_hash/HashMap.h"

#define BUFF_SIZE 1024
#define IP "127.0.0.1"
#define ERROR 1
#define PORT 1027
#define NUM_OF_CLIENTS 1000

void perror(const char* _msg)
{
	/*TODO*/
	exit(ERROR);
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
			perror("Couldn't open socket");
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
			sprintf(string, "Client %d send a message\n", i);			
			write(socketDesc[i], string, sizeof(string));
		}
	}
	
	for(i = 0; i < NUM_OF_CLIENTS; ++i)
	{
		close(socketDesc[i]);
	}
	
	
	/*read(socketDesc, buffer, BUFF_SIZE);
	
	printf("Data received by client: %s",buffer);  */
	
	return 0;
}
