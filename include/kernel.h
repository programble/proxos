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

#define string char*

typedef enum
{
    false,
    true
} bool;

#define NULL ((void*) 0)

/* Linking things */
extern void *link_kernel_start;
extern void *link_kernel_end;

/* Halt, panic, assert! */
#define halt() asm("cli;hlt")

void _panic(const string, const string, const string, const string);

#define panic(x) _panic(x, __PRETTY_FUNCTION__, __FILE__, MACRO_STRING(__LINE__))
#define assert(x,m) if (!(x)) _panic("Assert failed: " #x "\n" m, __PRETTY_FUNCTION__, __FILE__, MACRO_STRING(__LINE__))

/* IO Ports */
u8 inportb(u16);
void outportb(u16, u8);

/* Interrupts */
#define enable_interrupts() asm("sti")
#define disable_interrupts() asm("cli")

#endif
