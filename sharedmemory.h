#include "platform.h"
#include "bytebuffer.h"
#include "list.h"

#ifndef SHAREDMEMORY_SHAREDMEMORY_H
#define SHAREDMEMORY_SHAREDMEMORY_H
#define DEFAULT_SHARED_MEMORY_SIZE 32*1024
typedef struct SharedMemory{
    PMAPPING_KEY mapping_key;
    PBYTEBUFFER data_buffer;
    PARRAYLIST symbol_list;

    int (*SetElement)(struct SharedMemory* memory, STRING name, void* data, int size);
    int (*SetInt8)(struct SharedMemory* memory, STRING name, INT8 data);
    int (*SetInt16)(struct SharedMemory* memory, STRING name, INT16 data);
    int (*SetInt32)(struct SharedMemory* memory, STRING name, INT32 data);
    int (*SetInt64)(struct SharedMemory* memory, STRING name, INT64 data);
    int (*SetUInt8)(struct SharedMemory* memory, STRING name, UINT8 data);
    int (*SetUInt16)(struct SharedMemory* memory, STRING name, UINT16 data);
    int (*SetUInt32)(struct SharedMemory* memory, STRING name, UINT32 data);
    int (*SetUInt64)(struct SharedMemory* memory, STRING name, UINT64 data);
    int (*SetFloat32)(struct SharedMemory* memory, STRING name, FLOAT32 data);
    int (*SetFloat64)(struct SharedMemory* memory, STRING name, FLOAT64 data);
    int (*SetBoolean)(struct SharedMemory* memory, STRING name, UINT8 data);

    void* (*GetElement)(struct SharedMemory* memory, STRING name);
    INT8 (*GetInt8)(struct SharedMemory* memory, STRING name);
    INT16 (*GetInt16)(struct SharedMemory* memory, STRING name);
    INT32 (*GetInt32)(struct SharedMemory* memory, STRING name);
    INT64 (*GetInt64)(struct SharedMemory* memory, STRING name);
    UINT8 (*GetUInt8)(struct SharedMemory* memory, STRING name);
    UINT16 (*GetUInt16)(struct SharedMemory* memory, STRING name);
    UINT32 (*GetUInt32)(struct SharedMemory* memory, STRING name);
    UINT64 (*GetUInt64)(struct SharedMemory* memory, STRING name);
    FLOAT32 (*GetFloat32)(struct SharedMemory* memory, STRING name);
    FLOAT64 (*GetFloat64)(struct SharedMemory* memory, STRING name);
    UINT8 (*GetBoolean)(struct SharedMemory* memory, STRING name);
} SHAREDMEMORY,*PSHAREDMEMORY;

PSHAREDMEMORY CreateSharedMemory(STRING mapping_name, int size);
void CloseSharedMemory(PSHAREDMEMORY memory);

int SHARED_MEMORY_SetElement(PSHAREDMEMORY memory, STRING name, void* data, int size);
int SHARED_MEMORY_SetInt8(PSHAREDMEMORY memory, STRING name, INT8 data);
int SHARED_MEMORY_SetInt16(PSHAREDMEMORY memory, STRING name, INT16 data);
int SHARED_MEMORY_SetInt32(PSHAREDMEMORY memory, STRING name, INT32 data);
int SHARED_MEMORY_SetInt64(PSHAREDMEMORY memory, STRING name, INT64 data);
int SHARED_MEMORY_SetUInt8(PSHAREDMEMORY memory, STRING name, UINT8 data);
int SHARED_MEMORY_SetUInt16(PSHAREDMEMORY memory, STRING name, UINT16 data);
int SHARED_MEMORY_SetUInt32(PSHAREDMEMORY memory, STRING name, UINT32 data);
int SHARED_MEMORY_SetUInt64(PSHAREDMEMORY memory, STRING name, UINT64 data);
int SHARED_MEMORY_SetFloat32(PSHAREDMEMORY memory, STRING name, FLOAT32 data);
int SHARED_MEMORY_SetFloat64(PSHAREDMEMORY memory, STRING name, FLOAT64 data);

void* SHARED_MEMORY_GetElement(PSHAREDMEMORY memory, STRING name);
INT8 SHARED_MEMORY_GetInt8(PSHAREDMEMORY memory, STRING name);
INT16 SHARED_MEMORY_GetInt16(PSHAREDMEMORY memory, STRING name);
INT32 SHARED_MEMORY_GetInt32(PSHAREDMEMORY memory, STRING name);
INT64 SHARED_MEMORY_GetInt64(PSHAREDMEMORY memory, STRING name);
UINT8 SHARED_MEMORY_GetUInt8(PSHAREDMEMORY memory, STRING name);
UINT16 SHARED_MEMORY_GetUInt16(PSHAREDMEMORY memory, STRING name);
UINT32 SHARED_MEMORY_GetUInt32(PSHAREDMEMORY memory, STRING name);
UINT64 SHARED_MEMORY_GetUInt64(PSHAREDMEMORY memory, STRING name);
FLOAT32 SHARED_MEMORY_GetFloat32(PSHAREDMEMORY memory, STRING name);
FLOAT64 SHARED_MEMORY_GetFloat64(PSHAREDMEMORY memory, STRING name);

#endif //SHAREDMEMORY_SHAREDMEMORY_H
