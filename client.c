#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

#define BUFF_SIZE 1024
#define URL "127.0.0.1"
#define ERROR 1
#define PORT 1026

void perror(const char* _msg)
{
	/*TODO*/
	exit(ERROR);
}

int main()
{
	int socketDesc;
	char buffer[BUFF_SIZE];
	struct sockaddr_in serverAddr;
	socklen_t addr_size;
	char string[] = "TheString";
	
	if((socketDesc = socket(PF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("Couldn't open socket");
	}
	
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr(URL);
	
	memset(serverAddr.sin_zero, '\0', sizeof(serverAddr.sin_zero));

	addr_size = sizeof(serverAddr);	
	
	if(connect(socketDesc, (struct sockaddr *) &serverAddr, addr_size) == -1)
	{
		perror("Couldn't establish connection with the server");
	}
	
	while(strcmp(buffer, "exit") != 0)
	{
		printf("Enter message...\n");
		scanf("%s", buffer);
		write(socketDesc, buffer, strlen(buffer) + 1);
	}  
	
	close(socketDesc);
	
	return 0;
}


































