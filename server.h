
typedef struct Server_t Server_t;
typedef void*(*CallbackFunc_t)(void* _data, void* _context);
typedef struct ServerArguments
{
	CallbackFunc_t m_callbackNew; /*pointer to server will be passed as first argument, NULL as socond*/
	CallbackFunc_t m_callbackExisting;/*received data will be passed af first argument, pointer to server as second*/
	CallbackFunc_t m_callbackDeletion;/*pointer to server will be passed as first argument, NULL as socond*/
	const char* m_configFile;
} ServerArguments;

/*TODO several callbacks here*/
Server_t* ServerCreate(ServerArguments _arguments);
void ServerDestroy(Server_t* _server);
void ServerRun(Server_t* _server);
/*FIXME*/
void SendToCurrentClient(Server_t* _server, void* _data, size_t _dataSize);
//int GetCurrentSocket(Server_t* _server);
