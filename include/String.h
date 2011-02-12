#ifndef __STRING_H__
#define __STRING_H__

#include <Kernel.h>

u8 *String_copy(u8 *dest, u8 *src, u32 count);
u8 *String_set(u8 *dest, u8 val, u32 count);
u16 *String_setWord(u16 *dest, u16 val, u32 count);

u32 String_length(const String);

#endif
