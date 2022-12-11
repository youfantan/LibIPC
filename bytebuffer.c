#include "bytebuffer.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>

PBYTEBUFFER AllocByteBuffer(int size, void* InitialData){
    PBYTEBUFFER buffer=(PBYTEBUFFER)malloc(sizeof(BYTEBUFFER));
    if (InitialData == NULL){
        PBYTE data=(PBYTE) malloc(size);
        memset(data,0,size);
        buffer->Pointer=data;
        buffer->SelfAllocated=BOOL_TRUE;
    } else{
        buffer->Pointer=InitialData;
        buffer->SelfAllocated=BOOL_FALSE;
    }
    buffer->Limit=size;
    buffer->Position=0;
    buffer->Write=BYTEBUFFER_Write;
    buffer->Read=BYTEBUFFER_Read;
    buffer->Append=BYTEBUFFER_Append;
    buffer->Get=BYTEBUFFER_Get;
    buffer->SetPosition=BYTEBUFFER_SetPosition;
    buffer->Reset=BYTEBUFFER_Reset;

    buffer->ReadInt8=BYTEBUFFER_ReadInt8;
    buffer->ReadInt16=BYTEBUFFER_ReadInt16;
    buffer->ReadInt32=BYTEBUFFER_ReadInt32;
    buffer->ReadInt64=BYTEBUFFER_ReadInt64;

    buffer->WriteInt8=BYTEBUFFER_WriteInt8;
    buffer->WriteInt16=BYTEBUFFER_WriteInt16;
    buffer->WriteInt32=BYTEBUFFER_WriteInt32;
    buffer->WriteInt64=BYTEBUFFER_WriteInt64;
    return buffer;
}
int BYTEBUFFER_Write(PBYTEBUFFER buffer, void* data, int offset, int size){
    if (buffer==NULL||data==NULL||size>buffer->Limit||offset>buffer->Limit){
        LG_ERR("%s","Error Input Parameter(s)");
        return -1;
    }
    PBYTE Pointer= buffer->Pointer + offset;
    memcpy(Pointer,data,size);
    return 0;
}
void* BYTEBUFFER_Read(PBYTEBUFFER buffer, int offset, int size){
    if (buffer==NULL||size>buffer->Limit||offset>buffer->Limit){
        LG_ERR("%s","Error Input Parameter(s)");
        return NULL;
    }
    PBYTE data=(PBYTE) malloc(sizeof(BYTE) * size);
    memset(data,0,size);
    PBYTE Pointer= buffer->Pointer + offset;
    memcpy(data,Pointer,size);
    return data;
}
int BYTEBUFFER_Append(PBYTEBUFFER buffer, void* data, int size){
    if (buffer==NULL||size>(buffer->Limit-buffer->Position)){
        LG_ERR("%s","Error Input Parameter(s)");
        return -1;
    }
    int oldPosition=buffer->Position;
    buffer->Position=buffer->Position+size;
    return BYTEBUFFER_Write(buffer, data, oldPosition, size);
}
void* BYTEBUFFER_Get(PBYTEBUFFER buffer, int size){
    if (buffer==NULL||size>buffer->Position){
        LG_ERR("%s","Error Input Parameter(s)");
        return NULL;
    }
    buffer->Position=buffer->Position-size;
    return BYTEBUFFER_Read(buffer, buffer->Position, size);
}
int BYTEBUFFER_SetPosition(PBYTEBUFFER buffer, int offset){
    buffer->Position=offset;
    return 0;
}
int BYTEBUFFER_Reset(PBYTEBUFFER buffer, int new_size){
    if (new_size<buffer->Limit){
        return -1;
    } else if (new_size==buffer->Limit){
        return 0;
    }
    PBYTE data=(PBYTE) malloc( new_size);
    memset(data,0,new_size);
    memcpy(data,buffer->Pointer,buffer->Limit);
    buffer->Limit=new_size;
    free(buffer->Pointer);
    buffer->Pointer=data;
    return 0;
}

INT8 BYTEBUFFER_ReadInt8(PBYTEBUFFER buffer){
    if (buffer==NULL||buffer->Position<1){
        LG_ERR("%s","Error Input Parameter(s)");
        return 0;
    }
    INT8 val=0;
    buffer->Position--;
    memcpy(&val,buffer->Pointer+buffer->Position,1);
    return val;
}

INT16 BYTEBUFFER_ReadInt16(PBYTEBUFFER buffer){
    if (buffer==NULL||buffer->Position<2){
        LG_ERR("%s","Error Input Parameter(s)");
        return 0;
    }
    INT16 val=0;
    buffer->Position-=2;
    memcpy(&val,buffer->Pointer+buffer->Position,2);
    return val;
}

INT32 BYTEBUFFER_ReadInt32(PBYTEBUFFER buffer){
    if (buffer==NULL||buffer->Position<4){
        LG_ERR("%s","Error Input Parameter(s)");
        return 0;
    }
    INT32 val=0;
    buffer->Position-=4;
    memcpy(&val,buffer->Pointer+buffer->Position,4);
    return val;
}

INT64 BYTEBUFFER_ReadInt64(PBYTEBUFFER buffer){
    if (buffer==NULL||buffer->Position<8){
        LG_ERR("%s","Error Input Parameter(s)");
        return 0;
    }
    INT64 val=0;
    buffer->Position-=8;
    memcpy(&val,buffer->Pointer+buffer->Position,8);
    return val;
}

int BYTEBUFFER_WriteInt8(PBYTEBUFFER buffer, INT8 value){
    if (buffer==NULL||(buffer->Limit-buffer->Position)<1) {
        LG_ERR("%s","Error Input Parameter(s)");
        return -1;
    }
    memcpy(buffer->Pointer+buffer->Position,&value,1);
    buffer->Position+=1;
    return 0;
}

int BYTEBUFFER_WriteInt16(PBYTEBUFFER buffer, INT16 value){
    if (buffer==NULL||(buffer->Limit-buffer->Position)<2) {
        LG_ERR("%s","Error Input Parameter(s)");
        return -1;
    }
    memcpy(buffer->Pointer+buffer->Position,&value,2);
    buffer->Position+=2;
    return 0;
}

int BYTEBUFFER_WriteInt32(PBYTEBUFFER buffer, INT32 value){
    if (buffer==NULL||(buffer->Limit-buffer->Position)<4) {
        LG_ERR("%s","Error Input Parameter(s)");
        return -1;
    }
    memcpy(buffer->Pointer+buffer->Position,&value,4);
    buffer->Position+=4;
    return 0;
}

int BYTEBUFFER_WriteInt64(PBYTEBUFFER buffer, INT64 value){
    if (buffer==NULL||(buffer->Limit-buffer->Position)<8) {
        LG_ERR("%s","Error Input Parameter(s)");
        return -1;
    }
    memcpy(buffer->Pointer+buffer->Position,&value,8);
    buffer->Position+=8;
    return 0;
}

int FreeByteBuffer(PBYTEBUFFER buffer){
    if (buffer->SelfAllocated==BOOL_TRUE){
        free(buffer->Pointer);
    }
    free(buffer);
    return 0;
}