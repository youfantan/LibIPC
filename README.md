# LibIPC

LibIPC是一个跨平台的进程间通信库。

## 已实现的特性
+ SharedMemory: 进程间共享变量表
+ IPC: 进程间通信

## 待实现的特性
+ 跨平台支持

## 构建
Windows平台：  
安装cmake、msys2、clang
```
mkdir build
cd build
cmake ..
mingw32-make
```

## 使用
### 进程间共享变量表
#### PSHAREDMEMORY CreateSharedMemory(STRING mapping_name, int size) : 创建SharedMemory结构体（当作句柄使用）
+ mapping_name: 变量表名称（最大24字节）/const char\*
+ size: 变量表大小（最大为2^32-33554432字节）/INT32
+ 返回值: SharedMemory结构体指针/struct SharedMemory\* 

#### void CloseSharedMemory(PSHAREDMEMORY memory) : 关闭SharedMemory结构体
+ memory: SharedMemory结构体指针/struct SharedMemory\* 

#### void* (\*GetElement)(struct SharedMemory\* memory, STRING name) : 通过变量名获取数据
+ memory: SharedMemory结构体指针/struct SharedMemory\* 
+ name: 变量名称/const char\*
+ RET: 指向数据的指针（返回NULL/0为失败）/void*

#### int (\*SetElement)(struct SharedMemory\* memory, STRING name, void\* data, int size) : 通过变量名写入数据
+ memory: SharedMemory结构体指针/struct SharedMemory\* 
+ name: 变量名称/const char\*
+ data: 指向数据的指针/void\*
+ size: 数据大小/INT32
+ RET: 返回值（非0值为失败）/INT32

DataType: (U)IntX(8/16/32/64)/FloatX(32/64)/Boolean(UINT8))

#### int (\*SetDataType)(struct SharedMemory\* memory, STRING name, DataType data) : 通过变量名写入指定类型数据
+ memory: SharedMemory结构体指针/struct SharedMemory\* 
+ name: 变量名称/const char\*
+ data: 数据内容/DataType
+ RET: 返回值（非0值为失败）/INT32

#### DataType (\*GetDataType)(struct SharedMemory\* memory, STRING name) : 通过变量名获取指定类型数据
+ memory: SharedMemory结构体指针/struct SharedMemory\* 
+ name: 变量名称/const char\*
+ RET: 数据内容/DataType

例：
```c
PSHAREDMEMORY shm= CreateSharedMemory("TestSharedMemory",DEFAULT_SHARED_MEMORY_SIZE);//DEFAULT_SHARED_MEMORY_SIZE默认为32KBytes
char data[64]={0};
shm->SetElement(shm,"TestData",data,64);//创建/设置名为TestData的变量，指定其大小为64并传入数据
char* data0=shm->GetElement(shm,"TestData");//获取名为TestData的变量
CloseSharedMemory(shm);
```

### 进程间通信
#### PIPCCONFIG CreateConfig(STRING name, int size) : 创建IPCConfig结构体（当作句柄使用）
+ name: 进程间通信块名称/const char*
+ size: 进程间通信块大小/INT32
+ RET: IPCConfig结构体指针/struct IPCConfig\* 

#### void CloseIPCConfig(PIPCCONFIG cfg) : 关闭IPCConfig结构体
+ cfg: IPCConfig结构体指针/struct IPCConfig\* 

#### void* IPC_PeekMessage(PIPCCONFIG cfg) : 非阻塞获取信息（失败原因：获取的消息重复，即本进程已经读取过的消息或已写入的消息）
+ cfg: IPCConfig结构体指针/struct IPCConfig\* 
+ RET: 指向消息的指针（返回NULL/0为失败）/void\*

#### void* IPC_GetMessage(PIPCCONFIG cfg) : 阻塞获取信息（失败原因：获取的消息重复，即本进程已经读取过的消息或已写入的消息）
+ cfg: IPCConfig结构体指针/struct IPCConfig\* 
+ RET: 指向消息的指针（返回NULL/0为失败）/void\*

#### int IPC_WriteMessage(PIPCCONFIG cfg, void\* message, int message_size) : 阻塞写入消息
+ cfg: IPCConfig结构体指针/struct IPCConfig\* 
+ message: 消息内容/(U)INTX

例：
```c
    PIPCCONFIG ipccfg= CreateConfig("TestIPC",DEFAULT_IPC_MESSAGE_SIZE);//DEFAULT_IPC_MESSAGE_SIZE默认为32Kbytes
    char* message="Hello World";
    IPC_WriteMessage(ipccfg,message, 12);//写入消息
    char* message0=;
    do {
        message0= IPC_Get(Peek)Message(ipccfg);
    } while (msg==NULL);//重复获取消息，直到成功为止

```
## 注意
这个库只在Windows/Clang x64平台上编写并通过测试。同时本库含有少量内联汇编，Visual Studio的x64配置可能不能正常编译汇编，你可以将内联汇编转为MASM编译并链接。Linux平台适配和Visual Studio适配将在后续版本加入。


## 简述
+ ipc.c/h: 进程间通信实现
+ sharedmemory.c/h: 进程间共享变量表实现
+ bytebuffer.c/h: ByteBuffer实现
+ list.c/h: ArrayList实现
+ mapping.c/h: 平台提供内存映射的抽象层
+ platform.c/h: 平台API的抽象层
+ utils.c/h: 常用API实现
+ test.c: 测试