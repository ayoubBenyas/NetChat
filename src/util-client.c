#include "./../include/lib.h"
#include "./../include/type.h"
#include "./../include/const.h"
#include "./../include/global.h"


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