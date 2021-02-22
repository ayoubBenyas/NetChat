#include "./../include/lib.h"
#include "./../include/const.h"
#include "./../include/prototype.h"

void main (int argc, char * argv[]){
     
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

    SOCKADD_bind(&serveradd, ipAddress, portNumber);

    // connect to server
    int connection =  connect(clientSocket, (struct sockaddr*) &serveradd, sizeof(struct sockaddr));

    if(connection == -1){
        printf("connection error : %ld \n", WSAGetLastError());
        printf("Can't connect to the server, exit !\n");
        closesocket(clientSocket);
        exit(3);
    }

    // recieve data from  the server
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

    return;
}