#include <stddef.h>

typedef struct Clients_t Clients_t;

Clients_t* ClientsCreate(int _portNum, const char* _IP, const char* _configFile);
void ClientsDestroy(Clients_t* _clients);
void ClientsRun(Clients_t* _clients); 
