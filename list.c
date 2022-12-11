#include <stdlib.h>
#include <string.h>
#include "list.h"
Element ARRAYLIST_Get(PARRAYLIST list, int index){
    Element element=(Element) malloc(list->ElementSize);
    memcpy(element,list->Pointer+(list->ElementSize*index),list->ElementSize);
    return element;
}
void ARRAYLIST_Set(PARRAYLIST list, int index, Element element){
    if (list->Capacity<=index){
        ARRAYLIST_Reset(list,index+1);
    }
    if (index>list->Position){
        list->Position+=index+1;
        memcpy(list->Pointer-4,&list->Position,4);
    }
    memcpy(list->Pointer+(list->ElementSize*index),element,list->ElementSize);
}
void ARRAYLIST_Add(PARRAYLIST list, Element element){
    if (list->Capacity<=list->Position){
        ARRAYLIST_Reset(list,list->Capacity*2);
    }
    memcpy(list->Pointer+(list->ElementSize*list->Position),element,list->ElementSize);
    list->Position++;
    memcpy(list->Pointer-4,&list->Position,4);
}
PARRAYLIST AllocArrayList(int InitialSize, void* InitialData, int ElementSize){
    PARRAYLIST list=(PARRAYLIST) malloc(sizeof(ARRAYLIST));
    if (InitialData==NULL){
        Element Pointer=(Element) malloc(InitialSize*ElementSize);
        memset(Pointer,0,InitialSize*ElementSize);\
        list->Pointer=Pointer+4;
        list->Position=0;
        list->SelfAllocated=BOOL_TRUE;
    } else{
        int Position=0;
        memcpy(&Position,InitialData,4);
        list->Pointer=InitialData+4;
        list->Position=Position;
        list->SelfAllocated=BOOL_FALSE;
    }
    list->ElementSize=ElementSize;
    list->Capacity=InitialSize;
    list->Get=ARRAYLIST_Get;
    list->Set=ARRAYLIST_Set;
    list->Add=ARRAYLIST_Add;
    return list;
}
void ARRAYLIST_Reset(PARRAYLIST list, int capacity){
    Element Pointer=(Element) malloc(capacity*list->ElementSize);
    memset(Pointer,0,capacity*list->ElementSize);
    memcpy(Pointer,list->Pointer,list->Capacity*list->ElementSize);
    free(list->Pointer);
    list->Capacity=capacity;
    list->Pointer=Pointer;
}

void FreeArrayList(PARRAYLIST list){
    if (list->SelfAllocated==BOOL_TRUE){
        free(list->Pointer);
    }
    free(list);
}
