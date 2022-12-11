#include "platform.h"
#include "bytebuffer.h"
#ifndef SHAREDMEMORY_MAPPING_H
#define SHAREDMEMORY_MAPPING_H
PMAPPING_KEY CreateKey(const char* mapping_name);
UINT32 CreateOrOpenMemoryMapping(PMAPPING_KEY mapping_key, INT32 size);
UINT32 MappingMemory(PMAPPING_KEY mapping_key);
UINT32 EndMappingMemory(PMAPPING_KEY mapping_key);
UINT32 CloseMemoryMapping(PMAPPING_KEY mapping_key);
#endif //SHAREDMEMORY_MAPPING_H
