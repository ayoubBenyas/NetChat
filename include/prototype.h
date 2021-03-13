#ifndef H_PROTOTYPE
#define H_PROTOTYPE

SOCKET  SOCKET_create();

int     WINSOCK_init();

void    SOCKADDR_set(struct sockaddr_in *serveradd, char * IPadd , int port);

void broadcast(fd_set * master, char * msg, SOCKET sender, SOCKET server);

#endif