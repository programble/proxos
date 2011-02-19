#ifndef __ISR_H__
#define __ISR_H__

#include <Kernel.h>

typedef struct
{
    u32 gs, fs, es, ds;
    u32 edi, esi, ebp, esp, ebx, edx, ecx, eax;
    u32 interruptNumber, errorCode;
    u32 eip, cs, eflags, useresp, ss;
} Isr_Registers;

Bool Isr_initialize();

#endif
