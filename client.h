#include <stddef.h>

typedef struct Clients_t Clients_t;

Clients_t* ClientsCreate(size_t _numOfClients, int _portNum, const char* _IP);
void ClientsDestroy(Clients_t* _clients);
void ClientsRun(Clients_t* _clients); 
