#include "./../include/lib.h"
#include "./../include/const.h"
#include "./../include/prototype.h"

void main(int argc, char * argv[]){
   
    // Initialize winsock
    int wsok = WINSOCK_init();

    // Create socket
    SOCKET serverSocket = SOCKET_create();

    // Bind the socket to an ip address and port
    int portNumber = DEFAULT_PORT;
    char ipAddress[15] = DEFAULT_HOST;
    struct sockaddr_in serveradd;
    
    if(argc == 3){
        portNumber = atoi(argv[2]);
        strcpy(ipAddress, argv[1]);
    }

    SOCKADD_bind(&serveradd, ipAddress, portNumber);
    

    bind(serverSocket, (struct sockaddr*) &serveradd , sizeof(serveradd));

    // Listening, one connection is allowed 
    int listening = listen(serverSocket, 2); // SOMAXCONN 

    if(listening != -1){
        printf("Server listening on port %d\n", portNumber);
    }

    // Accept connection
    SOCKET clientSocket;
    struct sockaddr_in clientadd;
    clientSocket = accept(serverSocket, (struct sockaddr*) &clientadd, NULL);

    if(clientSocket != -1){
        printf("Client is connecting ... \n");
    }

    printf("Client with ip : %s , is connected\n", inet_ntoa(clientadd.sin_addr));

    // send message to client
    char clientRequest[256];
    char response[256];
    int data_len;
    
    while( (data_len = recv(clientSocket, clientRequest, sizeof(clientRequest), 0)) > 0){
        printf("client : %s\n", clientRequest);
        //strcpy(response, clientRequest);
        fflush(stdin);
        printf("server : "); gets(response);
        send(clientSocket, response, sizeof(clientRequest), 0);
        memset(clientRequest, 0, sizeof(clientRequest));
        memset(response, 0, sizeof(response));
    }
    
    printf("Client disconnected !\n");

    // Shutdown winsock
    shutdown(serverSocket, SD_RECEIVE);
    // 0 : SD_RECEIVE
    // 1 : SD_SEND
    // 2 : SD_BOTH

    // Close socket
    closesocket(serverSocket);

    return;
}