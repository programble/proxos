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

#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#define panic(message) _panic(message, __FILE__, __LINE__)

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;
typedef unsigned long u64;
typedef signed long s64;

u8 inportb(u16);
void outportb (u16, u8);

int strlen(char*);

u8 *memcpy(u8*, u8*, int);
u8 *memset(u8*, u8, int);
u16 *memsetw(u16*, u16, int);

void halt();

void _panic();

#include <text.h>

#endif
