#ifndef __GDT_H__
#define __GDT_H__

#include <Kernel.h>

struct Gdt_Entry
{
    u16 limitLow;
    u16 baseLow;
    u8 baseMiddle;
    u8 access;
    u8 granularity;
    u8 baseHigh;
} __attribute__((packed));
typedef struct Gdt_Entry Gdt_Entry;

struct Gdt_Pointer
{
    u16 limit;
    u32 base;
} __attribute__((packed));
typedef struct Gdt_Pointer Gdt_Pointer;

extern void Gdt_flush(); /* Gdt.asm */

void Gdt_setGate(int num, u64 base, u64 limit, u8 access, u8 gran);
Bool Gdt_initialize();

#endif
