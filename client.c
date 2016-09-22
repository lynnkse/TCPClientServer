#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include  <fcntl.h>
#include "../GenericHashMap/HashMap.h "

#define BUFF_SIZE 1024
#define IP "127.0.0.1"
#define ERROR 1
#define PORT 1025
#define NUM_OF_CLIENTS

void perror(const char* _msg)
{
	/*TODO*/
	exit(ERROR);
}

int SimpleHash(int* _num)
{
	return *_num;
}

int KeyEq(int* _num1, int* _num2)
{
	return *_num1 == *_num2;
}

int main()
{
	int socketDesc[NUM_OF_CLIENTS];
	char buffer[BUFF_SIZE];
	struct sockaddr_in serverAddr;
	socklen_t addr_size;
	char string[1024];
	int i;

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
	}
	
	while(1)
	{
		for(i = 0; i < NUM_OF_CLIENTS; ++i)
		{
			sprintf(string, "Client %d send a message\n", i);			
			write(socketDesc, string, sizeof(string));
		}
	}
	
	
	/*read(socketDesc, buffer, BUFF_SIZE);
	
	printf("Data received by client: %s",buffer);  */
	
	return 0;
}
