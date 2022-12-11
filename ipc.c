#include <stdio.h>
#include "ipc.h"
#include "utils.h"
PIPCCONFIG CreateConfig(STRING name, int size){
    size+=16;
    PIPCCONFIG cfg=(PIPCCONFIG) malloc(sizeof(IPCCONFIG));
    PMAPPING_KEY mapping_key;
    char* mapping_name=qfs(3, "LIBIPC_", name, "_IPCCFG");
    mapping_key=CreateKey(mapping_name);
    free(mapping_name);
    RETCODE RET;
    RET=CreateOrOpenMemoryMapping(mapping_key, size);
    if (RET){
        return NULL;
    }
    RET=MappingMemory(mapping_key);
    if (RET){
        return NULL;
    }
    cfg->mapping_key=mapping_key;
    PBYTEBUFFER meta_buffer= AllocByteBuffer(16,mapping_key->MemoryPointer);
    PBYTEBUFFER message_buffer= AllocByteBuffer(size, mapping_key->MemoryPointer+16);
    LOCK(mapping_key->MUTEX);
    meta_buffer->SetPosition(meta_buffer,2);
    INT16 connected=meta_buffer->ReadInt16(meta_buffer);
    connected++;
    meta_buffer->WriteInt16(meta_buffer,connected);
    UNLOCK(mapping_key->MUTEX);
    cfg->message_buffer=message_buffer;
    cfg->meta_buffer=meta_buffer;
    char* write_event_name= qfs(3,"LIBIPC_",name,"_EVENT_WRITE");
    char* read_event_name= qfs(3,"LIBIPC_",name,"_EVENT_READ");
    event_t AvailableForWrite= CreateOrOpenEvent(write_event_name,BOOL_FALSE);
    event_t AvailableForRead= CreateOrOpenEvent(read_event_name,BOOL_TRUE);
    cfg->AvailableForWrite=AvailableForWrite;
    cfg->AvailableForRead=AvailableForRead;
    free(write_event_name);
    free(read_event_name);
    if (connected==1){
        ActivateEvent(AvailableForWrite);
    }
    return cfg;
}

void* IPC_PeekMessage(PIPCCONFIG cfg){
    if (TryForEvent(cfg->AvailableForRead)==WAIT_OBJECT_0){
        if (LOCK(cfg->mapping_key->MUTEX)==WAIT_OBJECT_0){
            PBYTEBUFFER meta_buffer=cfg->meta_buffer;
            meta_buffer->SetPosition(meta_buffer,16);
            INT32 message_size=meta_buffer->ReadInt32(meta_buffer);
            UINT64 message_id=(UINT64)meta_buffer->ReadInt64(meta_buffer);
            INT16 received=meta_buffer->ReadInt16(meta_buffer);
            INT16 connected=meta_buffer->ReadInt16(meta_buffer);
            received++;
            if (message_id!=cfg->MessageID){
                cfg->MessageID=message_id;
                PBYTEBUFFER message_buffer=cfg->message_buffer;
                void* message=message_buffer->Read(message_buffer,0,message_size);
                if (received==connected){
                    InhibitEvent(cfg->AvailableForRead);
                    ActivateEvent(cfg->AvailableForWrite);
                } else{
                    meta_buffer->SetPosition(meta_buffer,2);
                    meta_buffer->WriteInt16(meta_buffer,received);
                }
                UNLOCK(cfg->mapping_key->MUTEX);
                return message;
            }
        }
        UNLOCK(cfg->mapping_key->MUTEX);
    }
    return NULL;
}
void* IPC_GetMessage(PIPCCONFIG cfg){
    WaitForEvent(cfg->AvailableForRead);
    LOCK(cfg->mapping_key->MUTEX);
    PBYTEBUFFER meta_buffer=cfg->meta_buffer;
    meta_buffer->SetPosition(meta_buffer,16);
    INT32 message_size=meta_buffer->ReadInt32(meta_buffer);
    UINT64 message_id=(UINT64)meta_buffer->ReadInt64(meta_buffer);
    INT16 received=meta_buffer->ReadInt16(meta_buffer);
    INT16 connected=meta_buffer->ReadInt16(meta_buffer);
    received++;
    if (message_id!=cfg->MessageID){
        cfg->MessageID=message_id;
        PBYTEBUFFER message_buffer=cfg->message_buffer;
        void* message=message_buffer->Read(message_buffer,0,message_size);
        if (received==connected){
            InhibitEvent(cfg->AvailableForRead);
            ActivateEvent(cfg->AvailableForWrite);
        } else{
            meta_buffer->SetPosition(meta_buffer,2);
            meta_buffer->WriteInt16(meta_buffer,received);
        }
        UNLOCK(cfg->mapping_key->MUTEX);
        return message;
    }
    UNLOCK(cfg->mapping_key->MUTEX);
    return NULL;
}
int IPC_WriteMessage(PIPCCONFIG cfg, void* message, int message_size){
    WaitForEvent(cfg->AvailableForWrite);
    LOCK(cfg->mapping_key->MUTEX);
    PBYTEBUFFER meta_buffer=cfg->meta_buffer;
    PBYTEBUFFER message_buffer=cfg->message_buffer;
    meta_buffer->SetPosition(meta_buffer,2);
    meta_buffer->WriteInt16(meta_buffer,1);
    UINT32 message_digest= crc32(message, message_size);
    UINT32 cycle_high=0;
    UINT32 cycle_low=0;
    __asm{
            rdtsc;
            mov cycle_high,edx;
            mov cycle_low,eax;
    }
    UINT64 message_id=cycle_high+cycle_low+message_digest;
    meta_buffer->WriteInt64(meta_buffer,(INT64)message_id);
    meta_buffer->WriteInt32(meta_buffer,message_size);
    message_buffer->Write(message_buffer,message,0,message_size);
    cfg->MessageID=message_id;
    UNLOCK(cfg->mapping_key->MUTEX);
    ActivateEvent(cfg->AvailableForRead);
    return 0;
}
void CloseIPCConfig(PIPCCONFIG cfg){
    FreeByteBuffer(cfg->message_buffer);
    EndMappingMemory(cfg->mapping_key);
    CloseMemoryMapping(cfg->mapping_key);
}