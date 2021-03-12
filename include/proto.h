#ifndef PROTO
#define PROTO

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

#endif //PROTO