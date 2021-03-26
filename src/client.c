#include "./../include/lib.h"
#include "./../include/const.h"
#include "./../include/config.h"
#include "./../include/prototype.h"


void main (int argc, char * argv[]){
     
    // Initialize winsock
    int wsok = WINSOCK_init();

    //cerate socket
    SOCKET clientSocket = SOCKET_create();

    // Bind the socket to an ip address and port 
    int portNumber = DEFAULT_PORT;
    char * ipAddress = DEFAULT_HOST;
    struct sockaddr_in serveradd;
    
    if(argc == 3){
        portNumber = atoi(argv[2]);
        ipAddress = (char*)malloc(sizeof(char)*16);
        strcpy(ipAddress, argv[1]);
    }

    SOCKADDR_set(&serveradd, ipAddress, portNumber);

    // connect to server
    int connection =  connect(clientSocket, (struct sockaddr*) &serveradd, sizeof(struct sockaddr));

    if(connection == -1){
        printf("connection error : %ld \n", WSAGetLastError());
        printf("Can't connect to the server, exit !\n");
        closesocket(clientSocket);
        exit(3);
    }
    char msg[LENGTH_MSG];

    if(recv(clientSocket, msg, sizeof(msg), 0)> 0){
        printf("%s\n", msg); ZeroMemory(msg, LENGTH_MSG);
        HANDLE handleArray[2];
        handleArray[0] = CreateThread( NULL, 0, recv_msg_handler, (void *) &clientSocket, 0, NULL);
        handleArray[1] = CreateThread( NULL, 0, send_msg_handler, (void *) &clientSocket, 0, NULL);
        WaitForMultipleObjects(2,handleArray,TRUE,INFINITE);
        // close handles
        for(int i =0; i<2; i++)
            CloseHandle(handleArray[i]);

    }

    // Shutdown
    shutdown(clientSocket, 2);

    // close socket
    closesocket(clientSocket);

    return;
}