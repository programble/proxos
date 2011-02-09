/*  Copyright 2010 Curtis McEnroe <programble@gmail.com>
 *
 *  This file is part of AmusOS.
 *
 *  AmusOS is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 * 
 *  AmusOS is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with AmusOS.  If not, see <http://www.gnu.org/licenses/>.
 */

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
