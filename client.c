#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include  <fcntl.h>

#define BUFF_SIZE 1024
#define IP "127.0.0.1"
#define ERROR 1
#define PORT 1025

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
	printf("Socket: %d\n", socketDesc);
	
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr(IP);
	
	memset(serverAddr.sin_zero, '\0', sizeof(serverAddr.sin_zero));

	addr_size = sizeof(serverAddr);	
	
	if(connect(socketDesc, (struct sockaddr *) &serverAddr, addr_size) == -1)
	{
		perror("Couldn't establish connection with the server");
	}
	
	printf("Enter your message...\n");
	scanf("%s", string);
	write(socketDesc, string, sizeof(string));
	
	/*read(socketDesc, buffer, BUFF_SIZE);
	
	printf("Data received by client: %s",buffer);  */
	
	return 0;
}
