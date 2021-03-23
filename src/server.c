#include "./../include/lib.h"
#include "./../include/const.h"
#include "./../include/type.h"
#include "./../include/global.h"
#include "./../include/proto.h"
#include "./../include/config.h"


// connected Clients Array
int clientCount = 0;
Client  listClients[MAXCONN];

// For thread management
HANDLE  hThreadArray[MAXCONN];

// Mutexs
HANDLE hMutexRoom;

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
        LOCK;
        listClients[clientCount].sockID = *newClient;

        if( listClients[clientCount].sockID != -1 ){
            listClients[clientCount].index = clientCount;
            hThreadArray[clientCount] = CreateThread( NULL, 0, client_handler, (void *) &(listClients[clientCount].index), 0, NULL);
            clientCount ++;
        }
        // fin section critique
        UNLOCK;
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