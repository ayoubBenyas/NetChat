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
 * Free Client structure memorie
*/
void free_client(Client *clt);

/**
 * Clone a client structure
*/
// Client * Client_clone( Client clt1);

#endif //PROTO