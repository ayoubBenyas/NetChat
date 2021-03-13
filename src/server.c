#include "./../include/lib.h"
#include "./../include/const.h"
#include "./../include/config.h"
#include "./../include/prototype.h"

void main(int argc, char * argv[]){
   
    // Initialize winsock
    int wsok = WINSOCK_init();

    // Create socket
    SOCKET serverSocket = SOCKET_create();

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
    

    bind(serverSocket, (struct sockaddr*) &serveradd , sizeof(serveradd));

    // Listening, one connection is allowed 
    int listening = listen(serverSocket, SOMAXCONN);

    if(listening != -1){
        printf("Server listening on port %d\n", portNumber);
    }

   /// Declare the set
    fd_set master;
    FD_ZERO(&master); // cleanup set

    // Add serversocket to the master set
    FD_SET(serverSocket, &master);

    // send message to client
    char request[LENGTH_MSG];
    char response[LENGTH_MSG];
    char msg[LENGTH_MSG];

    while (TRUE)
    {
        fd_set masterCopy = master;
        int  clientCount = select(0, &masterCopy, NULL, NULL, NULL);

        for(int i=0; i< clientCount; i++){
            SOCKET socket = masterCopy.fd_array[i];
            if(socket == serverSocket){
                // server action accept client
                SOCKET clientsocket = accept(serverSocket, NULL, NULL);

                // add client to set 
                FD_SET(clientsocket, &master);

                ZeroMemory(response, LENGTH_MSG);
                sprintf(response, "Welcome to the chatroom !\r\n");

                send(clientsocket, response, LENGTH_MSG, 0);

            }else{
                // server action receive message
                ZeroMemory(request, sizeof(request));
                
                int receive = recv(socket, request, sizeof(request), 0);

                if(receive <= 0){
                    // if Error
                    closesocket(socket);
                    FD_CLR(socket, &master); // drop client socket
                }else{
                    ZeroMemory(msg, sizeof(msg));
                    sprintf(msg,"SOCKET %d : %s", socket, request);
                    // broadcast the message to other clients
                    broadcast(&master, request, socket, serverSocket);
                }
            }
        }
    }
    

    printf("Client disconnected !\n");

    // Shutdown winsock
    shutdown(serverSocket, SD_BOTH);

    // Close socket
    closesocket(serverSocket);

    return;
}