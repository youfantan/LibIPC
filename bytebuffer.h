#ifndef SHAREDMEMORY_BYTEBUFFER_H
#define SHAREDMEMORY_BYTEBUFFER_H
#include "platform.h"
typedef struct ByteBuffer{
    void* Pointer;
    int Limit;
    int Position;
    UINT8 SelfAllocated;
    int (*Write)(struct ByteBuffer* buffer, void* data, int offset, int size);
    void* (*Read)(struct ByteBuffer* buffer, int offset, int size);
    int (*Append)(struct ByteBuffer* buffer, void* data, int size);
    void* (*Get)(struct ByteBuffer* buffer, int size);
    int (*SetPosition)(struct ByteBuffer* buffer, int offset);
    int (*Reset)(struct ByteBuffer* buffer, int new_size);
    INT8 (*ReadInt8)(struct ByteBuffer* buffer);
    INT16 (*ReadInt16)(struct ByteBuffer* buffer);
    INT32 (*ReadInt32)(struct ByteBuffer* buffer);
    INT64 (*ReadInt64)(struct ByteBuffer* buffer);
    int (*WriteInt8)(struct ByteBuffer* buffer, INT8 value);
    int (*WriteInt16)(struct ByteBuffer* buffer, INT16 value);
    int (*WriteInt32)(struct ByteBuffer* buffer, INT32 value);
    int (*WriteInt64)(struct ByteBuffer* buffer, INT64 value);
} BYTEBUFFER,*PBYTEBUFFER;
PBYTEBUFFER AllocByteBuffer(int size, void* InitialData);
int BYTEBUFFER_Write(PBYTEBUFFER buffer, void* data, int offset, int size);
void* BYTEBUFFER_Read(PBYTEBUFFER buffer, int offset, int size);
int BYTEBUFFER_Append(PBYTEBUFFER buffer, void* data, int size);
void* BYTEBUFFER_Get(PBYTEBUFFER buffer, int size);
int BYTEBUFFER_SetPosition(PBYTEBUFFER buffer, int offset);
int BYTEBUFFER_Reset(PBYTEBUFFER buffer, int new_size);

INT8 BYTEBUFFER_ReadInt8(PBYTEBUFFER buffer);
INT16 BYTEBUFFER_ReadInt16(PBYTEBUFFER buffer);
INT32 BYTEBUFFER_ReadInt32(PBYTEBUFFER buffer);
INT64 BYTEBUFFER_ReadInt64(PBYTEBUFFER buffer);

int BYTEBUFFER_WriteInt8(PBYTEBUFFER buffer, INT8 value);
int BYTEBUFFER_WriteInt16(PBYTEBUFFER buffer, INT16 value);
int BYTEBUFFER_WriteInt32(PBYTEBUFFER buffer, INT32 value);
int BYTEBUFFER_WriteInt64(PBYTEBUFFER buffer, INT64 value);

int FreeByteBuffer(PBYTEBUFFER buffer);
#endif //SHAREDMEMORY_BYTEBUFFER_H
