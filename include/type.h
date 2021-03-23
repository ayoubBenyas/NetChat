#ifndef H_TYPE
#define H_TYPE

#include "./../include/lib.h"
#include "./../include/const.h"


// Client stucture
typedef struct {
    int index; // client identifier on the server
    char nickName[20];
	SOCKET sockID; // client socket
	struct sockaddr_in addr;
	int quit;
}Client;



typedef struct{
    int sender_index;
    char msg[LENGTH_SEND];
}BroadParams;



#endif //H_TYPE