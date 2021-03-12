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
 * Create Mutex
*/
HANDLE mutexCreate(){
    return  CreateMutexA(
            NULL,              // security attributes
            FALSE,             // initially not owned
            NULL);             // unnamed mutex
}

/**
 * Free the client structure
*/
void free_client(Client *clt){
    clt->index = -1;
    ZeroMemory(clt->nickName, LENGTH_NAME);
    closesocket(clt->sockID);
    ZeroMemory(clt->addr.sin_zero, sizeof(clt->addr.sin_zero));
}

/**
 * Trim array 
*/
void trim_array_from(int index, Client *array, int * arrayLength){
    for(int i=index; i< *arrayLength -1 ; i++){
        strcpy(array[i].nickName, array[i+1].nickName);
        array[i].addr = array[i+1].addr;
        array[i].sockID = array[i+1].sockID;
        array[i].index = array[i+1].index - 1;
    }
    (*arrayLength)--;
}