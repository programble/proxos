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

#ifndef __MM_H__
#define __MM_H__

#include <system.h>

#define MM_MAGIC (0xE1AFE909)
#define HSIZE (sizeof(memory_header))
#define start(header) (void*) ((u32) header + HSIZE)
#define header(block) (memory_header*) ((u32) block - HSIZE)
#define check(block) assert(block->magic == MM_MAGIC)

typedef struct memory_header
{
    u32 magic;
    u32 size;
    u8 free;
    struct memory_header *next;
} memory_header;

void mm_init(multiboot_header*);
void *malloc(u32);
void free(void*);
void *realloc(void*, u32);

void coredump();
u32 free_count();

#endif
