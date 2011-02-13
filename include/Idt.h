#ifndef __IDT_H__
#define __IDT_H__

#include <Kernel.h>

struct Idt_Entry
{
    u16 base_lo;
    u16 sel;
    u8 always0;
    u8 flags;
    u16 base_hi;
} __attribute__((packed));
typedef struct Idt_Entry Idt_Entry;

struct Idt_Pointer
{
    u16 limit;
    u32 base;
} __attribute__((packed));
typedef struct Idt_Pointer Idt_Pointer;

extern void Idt_load(); /* Idt.asm */

void idt_set_gate(u8, u64, u16, u8);
void idt_install();

#endif
