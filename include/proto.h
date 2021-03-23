#ifndef PROTO
#define PROTO

#include "./../include/global.h"
#include "./../include/type.h"

/**
 * Create new SOCKET
*/
SOCKET  SOCKET_create();

/**
 * Initialize Windows SOCKET librarie
*/
int     WINSOCK_init();

/**
 * set sockeraddr_in structure
*/
void    SOCKADDR_set(struct sockaddr_in *serveradd, char *IPadd, int port);

/**
 * Create Mutex
*/
HANDLE mutexCreate();

/**
 * Free Client structure memorie
*/
void free_client(Client *clt);

/**
 * Trim array from stating index
*/
void trim_array_from(int index, Client *clt, int *arrayLength);


/*
*   util server
*/
DWORD WINAPI broadcast(void * param);
DWORD WINAPI client_handler(void * indexV);

/*
*   util client
*/
DWORD WINAPI recv_msg_handler(void* sockIdV);
DWORD WINAPI send_msg_handler(void* sockIdV);



#endif //PROTO