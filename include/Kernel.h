#ifndef __KERNEL_H__
#define __KERNEL_H__

/* Macro macros */
#define STRING(x) #x
#define MACRO_STRING(x) STRING(x)

/* Version stuffs */
#include <version.h>
#define COMPILED __DATE__ " " __TIME__

/* Inline Assembly macros */
#define asm(x) __asm__ __volatile__(x)
#define intel_asm(x) __asm__ __volatile__(".intel_syntax noprefix;" x ";.att_syntax prefix")

/* Types */
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;
typedef unsigned long long u64;
typedef signed long long s64;

/* Modifiers */
#define always_inline __attribute__((gnu_inline,always_inline)) static inline
#define deprecated __attribute__((deprecated))
#define noreturn __attribute__((noreturn)) void

#define String char*

typedef enum
{
    false,
    true
} Bool;

#define NULL ((void*) 0)

/* Linking things */
extern void *Kernel_linkStart;
extern void *Kernel_linkEnd;

/* Halt, panic, assert! */
#define Kernel_halt() asm("cli;hlt")

void Kernel__panic(const String message, const String function, const String file, const String line);

#define Kernel_panic(x) Kernel__panic(x, __PRETTY_FUNCTION__, __FILE__, MACRO_STRING(__LINE__))
#define Kernel_assert(x,m) if (!(x)) Kernel__panic("Assert failed: " #x "\n" m, __PRETTY_FUNCTION__, __FILE__, MACRO_STRING(__LINE__))

/* IO Ports */
u8 Kernel_inportb(u16 port);
void Kernel_outportb(u16 port, u8 data);

/* Interrupts */
#define Kernel_enableInterrupts() asm("sti")
#define Kernel_disableInterrupts() asm("cli")

/* Global access to multiboot */
#include <Multiboot.h>
multiboot_header *Kernel_multiboot;

/* Access to stack pointer */
extern void *Kernel_stackPointer;

#endif
