#ifndef PLATFORM_UNKNOWN
#ifndef SHAREDMEMORY_PLATFORM_H
#define SHAREDMEMORY_PLATFORM_H
//general definition
typedef unsigned char* PBYTE;
typedef unsigned char BYTE;
typedef const char* STRING;
#define RETCODE unsigned int
//platform definition
#define POINTER_SIZE 8 //64Bit

#ifdef PLATFORM_WINDOWS
#include "windows.h"
typedef void* mutex_t;
typedef void* event_t;
typedef signed char INT8;
typedef signed short INT16;
typedef signed int INT32;
typedef signed long long INT64;
typedef unsigned char UINT8;
typedef unsigned short UINT16;
typedef unsigned int UINT32;
typedef unsigned long long UINT64;
typedef float FLOAT32;
typedef double FLOAT64;
#define BOOL_TRUE 0x00
#define BOOL_FALSE 0xff

typedef struct mapping_key_t{
    STRING KeyValue;
    mutex_t MUTEX;
    int MappingSize;
    void* MappingHandle;
    void* MemoryPointer;
    UINT8 isCreated;
} MAPPING_KEY,*PMAPPING_KEY;
#define LOCK(mutex) WaitForSingleObject(mutex, INFINITE)
#define UNLOCK(mutex) TryReleaseMutex(mutex)
#define GetProcessID() GetCurrentProcessId()
#define WaitForEvent(event) WaitForSingleObject(event, INFINITE)
#define TryForEvent(event) WaitForSingleObject(event, 0)
#define ActivateEvent(event) SetEvent(event)
#define TryActivateEvent(event) PulseEvent(event)
#define InhibitEvent(event) ResetEvent(event)
#define BOOLTOWINBOOL(val) val==BOOL_TRUE?TRUE:FALSE
HANDLE CreateOrOpenEvent(STRING name, UINT8 use_manual_mode);
void TryReleaseMutex(HANDLE mutex);
#endif

#ifdef PLATFORM_LINUX
#define MAPPING_KEY key_t;
#endif

#endif

#endif