#ifndef PROTO
#define PROTO

SOCKET  SOCKET_create();

int     WINSOCK_init();

void    SOCKADD_bind(struct sockaddr_in *serveradd, char IPadd[15] , int port);

void test(char * text);

#endif //PROTO