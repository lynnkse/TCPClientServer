#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>

#define PORT 1025
#define URL "127.0.0.1"
#define BUFF_SIZE 1025
#define ERROR 1

void perror(const char* _msg)
{
	/*TODO*/
	printf("Error: %s\n", _msg);
	exit(ERROR);
}

int main()
{
	int socketDesc, newSocket;
	char buffer[BUFF_SIZE];
	struct sockaddr_in serverAddr;
	socklen_t addr_size;
	struct sockaddr_storage serverStorage;
	
	if((socketDesc = socket(PF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("Couldn't open socket");
	}
	
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr(URL);
	
	memset(serverAddr.sin_zero, '\0', sizeof(serverAddr.sin_zero));

	bind(socketDesc, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
	
	listen(socketDesc, 1);
	
	addr_size = sizeof(serverStorage);
  	newSocket = accept(socketDesc, (struct sockaddr *) &serverStorage, &addr_size);

	strcpy(buffer,"Hello World\n");
    write(newSocket,buffer,13);

	return 0;
}

























