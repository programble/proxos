#ifndef __IDT_H__
#define __IDT_H__

#include <Kernel.h>

typedef struct
{
    u16 baseLow;
    u16 sel;
    u8 always0;
    u8 flags;
    u16 baseHigh;
} Idt_Entry;

struct Idt_Pointer
{
    u16 limit;
    u32 base;
} __attribute__((packed));
typedef struct Idt_Pointer Idt_Pointer;

extern void Idt_load(); /* Idt.asm */

void Idt_setGate(u8 num, u64 base, u16 sel, u8 flags);
void Idt_setLimit(u16 limit);
Bool Idt_initialize();

#endif
