

CFLAGS = -g -c #-ansi -pedantic -Wall -Werror -g -c
CC = gcc

SRV_OBJS = server.o
CLT_OBJS = client.o

server : server.o ../generic_hash/HashMap.h 
	$(CC) $(SRV_OBJS) -o server -L../DynamicDSLib -lds -lm
	
server.o : server.c server.h 
	$(CC) $(CFLAGS) server.c 

client : client.o ../generic_hash/HashMap.h 
	$(CC) $(CLT_OBJS) -o client -L../DynamicDSLib -lds -lm
	
client.o : client.c 
	$(CC) $(CFLAGS) client.c 
