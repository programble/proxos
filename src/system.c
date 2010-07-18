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

u8 *memcpy(u8 *dest, const u8 *src, int count)
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
