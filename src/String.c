#include <String.h>

u8 *String_copy(u8 *dest, u8 *src, u32 count)
{
    for (u32 i = 0; i < count; i++)
        dest[i] = src[i];
    return dest;
}

u8 *String_set(u8 *dest, u8 val, u32 count)
{
    for (u32 i = 0; i < count; i++)
        dest[i] = val;
    return dest;
}

u16 *String_setWord(u16 *dest, u16 val, u32 count)
{
    for (u32 i = 0; i < count; i++)
        dest[i] = val;
    return dest;
}

u32 String_length(const String s)
{
    u32 i;
    for (i = 0; s[i] != 0; i++);
    return i;
}

Bool String_equals(const String a, const String b)
{
    for (; *a == *b; a++, b++)
        if (*a == 0)
            return true;
    return false;
}

String String_formatInt(u32 n, u32 radix)
{
    char numerals[16] = "0123456789ABCDEF";
    static char s[34];
    s[33] = 0;
    u32 i = 33;
    do
    {
        i--;
        s[i] = numerals[n % radix];
    } while ((n /= radix) > 0 && i);
    return (String) (s + i);
}
