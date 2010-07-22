/*  Copyright 2010 Curtis McEnroe <programble@gmail.com>
 *
 *  This file is part of Proxos.
 *
 *  Proxos is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 * 
 *  Proxos is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Proxos.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <system.h>

u8 inportb(u16 port)
{
    u8 rv;
    __asm__ __volatile__ ("inb %1, %0" : "=a" (rv) : "dN" (port));
    return rv;
}

void outportb(u16 port, u8 data)
{
    __asm__ __volatile__ ("outb %1, %0" : : "dN" (port), "a" (data));
}

int strlen(char *str)
{
    int i;
    for (i = 0; str[i] != 0x0; i++);
    return i;
}

int strcmp(char *s1, char *s2)
{
    for (; *s1 == *s2; s1++, s2++)
        if (*s1 == 0x0)
            return 0;
    return *(u8*) s1 < *(u8*) s2 ? -1 : 1;
}

u8 *memcpy(u8 *dest, u8 *src, int count)
{
    for (int i = 0; i < count; i++)
        dest[i] = src[i];
    return dest;
}

u8 *memset(u8 *dest, u8 val, int count)
{
    for (int i = 0; i < count; i++)
        dest[i] = val;
    return dest;
}

u16 *memsetw(u16 *dest, u16 val, int count)
{
    for (int i = 0; i < count; i++)
        dest[i] = val;
    return dest;
}

void halt()
{
    __asm__ __volatile__("cli");
    __asm__ __volatile__("hlt");
}

u8 in_panic = 0;

void _panic(char *message, char *file, char *line)
{
    if (in_panic)
    {
        /* Recursive panic, just halt */
        halt();
    }
    in_panic = 1;
    /* Dump a coredump */
    /*coredump();*/
    puts("\n");
    set_text_color_foreground(red);
    puts(":: When in ");
    set_text_color_foreground(light_red);
    puts("panic");
    set_text_color_foreground(red);
    puts(", fear or doubt, run in circles, scream and shout\n ");
    puts(message);
    puts("\n File: ");
    puts(file);
    puts("\n Line: ");
    puts(line);
    puts("\n");
    halt();
}

char *int_to_str(u32 n, u32 radix)
{
    switch (radix)
    {
    case 2:
    {
        char numerals[2] = "01";
        int i = 33;
        static char buf[33];
        do
        {
            i--;
            buf[i] = numerals[n % 2];
        } while ((n /= 2) > 0 && i);
        buf[33] = 0;
        return (char*)(buf + i);
    } break;
    case 10:
    {
        char numerals[10] = "0123456789";
        int i = 11;
        static char buf[11];
        do
        {
            i--;
            buf[i] = numerals[n % 10];
        } while ((n /= 10) > 0 && i);
        buf[11] = 0;
        return (char*)(buf + i);
    } break;
    case 16:
    {
        char numerals[16] = "0123456789ABCDEF";
        int i = 9;
        static char buf[9];
        do
        {
            i--;
            buf[i] = numerals[n % 16];
        } while ((n /= 16) > 0 && i);
        buf[9] = 0;
        return (char*)(buf + i);
    } break;
    }
    return NULL;
}
