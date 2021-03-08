#ifndef H_TYPE
#define H_TYPE

#include "./../include/lib.h"

#ifndef H_CLIENT
#define H_CLIENT

typedef struct {
    int index;
    char nickName[20];
	SOCKET sockID;
	struct sockaddr_in addr;
	int quit;
}Client;

Client * Client_clone( Client clt1);
void Client_print(Client clt);
int Client_send(Client clt,  char *message);
int Client_recv(Client clt,  char *message);
void Client_quit(Client* clt);

#endif //H_CLIENT

#endif //H_TYPE