
typedef struct Server_t Server_t;
typedef void*(*CallbackFunc_t)(void* _data, void* _context);

/*TODO several callbacks here*/
Server_t* ServerCreate(CallbackFunc_t _callback, const char* _configFile);
void ServerDestroy(Server_t* _server);
void ServerRun(Server_t* _server);
/*FIXME*/
void SendToCurrentClient(Server_t* _server, void* _data, size_t _dataSize);
//int GetCurrentSocket(Server_t* _server);
