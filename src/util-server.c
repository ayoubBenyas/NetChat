#include "./../include/lib.h"
#include "./../include/type.h"
#include "./../include/const.h"
#include "./../include/global.h"


/**
 * Create Mutex
*/
HANDLE mutexCreate(){
    return  CreateMutexA(
            NULL,              // security attributes
            FALSE,             // initially not owned
            NULL);             // unnamed mutex
}

/**
 * Free the client structure
*/
void free_client(Client *clt){
    clt->index = -1;
    ZeroMemory(clt->nickName, LENGTH_NAME);
    closesocket(clt->sockID);
    ZeroMemory(clt->addr.sin_zero, sizeof(clt->addr.sin_zero));
}

/**
 * Trim array 
*/
// NOT STABLE YET GIVES ERROR IN CASE CLIENT OUT NOT FROM THE END OF THE CLIENT ARRAY
void trim_array_from(int index, Client *array, int * arrayLength){
    for(int i=index; i< *arrayLength -1 ; i++){
        strcpy((array+i)->nickName, (array+i+1)->nickName);
        (array+i)->addr = (array+i+1)->addr;
        (array+i)->sockID = (array+i+1)->sockID;
        ((array+i)->index)--;
        printf("%s : form %d to %d",(array+i)->nickName, (array+i)->index+1, (array+i)->index);
    }
    (*arrayLength)--;
}


DWORD WINAPI broadcast(void * param) {
    BroadParams broad = *(BroadParams*) param;
    // debut section critique
    LOCK;
    if(clientCount > 1){
        for(int i = 0 ; i< clientCount; i++){
            if (broad.sender_index != i) { // all clients except the messenger.
                send(listClients[i].sockID, broad.msg, LENGTH_SEND, 0);
            }
        }
    }
    // fin section critique
    UNLOCK;
    ZeroMemory(broad.msg, LENGTH_SEND);
    return 0;
}

DWORD WINAPI client_handler(void * indexV){
    int index = *(int*) indexV;
    int receive;
    char recv_buffer[LENGTH_MSG];
    char *nickname = (char*)malloc(LENGTH_NAME);
    BroadParams broad;

    // debut section critique
    LOCK;
        Client current_client = listClients[index];
        broad.sender_index = current_client.index;
    // fin section critique
    UNLOCK;

    recv(current_client.sockID, nickname, LENGTH_NAME, 0);
    strcpy(current_client.nickName, nickname);  free(nickname);
    sprintf(broad.msg, "\"%s\" joined the chatroom.",current_client.nickName);

    // create thread
    CreateThread(NULL, 0, broadcast, (void *) &broad, 0, NULL); 
    
    do {
        // Clear buffers
        ZeroMemory(recv_buffer, LENGTH_MSG);
        
        receive = recv(current_client.sockID, recv_buffer, LENGTH_MSG, 0);
        if ( receive > 0 ){
            sprintf(broad.msg, "%s : %s", current_client.nickName, recv_buffer);
            CreateThread(NULL, 0, broadcast, (void *) &broad, 0, NULL);  // create thread
        }
        else {
            sprintf(broad.msg, "\"%s\" just left the chatroom.", current_client.nickName);
            HANDLE hBroad = CreateThread(NULL, 0, broadcast, (void *) &broad, 0, NULL);  // create thread
            WaitForSingleObject(hBroad, INFINITE);
            CloseHandle(hBroad);
            // debut section critique
            LOCK;
                free_client(&current_client);
                trim_array_from(index, listClients, &clientCount);
            // fin section critique
            UNLOCK;
            return 0;
        }
    }while(receive >0);
}
