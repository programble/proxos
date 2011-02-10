#ifndef __STRING_H__
#define __STRING_H__

#include <kernel.h>

u8 *memcpy(u8 *dest, u8 *src, u32 count);
u8 *memset(u8 *dest, u8 val, u32 count);
u16 *memsetw(u16 *dest, u16 val, u32 count);

u32 strlen(const string);

#endif
