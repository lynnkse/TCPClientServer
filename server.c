#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>

#define PORT 1026
#define URL "127.0.0.1"
#define BUFF_SIZE 1025
#define ERROR 1
#define NUM_OF_CLIENTS 3
#define TIMEOUT 5

void perror(const char* _msg)
{
	printf("Error: %s\n", _msg);
	exit(ERROR);
}

int main() 
{
	int socketDesc;
	int newSocket[3];
	fd_set rfds;
	char buffer[BUFF_SIZE];
	struct sockaddr_in serverAddr;
	socklen_t addr_size;
	struct sockaddr_storage serverStorage;
	int i;
	int currSoc;
	
	if((socketDesc = socket(PF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("Couldn't open socket");
	}
	
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr(URL);
	
	memset(serverAddr.sin_zero, '\0', sizeof(serverAddr.sin_zero));

	if(bind(socketDesc, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) == -1)
	{
		perror("Couldn't bind\n");
	}
	
	FD_ZERO(&rfds);
	FD_SET(socketDesc, &rfds);	
	
	while(1)
	{
		
	}
	
	
	
	while((currSoc = select(3, &rfds, NULL, NULL, NULL)) != -1)
	{
		read(currSoc, buffer, BUFF_SIZE);
		printf("Server recieved message: %s\n", buffer);
	}
	
	close(newSocket);

	return 0;
}


















