#include "./../include/lib.h"
#include "./../include/type.h"

SOCKET   SOCKET_create(){
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    if(clientSocket == INVALID_SOCKET){
        printf("Can't create client socket, exit !\n");
        exit(2);
    }
    return clientSocket;
}


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

void SOCKADD_bind(struct sockaddr_in *serveradd, char IPadd[15] , int port){
    serveradd->sin_family = AF_INET;
    serveradd->sin_port = htons( port); // host to network short
    serveradd->sin_addr.S_un.S_addr = inet_addr(IPadd);    
}

void Client_print(Client clt){
    printf("Name \t: \"%s\"\n",clt.nickName);
    printf("Host \t: @(%s|%d)\n",inet_ntoa(clt.addr.sin_addr), ntohs(clt.addr.sin_port));
    printf("Socket\t: ->%d\n",clt.sockID);
    printf("Index \t: [%d]\n",clt.index);
}