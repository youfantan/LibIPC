#include "platform.h"
#include "utils.h"
#include <stdio.h>
HANDLE CreateOrOpenEvent(STRING name, UINT8 use_manual_mode){
    HANDLE event= OpenEventA(EVENT_ALL_ACCESS, FALSE, name);
    if (event==NULL){
        event=CreateEventA(NULL,BOOLTOWINBOOL(use_manual_mode),FALSE,name);
    }
    return event;
}
void TryReleaseMutex(HANDLE mutex){
    if (ReleaseMutex(mutex)==FALSE){
        LG_ERR("Cannot Release Mutex.Error Code: %d",GetLastError());
    }

}