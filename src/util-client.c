#include "./../include/lib.h"
#include "./../include/const.h"

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
