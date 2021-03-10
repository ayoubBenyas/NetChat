#include "./../include/lib.h"
#include "./../include/const.h"
#include "./../include/proto.h"
#include "./../include/type.h"
#include "./../include/config.h"

int clientCount = 0;
// connected Clients Array
Client  listClients[MAXCONN];
HANDLE  hThreadArray[MAXCONN];
DWORD   dwThreadIdArray[MAXCONN];


void broadcast(int sender_index, char send_buffer[]) {
    for(int i = 0 ; i< clientCount; i++){
        if (sender_index != i) { // all clients except the messenger.
            printf("to -> %s (%d)\n",listClients[i].nickName, listClients[i].sockID);
            send(listClients[i].sockID, send_buffer, LENGTH_SEND, 0);
        }
    }
}

DWORD WINAPI client_handler(void * indexV){
    int index = *(int*) indexV;

    char recv_buffer[LENGTH_MSG] = {};
    char send_buffer[LENGTH_SEND] = {};
    
    recv(listClients[index].sockID, listClients[index].nickName, LENGTH_NAME, 0);
    
    sprintf(send_buffer, "\"%s\" joined the chatroom.",listClients[index].nickName);
    broadcast(index, send_buffer);
    
    while (1) {
        // Clear buffers
        memset(send_buffer, 0, LENGTH_SEND);
        memset(recv_buffer, 0, LENGTH_MSG);

        int recived = recv(listClients[index].sockID, recv_buffer, LENGTH_MSG, 0);
        if ( recived > 0) {
            if (strlen(recv_buffer) == 0) {
                continue; // skip emty messages
            }
            sprintf(send_buffer, "%s : %s.", listClients[index].nickName, recv_buffer);
        } else{
           sprintf(send_buffer, "\"%s\" just left the chatroom.", listClients[index].nickName);
           broadcast(index, send_buffer);
           clientCount --;
           //free_client(listClients+index);
           break;
        }

        broadcast(index, send_buffer);
    }
}

int main(int argc, char * argv[]){

    // Initialize winsock
    int wsok = WINSOCK_init();

    // Create socket
    SOCKET serverSocket = SOCKET_create();

    // Bind the socket to an ip address and port
    int portNumber = DEFAULT_PORT;
    char *ipAddress = DEFAULT_HOST;
    struct sockaddr_in serverAdd;
    
    if(argc == 3){
        portNumber = atoi(argv[2]);
        ipAddress = (char*) malloc(strlen(argv[1])*sizeof(char));
        strcpy(ipAddress, argv[1]);
    }

    // set sockaddr structure
    SOCKADDR_set(&serverAdd, ipAddress, portNumber);
    
    if ( bind(serverSocket, (struct sockaddr*) &serverAdd , sizeof(serverAdd)) == -1 )  return -1;

    // Listening, on new connections
    if ( listen(serverSocket, MAXCONN) == -1 ) return -2; 

    printf("Server [%s] listening on port %d .....\n",ipAddress, portNumber);
    
    while(1){
        printf(".");
        listClients[clientCount].sockID = accept(serverSocket, (struct sockaddr*) &listClients[clientCount].addr, NULL);
        
        if( listClients[clientCount].sockID != -1 ){
            listClients[clientCount].index = clientCount;
            hThreadArray[clientCount] = CreateThread( NULL, 1, client_handler, (void *) &(listClients[clientCount].index), 0, &dwThreadIdArray[clientCount]);
            if(dwThreadIdArray[clientCount])
                clientCount ++;
        }
        printf("-");
    }

    for(int i = 0 ; i < clientCount ; i ++)
        CloseHandle(hThreadArray[i]);
    
    // Shutdown winsock
    shutdown(serverSocket, SD_BOTH);

    getchar();
    closesocket(serverSocket);
    
    return 0;
}