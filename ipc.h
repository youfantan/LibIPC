
#ifndef SHAREDMEMORY_IPC_H
#define SHAREDMEMORY_IPC_H
#include "mapping.h"
#include "bytebuffer.h"

#define DEFAULT_IPC_MESSAGE_SIZE 32*1024

typedef struct IPCConfig{
    PMAPPING_KEY mapping_key;
    PBYTEBUFFER meta_buffer;
    PBYTEBUFFER message_buffer;
    event_t AvailableForWrite;
    event_t AvailableForRead;
    UINT64 MessageID;
} IPCCONFIG,*PIPCCONFIG;
PIPCCONFIG CreateConfig(STRING name, int size);
void* IPC_PeekMessage(PIPCCONFIG cfg);//No-Block Transport
void* IPC_GetMessage(PIPCCONFIG cfg);//Block Transport
int IPC_WriteMessage(PIPCCONFIG cfg, void* message, int message_size);
void CloseIPCConfig(PIPCCONFIG cfg);
#endif //SHAREDMEMORY_IPC_H
