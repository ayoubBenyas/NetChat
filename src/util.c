#include "./../include/lib.h"
#include "./../include/type.h"
#include "./../include/const.h"

/**
 * Create new SOCKET
*/
SOCKET   SOCKET_create(){
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    if(clientSocket == INVALID_SOCKET){
        printf("Can't create client socket, exit !\n");
        exit(2);
    }
    return clientSocket;
}

/**
 * Initilize Windows SOCKET librarie
*/
int     WINSOCK_init(){
    WSADATA wsData;
    WORD version = MAKEWORD(2, 2);

    int wsok = WSAStartup(version, &wsData);

    if(wsok != 0){
        printf("Can't initialize winsock, exit !\n");
        exit(1);
    } 
    return wsok;
}

/**
 * Set sockeraddr_in structure
*/
void SOCKADDR_set(struct sockaddr_in *serveradd, char *IPadd, int port){
    serveradd->sin_family = AF_INET;
    serveradd->sin_port = htons( port); // host to network short
    serveradd->sin_addr.S_un.S_addr = inet_addr(IPadd);    
}

/**
 * Free the client structure
*/
void free_client(Client *clt){
    clt->index = -1;
    memset(clt->nickName, 0, LENGTH_NAME);
    closesocket(clt->sockID);
    memset(clt->addr.sin_zero, 0, sizeof(clt->addr.sin_zero));
}

/**
 * Clone Client structure
*/
// Client * Client_clone( Client clt){
//     Client *ptr = (Client*) malloc(sizeof(Client));
//     if( ptr != NULL ){
//         ptr->index = -1;
//         strcpy(ptr->nickName, clt.nickName);
//         ptr->sockID = clt.sockID;
//         //ptr->addr = ;
//         ptr->quit = FALSE;
//     }
//     return ptr;
// }