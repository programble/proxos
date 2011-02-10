#include <string.h>

u8 *memcpy(u8 *dest, u8 *src, u32 count)
{
    for (u32 i = 0; i < count; i++)
        dest[i] = src[i];
    return dest;
}

u8 *memset(u8 *dest, u8 val, u32 count)
{
    for (u32 i = 0; i < count; i++)
        dest[i] = val;
    return dest;
}

u16 *memsetw(u16 *dest, u16 val, u32 count)
{
    for (u32 i = 0; i < count; i++)
        dest[i] = val;
    return dest;
}

u32 strlen(const string s)
{
    u32 i;
    for (i = 0; s[i] != 0; i++);
    return i;
}
