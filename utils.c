#include "utils.h"
char* qfs(int num,...){
    va_list vlst;
    va_start(vlst,num);
    char** lpStrings=(char**) malloc(num*POINTER_SIZE);
    unsigned long *szStrings=(unsigned long *) malloc(num*POINTER_SIZE);
    unsigned long totalSize=0;
    for (int i = 0; i < num; ++i) {
        lpStrings[i]=va_arg(vlst,char*);
        szStrings[i]= strlen(lpStrings[i]);
        totalSize+=szStrings[i];
    }
    va_end(vlst);
    char* lpString=(char*) malloc(totalSize+1);
    memset(lpString,0,totalSize+1);
    unsigned long mark=0;
    for (int i = 0; i < num; ++i) {
        memcpy(lpString+mark,lpStrings[i], szStrings[i]);
        mark+=szStrings[i];
    }
    free(szStrings);
    free(lpStrings);
    return lpString;
}
char* cfn(char* fp){
    UINT32 szPath= strlen(fp);
    UINT32 lastAppear=0;
    UINT32 szFileName=0;
    //test\test.cpp
    for (int i = 0; i < szPath; ++i) {
        if (fp[i]=='\\'){
            lastAppear=i+1;
            szFileName=szPath-lastAppear;
        }
    }
    char* fileName=(char*) malloc(szFileName+1);
    memset(fileName,0,szFileName+1);
    memcpy(fileName,fp+lastAppear,szFileName);
    return fileName;
}
UINT32 crc32(PBYTE buf, UINT32 size){
    UINT32 i, crc;
    crc = 0xFFFFFFFF;
    for (i = 0; i < size; i++)
        crc = crc32tab[(crc ^ buf[i]) & 0xff] ^ (crc >> 8);
    return crc ^ 0xFFFFFFFF;
}
void lg_err(int line,char* filename,char* fmt,...){
    va_list lst;
    va_start(lst,fmt);
    char msg[1024]={0};
    vsprintf(msg,fmt,lst);
    char* fn= cfn(filename);
    printf("[%s:%d] %s\n",fn,line,msg);
    free(fn);
    va_end(lst);
}