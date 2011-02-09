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

#define STRING(x) #x
#define MACRO_STRING(x) STRING(x)

#define VERSION "0.1.0"
#define COMPILED __DATE__

#define panic(message) _panic(message, __FILE__, MACRO_STRING(__LINE__))
#define assert(value) if (!(value)) _panic("Assert failed: " #value, __FILE__, MACRO_STRING(__LINE__))

#define NULL ((void*)0)

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;
typedef unsigned long long u64;
typedef signed long long s64;

typedef enum
{
    false,
    true
} bool;

u8 inportb(u16);
void outportb (u16, u8);

int strlen(char*);
int strcmp(char*, char*);

u8 *memcpy(u8*, u8*, int);
u8 *memset(u8*, u8, int);
u16 *memsetw(u16*, u16, int);

void halt();

void _panic();

typedef struct
{
    u32 size;
    u32 base_addr;
    u32 base_addr_high;
    u32 length;
    u32 length_high;
    u32 type;
} mmap_field;

/* Multiboot header */
typedef struct
{
    u32 flags;
    u32 mem_lower;
    u32 mem_upper;
    u32 boot_device;
    char *cmdline;
    u32 mods_count;
    u32 *mods_addr;
    u32 syms[4];
    u32 mmap_length;
    mmap_field *mmap_addr;
    u32 drives_length;
    u32 *drives_addr;
    u32 config_table;
    char *boot_loader_name;
    u32 apm_table;
    u32 vbe_control_info;
    u32 vbe_mode_info;
    u16 vbe_mode;
    u16 vbe_interface_seg;
    u16 vbe_interface_off;
    u16 vbe_interface_len;
    u64 framebuffer_addr;
    u32 framebuffer_pitch;
    u32 framebuffer_width;
    u32 framebuffer_height;
    u8 framebuffer_bpp;
    u8 framebuffer_type;
    u8 color_info[6];
} multiboot_header;

char *int_to_str(u32, u32);

extern void *link_kernel_start;
extern void *link_kernel_end;

#include <mm.h>
#include <tty.h>

#endif
