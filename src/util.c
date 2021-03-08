#include "./../include/lib.h"
#include "./../include/type.h"
#include "./../include/const.h"

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

void test(char * text){
    //printf("#test : %s ?>\n",text);//getchar();
}

void Client_print(Client clt){
    /*printf("Name \t: \"%s\"\n",clt.nickName);
    printf("Host \t: @(%s|%d)\n",inet_ntoa(clt.addr.sin_addr), ntohs(clt.addr.sin_port));
    printf("Socket\t: ->%d\n",clt.sockID);
    printf("Index \t: [%d]\n",clt.index);*/
    printf("[%d] \"%s\" ->%d    @(%s|%d)",clt.index, clt.nickName, clt.sockID, inet_ntoa(clt.addr.sin_addr), ntohs(clt.addr.sin_port));
}



Client * Client_clone( Client clt){
    Client *ptr = NULL;
    ptr = malloc(sizeof(Client*));
    if( ptr != NULL ){
        ptr->index = -1;
        strcpy(ptr->nickName, clt.nickName);
        ptr->sockID = clt.sockID;
        //ptr->addr = ;
        ptr->quit = FALSE;
    }
    return ptr;
}

int  Client_send(Client clt,  char *message){
    return send(clt.sockID, message, LENGTH_MSG, 0);
}

int  Client_recv(Client clt, char *message){
    return  recv(clt.sockID, message, LENGTH_SEND, 0);   
}

void Client_quit(Client* clt){
    send(clt->sockID, "left", LENGTH_MSG, 0);
    clt->quit = TRUE;
}