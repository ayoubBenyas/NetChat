#include "./../include/lib.h"
#include "./../include/const.h"
#include "./../include/proto.h"
#include "./../include/config.h"




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