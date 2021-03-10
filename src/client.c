#include "./../include/lib.h"
#include "./../include/const.h"
#include "./../include/proto.h"
#include "./../include/config.h"

int flag=0;

DWORD WINAPI recv_msg_handler(void* sockIdV) {
    SOCKET sockId = *(SOCKET*) sockIdV;
    char receiveMessage[LENGTH_SEND] = {};
    while (1) {
        int receive = recv(sockId, receiveMessage, LENGTH_SEND, 0);
        if (receive > 0) {
            printf("\r%s\n", receiveMessage);
            printf("You  : ");
            memset(receiveMessage, 0,sizeof(receiveMessage));
        } else {
            break;
        }
    }
    return 0;
}

DWORD WINAPI send_msg_handler(void* sockIdV) {
    SOCKET sockId = *(SOCKET*) sockIdV;
    char message[LENGTH_MSG] = {};
    puts("[press to continue!]");
    while (1) {
        fflush(stdin);
        printf("You  : ");
        while ( gets(message) != NULL) {
            if (strlen(message) == 0) {
                printf("You  : ");
            } else {
                break;
            }
        }
        if (strcmp(message, ":exit") == 0) {
            send(sockId, "leave chatroom", LENGTH_MSG, 0);
            break;
        }else{
            send(sockId, message, LENGTH_MSG, 0);
        }
    }
    flag=1;
    return 0;
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

    CreateThread( NULL, 0, recv_msg_handler, (void *) &clientSocket, 0, NULL);
    CreateThread( NULL, 0, send_msg_handler, (void *) &clientSocket, 0, NULL);

    while (1) {
        if(flag) {
            printf("\nBye\n");
            break;
        }
    }
    
    // Shutdown winsock
    shutdown(clientSocket, SD_BOTH);

    getchar();
    closesocket(clientSocket);

    return 0;
}