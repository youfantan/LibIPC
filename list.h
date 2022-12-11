#ifndef SHAREDMEMORY_LIST_H
#define SHAREDMEMORY_LIST_H

#include "bytebuffer.h"

#define CREATE_ARRAYLIST(size,type) AllocArrayList(size,NULL,sizeof(type))
#define DEFAULT_ARRAYLIST_SIZE 32
typedef void* Element;
typedef struct ArrayList{
    Element Pointer;
    int ElementSize;
    int Position;
    int Capacity;
    UINT8 SelfAllocated;
    Element(*Get)(struct ArrayList* list, int index);
    void(*Set)(struct ArrayList* list, int index, Element element);
    void(*Add)(struct ArrayList* list, Element element);
} ARRAYLIST,*PARRAYLIST;
PARRAYLIST AllocArrayList(int InitialSize, void* InitialData, int ElementSize);

Element ARRAYLIST_Get(PARRAYLIST list, int index);
void ARRAYLIST_Set(PARRAYLIST list, int index, Element element);
void ARRAYLIST_Add(PARRAYLIST list, Element element);
void ARRAYLIST_Reset(PARRAYLIST list, int capacity);

void FreeArrayList(PARRAYLIST list);
#endif //SHAREDMEMORY_LIST_H
