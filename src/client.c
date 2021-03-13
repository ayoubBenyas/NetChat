#include "./../include/lib.h"
#include "./../include/const.h"
#include "./../include/config.h"
#include "./../include/prototype.h"


DWORD WINAPI recv_msg_handler(void* sockIdV) {
    SOCKET sockId = *(SOCKET*) sockIdV;
    char receiveMessage[LENGTH_MSG];
    while (1) {
        ZeroMemory(receiveMessage, LENGTH_MSG);
        int receive = recv(sockId, receiveMessage, LENGTH_MSG, 0);
        if (receive >= 0) {
            printf("\r%s\n", receiveMessage);
            printf("You : ");
        } else {
            // in case of error or client quit
            return 0;
        }
    }
}

DWORD WINAPI send_msg_handler(void* sockIdV) {
    SOCKET sockId = *(SOCKET*) sockIdV;
    char message[LENGTH_MSG];
    while (1) {
        ZeroMemory(message, LENGTH_MSG);
        printf("You : ");
        gets(message);
        while ( strlen(message) == 0 ) {
            printf("\rYou  : "); gets(message);
        }

        // client wants to exit
        if (strcmp(message, ":exit") == 0) {
            printf("you are exiting the room !\n");
            closesocket(sockId);
            return 0;
        }
        // send the message to server
        send(sockId, message, LENGTH_MSG, 0);
    }
}


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