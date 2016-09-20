#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>

#define BUFF_SIZE 1024
#define URL "172.217.23.36"

void perror(const char* _msg)
{
	/*TODO*/
	exit(1);
}

int main()
{
	int socketDesc;
	char buffer[BUFF_SIZE];
	struct sockaddr_in serverAddr;
	socklen_t addr_size;
	
	if((socketDesc = socket(PF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("Couldn't open socket");
	}
	
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(53);
	serverAddr.sin_addr.s_addr = inet_addr(URL);
	
	memset(serverAddr.sin_zero, '\0', sizeof(serverAddr.sin_zero));

	addr_size = sizeof(serverAddr);	
	
	if(connect(socketDesc, (struct sockaddr *) &serverAddr, addr_size) == -1)
	{
		perror("Couldn't establish connection with the server");
	}
	
	recv(socketDesc, buffer, BUFF_SIZE, 0);
	
	printf("Data received: %s",buffer);  
	
	return 0;
}


































