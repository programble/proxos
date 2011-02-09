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

#ifndef __MULTIBOOT_H__
#define __MULTIBOOT_H__

#define MULTIBOOT_HEADER_MAGIC 0x1BADB002
#define MULTIBOOT_BOOTLOADER_MAGIC 0x2BADB002

typedef struct
{
    u32 size;
    u32 base_addr;
    u32 base_addr_high;
    u32 length;
    u32 length_high;
    u32 type;
} mmap_field;

typedef struct module
{
    u32 start;
    u32 end;
    string name;
    bool reserved;
} module;

typedef struct
{
    u32 flags;
    u32 mem_lower;
    u32 mem_upper;
    u32 boot_device;
    string cmdline;
    u32 mods_count;
    module *mods_addr;
    u32 syms[4];
    u32 mmap_length;
    mmap_field *mmap_addr;
    u32 drives_length;
    u32 *drives_addr;
    u32 config_table;
    string bootloader_name;
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

#endif
