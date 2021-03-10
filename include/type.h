#ifndef H_TYPE
#define H_TYPE

#include "./../include/lib.h"

#ifndef H_CLIENT
#define H_CLIENT

// Client stucture
typedef struct {
    int index; // client identifier on the server
    char nickName[20];
	SOCKET sockID; // client socket
	struct sockaddr_in addr;
	int quit;
}Client;


#endif //H_CLIENT

#endif //H_TYPE