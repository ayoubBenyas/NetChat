#ifndef H_GLOBAL
#define H_GLOBAL    

#include "./../include/type.h"
#include "./../include/global.h"

// connected Clients Array
extern int clientCount ;
extern Client listClients[MAXCONN];

// For thread management
extern HANDLE  hThreadArray[MAXCONN];

// Mutexs
extern HANDLE hMutexRoom;
#define LOCK WaitForSingleObject(hMutexRoom, INFINITE)
#define UNLOCK ReleaseMutex(hMutexRoom)

#endif //H_GLOBAL