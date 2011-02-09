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

#ifndef __IDT_H__
#define __IDT_H__

#include <string.h>

struct idt_entry
{
    u16 base_lo;
    u16 sel;
    u8 always0;
    u8 flags;
    u16 base_hi;
} __attribute__((packed));
typedef struct idt_entry idt_entry;

struct idt_ptr
{
    u16 limit;
    u32 base;
} __attribute__((packed));
typedef struct idt_ptr idt_ptr;

extern void idt_load(); /* idt.asm */

void idt_set_gate(u8, u64, u16, u8);
void idt_install();

#endif
