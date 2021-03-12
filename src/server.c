#include "./../include/lib.h"
#include "./../include/const.h"
#include "./../include/proto.h"
#include "./../include/type.h"
#include "./../include/config.h"

// Mutexs
HANDLE hMutexRoom;

// connected Clients Array
int clientCount = 0;
Client  listClients[MAXCONN];

// For thread management
HANDLE  hThreadArray[MAXCONN];

typedef struct{
    int sender_index;
    char msg[LENGTH_SEND];
}BroadParams;

DWORD WINAPI broadcast(void * param) {
    BroadParams broad = *(BroadParams*) param;
    // debut section critique
    WaitForSingleObject(hMutexRoom, INFINITE);
    if(clientCount > 1){
        for(int i = 0 ; i< clientCount; i++){
            if (broad.sender_index != i) { // all clients except the messenger.
                send(listClients[i].sockID, broad.msg, LENGTH_SEND, 0);
            }
        }
    }
    // fin section critique
    ReleaseMutex(hMutexRoom);
    ZeroMemory(broad.msg, LENGTH_SEND);
    return 0;
}

DWORD WINAPI client_handler(void * indexV){
    int index = *(int*) indexV;
    int receive;
    char recv_buffer[LENGTH_MSG];
    char *nickname = (char*)malloc(LENGTH_NAME);
    BroadParams broad;

    // debut section critique
    WaitForSingleObject(hMutexRoom, INFINITE);
        Client current_client = listClients[index];
        broad.sender_index = current_client.index;
    // fin section critique
    ReleaseMutex(hMutexRoom);

    recv(current_client.sockID, nickname, LENGTH_NAME, 0);
    strcpy(current_client.nickName, nickname);  free(nickname);
    sprintf(broad.msg, "\"%s\" joined the chatroom.",current_client.nickName);

    // create thread
    CreateThread(NULL, 0, broadcast, (void *) &broad, 0, NULL); 
    
    do {
        // Clear buffers
        ZeroMemory(recv_buffer, LENGTH_MSG);
        
        receive = recv(current_client.sockID, recv_buffer, LENGTH_MSG, 0);
        if ( receive > 0 ){
            sprintf(broad.msg, "%s : %s", current_client.nickName, recv_buffer);
            CreateThread(NULL, 0, broadcast, (void *) &broad, 0, NULL);  // create thread
        }
        else {
            sprintf(broad.msg, "\"%s\" just left the chatroom.", current_client.nickName);
            HANDLE hBroad = CreateThread(NULL, 0, broadcast, (void *) &broad, 0, NULL);  // create thread
            WaitForSingleObject(hBroad, INFINITE);
            CloseHandle(hBroad);
            // debut section critique
            WaitForSingleObject(hMutexRoom, INFINITE);
                free_client(&current_client);
                trim_array_from(index, listClients, &clientCount);
            // fin section critique
            ReleaseMutex(hMutexRoom);
            return 0;
        }
    }while(receive >0);
}

int main(int argc, char * argv[]){

    // create Mutex
    hMutexRoom = mutexCreate();

    if(hMutexRoom == NULL){
        printf("Error : Mutex nnot created !\n");
        exit(1);
    }

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
        ipAddress = (char*) malloc((strlen(argv[1])+1)*sizeof(char));
        strcpy(ipAddress, argv[1]);
    }

    // set sockaddr structure
    SOCKADDR_set(&serverAdd, ipAddress, portNumber);
    
    if ( bind(serverSocket, (struct sockaddr*) &serverAdd , sizeof(serverAdd)) == -1 )  return -1;

    // Listening, on new connections
    if ( listen(serverSocket, SOMAXCONN) == -1 ) return -2;

    printf("Server [%s] listening on port %d .....\n",ipAddress, portNumber);
    
    while(1){
        SOCKET * newClient;
        *newClient = accept(serverSocket, (struct sockaddr*) &listClients[clientCount].addr, NULL);
        // debut section critique
        WaitForSingleObject(hMutexRoom, INFINITE);
        listClients[clientCount].sockID = *newClient;

        if( listClients[clientCount].sockID != -1 ){
            listClients[clientCount].index = clientCount;
            hThreadArray[clientCount] = CreateThread( NULL, 0, client_handler, (void *) &(listClients[clientCount].index), 0, NULL);
            clientCount ++;
        }
        // fin section critique
        ReleaseMutex(hMutexRoom);
    }

    // waits till all child threads are finished.
    // If FALSE, the function returns when the state of any one of the objects is set to signaled
    // The INFINITE - the function will return only when the specified objects are signaled.
    WaitForMultipleObjects(clientCount,hThreadArray,FALSE,INFINITE);

    // Closing all Thread Handles
    for(int i = 0 ; i < clientCount ; i ++)
        CloseHandle(hThreadArray[i]);
    
    // Shutdown winsock
    shutdown(serverSocket, SD_BOTH);
    closesocket(serverSocket);

    return 0;
}