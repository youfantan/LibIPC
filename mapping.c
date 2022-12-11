#include <stdio.h>
#include "mapping.h"
#include "utils.h"

#ifdef PLATFORM_WINDOWS
PMAPPING_KEY CreateKey(const char* mapping_name){
    PMAPPING_KEY key=(PMAPPING_KEY) malloc(sizeof(MAPPING_KEY));
    //initialize KeyValue
    char* KeyValue=(char*) malloc(sizeof(char) * strlen(mapping_name) + 1);
    memset(KeyValue, 0, strlen(mapping_name) + 1);
    strcpy(KeyValue, mapping_name);
    key->KeyValue=KeyValue;
    //create mutex
    char* MutexValue=(char*) malloc(sizeof(char) * strlen(mapping_name) + 6);
    memset(MutexValue, 0, strlen(mapping_name) + 1);
    strcpy(MutexValue, mapping_name);
    strcat(MutexValue,"_MUTEX");
    mutex_t mutex= OpenMutexA(MUTEX_ALL_ACCESS, FALSE, MutexValue);
    if (mutex==NULL){
        mutex= CreateMutexA(NULL,FALSE,MutexValue);
        if (mutex==NULL||mutex==INVALID_HANDLE_VALUE){
            LG_ERR("Cannot Create Mutex.Error Code: %d",GetLastError());
        }
    }
    key->MUTEX=mutex;
    return key;
}
UINT32 CreateOrOpenMemoryMapping(PMAPPING_KEY mapping_key, INT32 size){
    HANDLE MappingHandle= OpenFileMappingA(FILE_MAP_READ|FILE_MAP_WRITE,FALSE,mapping_key->KeyValue);
    mapping_key->isCreated=BOOL_FALSE;
    if (MappingHandle==NULL){
        mapping_key->isCreated=BOOL_TRUE;
        MappingHandle=CreateFileMappingA(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,size,mapping_key->KeyValue);
        if (MappingHandle==NULL){
            LG_ERR("Cannot Create Memory Mapping.Error Code: %d",GetLastError());
            return -1;
        }
    }
    mapping_key->MappingHandle=MappingHandle;
    mapping_key->MappingSize=size;
    return 0;
}
UINT32 MappingMemory(PMAPPING_KEY mapping_key){
    if (mapping_key->MappingHandle!=NULL&&mapping_key->MappingHandle!=INVALID_HANDLE_VALUE){
        LPVOID MemoryPoINT32er= MapViewOfFile(mapping_key->MappingHandle,FILE_MAP_READ|FILE_MAP_WRITE,0,0,0);
        if (MemoryPoINT32er!=NULL){
            mapping_key->MemoryPointer=MemoryPoINT32er;
        } else{
            LG_ERR("Cannot Mapping Memory to Pointer.Error Code: %d",GetLastError());
            ReleaseMutex(mapping_key->MUTEX);
            return -1;
        }
    }
    return 0;
}
UINT32 EndMappingMemory(PMAPPING_KEY mapping_key){
    if (mapping_key->MappingHandle!=NULL&&mapping_key->MappingHandle!=INVALID_HANDLE_VALUE&&mapping_key->MemoryPointer!=NULL){
        if (!UnmapViewOfFile(mapping_key->MemoryPointer)){
            LG_ERR("Cannot End Memory Mapping.Error Code: %d",GetLastError());
            ReleaseMutex(mapping_key->MUTEX);
            return -1;
        }
    }
    return 0;
}
UINT32 CloseMemoryMapping(PMAPPING_KEY mapping_key){
    if (mapping_key->isCreated==BOOL_TRUE){
        CloseHandle(mapping_key->MappingHandle);
        CloseHandle(mapping_key->MUTEX);
    }
    free((void*)mapping_key->KeyValue);
    free((void*)mapping_key);
    return 0;
}
#endif