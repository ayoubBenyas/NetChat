#include "./../include/lib.h"
#include "./../include/const.h"
#include "./../include/proto.h"
#include "./../include/config.h"


DWORD WINAPI recv_msg_handler(void* sockIdV) {
    SOCKET sockId = *(SOCKET*) sockIdV;
    char receiveMessage[LENGTH_SEND];
    while (1) {
        ZeroMemory(receiveMessage, LENGTH_SEND);
        int receive = recv(sockId, receiveMessage, LENGTH_SEND, 0);
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
    char message[LENGTH_SEND];
    while (1) {
        ZeroMemory(message, LENGTH_SEND);
        printf("You : ");
        gets(message);
        while ( strlen(message) == 0 ) {
            printf("\rYou  : "); gets(message);
        }

        // client wants to exit
        if (strcmp(message, QUIT_ROOM) == 0) {
            printf("you are exiting the room !\n");
            closesocket(sockId);
            return 0;
        }
        // send the message to server
        send(sockId, message, LENGTH_SEND, 0);
    }
}

int main (int argc, char * argv[]){
     
    // Initialize winsock
    int wsok = WINSOCK_init();

    //cerate socket
    SOCKET clientSocket = SOCKET_create();

    // Bind the socket to an ip address and port 
    int portNumber = DEFAULT_PORT;
    char *ipAddress = DEFAULT_HOST;
    struct sockaddr_in serveradd;
    
    if(argc == 3){
        portNumber = atoi(argv[2]);
        strcpy(ipAddress, argv[1]);
    }
    printf("Try to connect to [ip: %s port: %d] .....\n",ipAddress, portNumber);
    
    // Set sockaddr_in structure
    SOCKADDR_set(&serveradd, ipAddress, portNumber);

    // connect to server
    int connection =  connect(clientSocket, (struct sockaddr*) &serveradd, sizeof(struct sockaddr));
    puts("Connected succesfully!");

    if(connection == -1){
        printf("connection error : %ld \n", WSAGetLastError());
        printf("Can't connect to the server, exit !\n");
        closesocket(clientSocket);
        exit(3);
    }

    char nickname[LENGTH_NAME] = {};
    printf("Enter your name: "); gets(nickname);
    while (strlen(nickname) < 2 || strlen(nickname) >= LENGTH_NAME-1){
        printf("\nName must be around [2-30] characters.\n");
        printf("Enter your name: "); gets(nickname);
    }

    send( clientSocket, nickname, LENGTH_NAME, 0);

    HANDLE handleArray[2];

    handleArray[0] = CreateThread( NULL, 0, recv_msg_handler, (void *) &clientSocket, 0, NULL);
    handleArray[1] = CreateThread( NULL, 0, send_msg_handler, (void *) &clientSocket, 0, NULL);

    WaitForMultipleObjects(2,handleArray,TRUE,INFINITE);
    
    for(int i =0; i<2; i++)
        CloseHandle(handleArray[i]);

    // Shutdown winsock
    shutdown(clientSocket, SD_BOTH);

    closesocket(clientSocket);

    ExitProcess(0);
    return 0;
}