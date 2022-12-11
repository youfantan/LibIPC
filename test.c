#include <time.h>
#include "sharedmemory.h"
#include "stdio.h"
#include "ipc.h"
#include "utils.h"

int test_shared_memory_w();
int test_shared_memory_r();
int test_ipc_proc0();
int test_ipc_proc1();
int main(int argc,char** argv){
    setbuf(stdout,0);
     if (argc==3){
        if (strcmp(argv[1],"shm") == 0){
            char* arg=argv[2];
            if (strcmp(arg,"r") == 0){
                test_shared_memory_r();
            } else if (strcmp(arg,"w") == 0){
                test_shared_memory_w();
            }
        } else if (strcmp(argv[1],"ipc") == 0){
            char* arg=argv[2];
            if (strcmp(arg,"0") == 0){
                test_ipc_proc0();
            } else if (strcmp(arg,"1") == 0){
                test_ipc_proc1();
            }
        }
    }
}

int test_shared_memory_w(){
    printf("WRITE MODE\n");
    PSHAREDMEMORY shm= CreateSharedMemory("TestSharedMemory",DEFAULT_SHARED_MEMORY_SIZE);
    char data[64];
    while (getchar()!='e'){
        for (int i = 0; i < 64; ++i) {
            data[i]=rand()%257;
            printf("%d ",data[i]);
        }
        shm->SetElement(shm,"TestData",data,64);
        printf("\n");
    }
    CloseSharedMemory(shm);
    return 0;
}

int test_shared_memory_r(){
    printf("READ MODE\n");
    PSHAREDMEMORY shm= CreateSharedMemory("TestSharedMemory",DEFAULT_SHARED_MEMORY_SIZE);
    char* data;
    while (getchar()!='e'){
        data=shm->GetElement(shm,"TestData");
        for (int i = 0; i < 64; ++i) {
            printf("%d ",data[i]);
        }
        printf("\n");
    }
    CloseSharedMemory(shm);
    return 0;
}
char* generateRandomString(){
    UINT64 high;
    UINT64 low;
    __asm{
            rdtscp;
            mov high,edx;
            mov low,eax;
    }
    UINT64 tm=high<<32|(UINT64)(low<<32);
    char* string=(char*)malloc(64);
    memset(string,0,64);
    sprintf(string,"%llu",tm);
    return string;
}

int test_ipc_proc0(){
    PIPCCONFIG ipccfg= CreateConfig("TestIPC",DEFAULT_IPC_MESSAGE_SIZE);
    char* message="Hello World";
    IPC_WriteMessage(ipccfg,message, 12);
    while (getchar()!='e'){
        getchar();
        char* msg=NULL;
        do {
            msg= IPC_GetMessage(ipccfg);
            Sleep(500);
        } while (msg==NULL);
        printf("PROC0: %s\n",msg);
        char* msg0=generateRandomString();
        IPC_WriteMessage(ipccfg,msg0,64);
        printf("PROC1: %s\n",msg0);
        free(msg);
    }
    CloseIPCConfig(ipccfg);
    return 0;
}

int test_ipc_proc1(){
    PIPCCONFIG ipccfg= CreateConfig("TestIPC",DEFAULT_IPC_MESSAGE_SIZE);
    char* message=IPC_GetMessage(ipccfg);
    printf("%s\n",message);
    while (getchar()!='e'){
        getchar();
        char* msg0=generateRandomString();
        printf("PROC1: %s\n",msg0);
        IPC_WriteMessage(ipccfg,msg0,64);
        char* msg=NULL;
        do {
            msg= IPC_PeekMessage(ipccfg);
            Sleep(500);
        } while (msg==NULL);
        printf("PROC0: %s\n",msg);
        free(msg0);
    }
    CloseIPCConfig(ipccfg);
    return 0;
}