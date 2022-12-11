#include "sharedmemory.h"
#include "mapping.h"
#include "utils.h"

PSHAREDMEMORY CreateSharedMemory(STRING mapping_name, int size){
    int symbol_size=33554432+4;//32Kb for saving symbol.(Max 1024 Variables)
    int data_size=size;
    int memory_size=symbol_size+data_size;
    PSHAREDMEMORY mem=(PSHAREDMEMORY) malloc(sizeof(SHAREDMEMORY));
    char* mapping_name0=qfs(2,"LIBIPC_",mapping_name);
    PMAPPING_KEY mapping_key= CreateKey(mapping_name0);
    free(mapping_name0);
    if (mapping_key!=NULL){
        UINT32 RET;
        RET=CreateOrOpenMemoryMapping(mapping_key,memory_size);
        if (RET){
            return NULL;
        }
        mem->mapping_key=mapping_key;
        RET=MappingMemory(mapping_key);
        if (RET){
            return NULL;
        }
        PARRAYLIST symbol_list=AllocArrayList(1024,mapping_key->MemoryPointer,32);
        PBYTEBUFFER data_buffer=AllocByteBuffer(data_size,mapping_key->MemoryPointer+symbol_size);
        mem->data_buffer=data_buffer;
        mem->symbol_list=symbol_list;

        mem->SetElement=SHARED_MEMORY_SetElement;
        mem->SetInt8=SHARED_MEMORY_SetInt8;
        mem->SetInt16=SHARED_MEMORY_SetInt16;
        mem->SetInt32=SHARED_MEMORY_SetInt32;
        mem->SetInt64=SHARED_MEMORY_SetInt64;
        mem->SetUInt8=SHARED_MEMORY_SetUInt8;
        mem->SetUInt16=SHARED_MEMORY_SetUInt16;
        mem->SetUInt32=SHARED_MEMORY_SetUInt32;
        mem->SetUInt64=SHARED_MEMORY_SetUInt64;
        mem->SetFloat32=SHARED_MEMORY_SetFloat32;
        mem->SetFloat64=SHARED_MEMORY_SetFloat64;
        mem->SetBoolean=SHARED_MEMORY_SetUInt8;

        mem->GetElement=SHARED_MEMORY_GetElement;
        mem->GetInt8=SHARED_MEMORY_GetInt8;
        mem->GetInt16=SHARED_MEMORY_GetInt16;
        mem->GetInt32=SHARED_MEMORY_GetInt32;
        mem->GetInt64=SHARED_MEMORY_GetInt64;
        mem->GetUInt8=SHARED_MEMORY_GetUInt8;
        mem->GetUInt16=SHARED_MEMORY_GetUInt16;
        mem->GetUInt32=SHARED_MEMORY_GetUInt32;
        mem->GetUInt64=SHARED_MEMORY_GetUInt64;
        mem->GetFloat32=SHARED_MEMORY_GetFloat32;
        mem->GetFloat64=SHARED_MEMORY_GetFloat64;
        mem->GetBoolean=SHARED_MEMORY_GetUInt8;
        return mem;
    }
    return NULL;
}
int SHARED_MEMORY_SetElement(PSHAREDMEMORY memory, STRING name, void* data, int size){
    LOCK(memory->mapping_key->MUTEX);
    if (strlen(name)>24){
        LG_ERR("%s","Variable Name Too Long(Max Variable Length is 24)");
        UNLOCK(memory->mapping_key->MUTEX);
        return -1;
    }
    //create symbol
    char symbol[32]={0};
    strcpy(symbol,name);
    memcpy(symbol+24,&memory->data_buffer->Position,4);
    memcpy(symbol+28,&size,4);
    //search if exists
    PARRAYLIST lst=memory->symbol_list;
    for (int i = 0; i < lst->Position; ++i) {
        char* symbol0=lst->Get(lst,i);
        UINT8 flag=BOOL_TRUE;
        for (int j = 0; j < 32; ++j) {
            if (symbol[j]!=symbol0[j]){
                flag=BOOL_FALSE;
            }
        }
        if (flag){
            int sPos;
            int sSize;
            memcpy(&sPos,symbol0+24,4);
            memcpy(&sSize,symbol0+28,4);
            if (size!=sSize){
                UNLOCK(memory->mapping_key->MUTEX);
                return -1;
            }
            int ret=memory->data_buffer->Write(memory->data_buffer,data,sPos,size);
            if (ret){
                UNLOCK(memory->mapping_key->MUTEX);
                return -1;
            }
            UNLOCK(memory->mapping_key->MUTEX);
            return 0;
        }
    }
    //copy to memory mapping
    int ret=memory->data_buffer->Append(memory->data_buffer,data,size);
    if (ret){
        UNLOCK(memory->mapping_key->MUTEX);
        return -1;
    }
    //register element
    memory->symbol_list->Add(memory->symbol_list,symbol);
    UNLOCK(memory->mapping_key->MUTEX);
    return 0;
}

int SHARED_MEMORY_SetInt8(PSHAREDMEMORY memory, STRING name, INT8 data){
    return SHARED_MEMORY_SetElement(memory, name, &data, 1);
}

int SHARED_MEMORY_SetInt16(PSHAREDMEMORY memory, STRING name, INT16 data){
    return SHARED_MEMORY_SetElement(memory, name, &data, 2);
}

int SHARED_MEMORY_SetInt32(PSHAREDMEMORY memory, STRING name, INT32 data){
    return SHARED_MEMORY_SetElement(memory, name, &data, 4);
}

int SHARED_MEMORY_SetInt64(PSHAREDMEMORY memory, STRING name, INT64 data){
    return SHARED_MEMORY_SetElement(memory, name, &data, 8);
}

int SHARED_MEMORY_SetUInt8(PSHAREDMEMORY memory, STRING name, UINT8 data){
    return SHARED_MEMORY_SetElement(memory, name, &data, 1);
}

int SHARED_MEMORY_SetUInt16(PSHAREDMEMORY memory, STRING name, UINT16 data){
    return SHARED_MEMORY_SetElement(memory, name, &data, 2);
}

int SHARED_MEMORY_SetUInt32(PSHAREDMEMORY memory, STRING name, UINT32 data){
    return SHARED_MEMORY_SetElement(memory, name, &data, 4);
}

int SHARED_MEMORY_SetUInt64(PSHAREDMEMORY memory, STRING name, UINT64 data){
    return SHARED_MEMORY_SetElement(memory, name, &data, 8);
}

int SHARED_MEMORY_SetFloat32(PSHAREDMEMORY memory, STRING name, FLOAT32 data){
    return SHARED_MEMORY_SetElement(memory, name, &data, 4);
}

int SHARED_MEMORY_SetFloat64(PSHAREDMEMORY memory, STRING name, FLOAT64 data){
    return SHARED_MEMORY_SetElement(memory, name, &data, 8);
}

void* SHARED_MEMORY_GetElement(PSHAREDMEMORY memory, STRING name){
    LOCK(memory->mapping_key->MUTEX);
    if (strlen(name)>24){
        LG_ERR("%s","Variable Name Too Long(Max Variable Length is 24)");
        UNLOCK(memory->mapping_key->MUTEX);
        return NULL;
    }
    char symbol[24]={0};
    strcpy(symbol,name);
    PARRAYLIST lst=memory->symbol_list;
    for (int i = 0; i < lst->Position; ++i) {
        //compare p0-p24
        char* symbol0=(char*)lst->Get(lst,i);
        INT8 flag=BOOL_TRUE;
        for (int j = 0; j < 24; ++j) {
            if (symbol[j]!=symbol0[j]){
                flag=BOOL_FALSE;
            }
        }
        if (flag==BOOL_TRUE){
            int sPos;
            int sSize;
            memcpy(&sPos,symbol0+24,4);
            memcpy(&sSize,symbol0+28,4);
            PBYTEBUFFER buffer=memory->data_buffer;
            void* data=buffer->Read(buffer,sPos,sSize);
            UNLOCK(memory->mapping_key->MUTEX);
            return data;
        }
        free(symbol0);
    }
    UNLOCK(memory->mapping_key->MUTEX);
    return NULL;
}

INT8 SHARED_MEMORY_GetInt8(PSHAREDMEMORY memory, STRING name){
    void* data= SHARED_MEMORY_GetElement(memory,name);
    INT8 i;
    memcpy(&i,data,1);
    free(data);
    return i;
}

INT16 SHARED_MEMORY_GetInt16(PSHAREDMEMORY memory, STRING name){
    void* data= SHARED_MEMORY_GetElement(memory,name);
    INT16 i;
    memcpy(&i,data,2);
    free(data);
    return i;
}

INT32 SHARED_MEMORY_GetInt32(PSHAREDMEMORY memory, STRING name){
    void* data= SHARED_MEMORY_GetElement(memory,name);
    INT32 i;
    memcpy(&i,data,4);
    free(data);
    return i;
}

INT64 SHARED_MEMORY_GetInt64(PSHAREDMEMORY memory, STRING name){
    void* data= SHARED_MEMORY_GetElement(memory,name);
    INT64 i;
    memcpy(&i,data,8);
    free(data);
    return i;
}

UINT8 SHARED_MEMORY_GetUInt8(PSHAREDMEMORY memory, STRING name){
    void* data= SHARED_MEMORY_GetElement(memory,name);
    UINT8 i;
    memcpy(&i,data,1);
    free(data);
    return i;
}

UINT16 SHARED_MEMORY_GetUInt16(PSHAREDMEMORY memory, STRING name){
    void* data= SHARED_MEMORY_GetElement(memory,name);
    UINT16 i;
    memcpy(&i,data,2);
    free(data);
    return i;
}

UINT32 SHARED_MEMORY_GetUInt32(PSHAREDMEMORY memory, STRING name){
    void* data= SHARED_MEMORY_GetElement(memory,name);
    UINT32 i;
    memcpy(&i,data,4);
    free(data);
    return i;
}

UINT64 SHARED_MEMORY_GetUInt64(PSHAREDMEMORY memory, STRING name){
    void* data= SHARED_MEMORY_GetElement(memory,name);
    UINT64 i;
    memcpy(&i,data,8);
    free(data);
    return i;
}

FLOAT32 SHARED_MEMORY_GetFloat32(PSHAREDMEMORY memory, STRING name){
    void* data= SHARED_MEMORY_GetElement(memory,name);
    FLOAT32 i;
    memcpy(&i,data,4);
    free(data);
    return i;
}

FLOAT64 SHARED_MEMORY_GetFloat64(PSHAREDMEMORY memory, STRING name){
    void* data= SHARED_MEMORY_GetElement(memory,name);
    FLOAT64 i;
    memcpy(&i,data,8);
    free(data);
    return i;
}

void CloseSharedMemory(PSHAREDMEMORY memory){
    if (memory!=NULL){
        FreeArrayList(memory->symbol_list);
        FreeByteBuffer(memory->data_buffer);
        EndMappingMemory(memory->mapping_key);
        CloseMemoryMapping(memory->mapping_key);
    }
}