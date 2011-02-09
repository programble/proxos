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

#ifndef __MM_H__
#define __MM_H__

#include <multiboot.h>
#include <string.h>

#define MM_FREE_MAGIC (0xE1AFE909)
#define MM_USED_MAGIC (0xE1AFE908)
#define HSIZE (sizeof(memory_header))
#define START(header) (void*) ((u32) header + HSIZE)
#define HEADER(block) (memory_header*) ((u32) block - HSIZE)
#define CHECK(block) assert(block->magic >> 1 == 0x70D7F484, "Memory block corrupt")
#define FREE(block) (block->magic == MM_FREE_MAGIC)
#define USED(block) (block->magic == MM_USED_MAGIC)
#define SET_FREE(block) block->magic = MM_FREE_MAGIC
#define SET_USED(block) block->magic = MM_USED_MAGIC

typedef struct memory_header
{
    u32 magic;
    u32 size;
    struct memory_header *next;
} memory_header;

void *malloc(u32);
void free(void*);
void *realloc(void*, u32);

void mm_install(multiboot_header*);

#endif
