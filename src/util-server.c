#include "./../include/lib.h"
#include "./../include/const.h"

/**
 * Broadcast message to all clients except for the sender
*/
void broadcast(fd_set * master, char * msg, SOCKET sender, SOCKET server){
    for(int i=0; i<(*master).fd_count; i++){
        SOCKET to_client = (*master).fd_array[i];
        // not the sender and not the server 
        if(to_client != server && to_client != sender){
            send(to_client, msg, LENGTH_MSG, 0);
        }
    }
}