#ifndef H_TYPE
#define H_TYPE

#include "./../include/lib.h"

#ifndef H_CLIENT
#define H_CLIENT

typedef struct {
    char nickName[20];
    int index;
	SOCKET sockID;
	struct sockaddr_in addr;
	int len;
}Client;

void Client_print(Client clt);

#endif //H_CLIENT

#endif //H_TYPE