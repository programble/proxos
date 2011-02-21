#ifndef __MEMORY_H__
#define __MEMORY_H__

#include <Kernel.h>

typedef struct Memory_Header
{
    u32 magic;
    u32 size;
    struct Memory_Header *next;
} Memory_Header;

void *Memory_allocate(u32 size);
void Memory_free(void *memory);
void *Memory_reallocate(void *memory, u32 size);

Bool Memory_initialize();

void Memory_headerDump();

#endif
