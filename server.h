
typedef struct Server_t Server_t;

Server_t* ServerCreate(int _portNum, const char* _IP);
void ServerDestroy(Server_t* _server);
void ServerRun(Server_t* _server);
