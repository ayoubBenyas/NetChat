#include "./../include/lib.h"
#include "./../include/const.h"
#include "./../include/proto.h"
#include "./../include/type.h"

int clientCount = 0;

Client  listClients[MAXCONN];
HANDLE  hThreadArray[MAXCONN];
DWORD   dwThreadIdArray[MAXCONN];


void send_to_all_clients(int sender_index, char send_buffer[]) {
    puts(send_buffer);
    for(int i = 0 ; i< clientCount; i++){
        if (sender_index != i) { // all clients except itself.
            printf("to -> %s (%d)\n",listClients[i].nickName, listClients[i].sockID);
            send(listClients[i].sockID, send_buffer, LENGTH_SEND, 0);
        }
    }
}

DWORD WINAPI client_handler(void * indexV){
    //Client* client = (Client*) clientVoid;
    int index = *(int*) indexV;

    char nickname[LENGTH_NAME] = {};
    char recv_buffer[LENGTH_MSG] = {};
    char send_buffer[LENGTH_SEND] = {};
    
    recv(listClients[index].sockID, nickname, LENGTH_NAME, 0);
    
    strcpy(listClients[index].nickName, nickname);
    
    sprintf(send_buffer, "\"%s\" joined the chatroom.",nickname);
    send_to_all_clients(index, send_buffer);

    while (1) {
        int receive = recv(listClients[index].sockID, recv_buffer, LENGTH_MSG, 0);
        if (receive > 0) {
            if (strlen(recv_buffer) == 0) {
                continue;
            }
            sprintf(send_buffer, "%s : %s.", listClients[index].nickName, recv_buffer);
        } else {
           break;
        }
        send_to_all_clients(index, send_buffer);
    }
}

int main(int argc, char * argv[]){

    // Initialize winsock
    int wsok = WINSOCK_init();

    // Create socket
    SOCKET serverSocket = SOCKET_create();

    // Bind the socket to an ip address and port
    int portNumber = DEFAULT_PORT;
    char ipAddress[15] = DEFAULT_HOST;
    struct sockaddr_in serverAdd;
    
    if(argc == 3){
        portNumber = atoi(argv[2]);
        strcpy(ipAddress, argv[1]);
    }

    SOCKADD_bind(&serverAdd, ipAddress, portNumber);
    
    if ( bind(serverSocket, (struct sockaddr*) &serverAdd , sizeof(serverAdd)) == -1 )  return -1;

    // Listening, one connection is allowed 
    if ( listen(serverSocket, 2) == -1 ) return -2;// MAXCONN 

    printf("Server [%s] listening on port %d .....\n",ipAddress, portNumber);
    
    while(1){
        printf(".");
        listClients[clientCount].sockID = accept(serverSocket, (struct sockaddr*) &listClients[clientCount].addr, NULL);
        
        if( listClients[clientCount].sockID != -1 ){
            listClients[clientCount].index = clientCount;
            hThreadArray[clientCount] = CreateThread( NULL, 1, (void *)client_handler, &(listClients[clientCount].index), 0, &dwThreadIdArray[clientCount]);
            clientCount ++;
        }
        printf("-");
    }

    for(int i = 0 ; i < clientCount ; i ++)
        CloseHandle(hThreadArray[i]);
    
    getchar();
    closesocket(serverSocket);
    
    return 0;
}