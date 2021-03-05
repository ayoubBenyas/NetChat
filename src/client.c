#include "./../include/lib.h"
#include "./../include/const.h"
#include "./../include/proto.h"

int flag=0;


void recv_msg_handler(void* sockIdV) {
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
        receive = -1;
    }
}

void send_msg_handler(void* sockIdV) {
    SOCKET sockId = *(SOCKET*) sockIdV;
    char message[LENGTH_MSG] = {};
    while (1) {
        fflush(stdin);
        printf("[press to continue!]");
        while ( gets(message) != NULL) {
            if (strlen(message) == 0) {
                printf("You  : ");
            } else {
                break;
            }
        }
        if (strcmp(message, "exit") == 0) {
            send(sockId, "leave chatroom", LENGTH_MSG, 0);
            break;
        }else{
            send(sockId, message, LENGTH_MSG, 0);

        }
    }
    flag=1;
}


int main (int argc, char * argv[]){
     
    // Initialize winsock
    int wsok = WINSOCK_init();

    //cerate socket
    SOCKET clientSocket = SOCKET_create();

    // Bind the socket to an ip address and port 
    int portNumber = DEFAULT_PORT;
    char ipAddress[15] = DEFAULT_HOST;
    struct sockaddr_in serveradd;
    
    if(argc == 3){
        portNumber = atoi(argv[2]);
        strcpy(ipAddress, argv[1]);
    }
    printf("Try connect on [%s|%d] .....\n",ipAddress, portNumber);
    SOCKADD_bind(&serveradd, ipAddress, portNumber);

    // connect to server
     puts("Connected succesfully!");
    int connection =  connect(clientSocket, (struct sockaddr*) &serveradd, sizeof(struct sockaddr));

    if(connection == -1){
        printf("connection error : %ld \n", WSAGetLastError());
        printf("Can't connect to the server, exit !\n");
        closesocket(clientSocket);
        exit(3);
    }

    char nickname[LENGTH_NAME] = {};
    printf("{%d}Please enter your name: ",clientSocket); gets(nickname);
    while (strlen(nickname) < 2 || strlen(nickname) >= LENGTH_NAME-1){
        printf("\nName must be around [1-30] characters.\n");
        printf("{%d}Please enter your name: ",clientSocket); gets(nickname);
    }

    send( clientSocket, nickname, LENGTH_NAME, 0);

    CreateThread( NULL, 0, (void *)recv_msg_handler, (void *) &clientSocket, 0, NULL);
    CreateThread( NULL, 0, (void *)send_msg_handler, (void *) &clientSocket, 0, NULL);

    getchar();

    /*// recieve data from  the server
    char my_request[256] = "Hello server !";
    char response_server[256];
    int data_len;

    while( (data_len = send(clientSocket, my_request, sizeof(my_request), 0)) > 0){
        memset(my_request, 0,sizeof(my_request));
        recv(clientSocket, response_server, sizeof(response_server), 0);
        printf("server : %s\n", response_server);
        memset(response_server, 0,sizeof(response_server));
        fflush(stdin);
        printf("client : "); gets(my_request);
    }

    printf("server is down : error 500 \n");

    // Shutdown
    shutdown(clientSocket, 2);

    // close socket
    closesocket(clientSocket);
*/
    while (1) {
        if(flag) {
            printf("\nBye\n");
            break;
        }
    }
    puts("end{client}!!");getchar();
    return 0;
}