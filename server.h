
typedef struct Server_t Server_t;
typedef void*(*CallbackFunc_t)(void* _data, void* _context);

Server_t* ServerCreate(int _portNum, const char* _IP, size_t _buffSize, CallbackFunc_t _callback);
void ServerDestroy(Server_t* _server);
void ServerRun(Server_t* _server);
int GetCurrentSocket(Server_t* _server);
