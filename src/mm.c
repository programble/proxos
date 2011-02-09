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

#include <mm.h>

memory_header *first_block = NULL;

bool mm_installed = false;

memory_header *init_prev_header = NULL;
void init_free_block(u32 base, u32 length)
{
    if (base < 0x100000)
        return;

    /* Block must be big enough to hold 1 byte (and a header) */
    if (length < HSIZE + sizeof(char))
        return;

    memory_header *header = (memory_header*) base;
    SET_FREE(header);
    header->size = length - HSIZE;
    header->next = NULL;

    if (!first_block)
        first_block = header;
    else
        init_prev_header->next = header;

    init_prev_header = header;
}

void mm_install(multiboot_header *multiboot)
{
    assert(multiboot->flags & (1 << 6), "No memory map");

    u32 kernel_start = (u32) &link_kernel_start;
    u32 kernel_end = (u32) &link_kernel_end;

    mmap_field *mmap = multiboot->mmap_addr;

    while ((unsigned) (mmap - multiboot->mmap_addr) < multiboot->mmap_length)
    {
        assert(mmap != NULL, "Invalid memory map");

        if (mmap->type == 1) /* Free? */
        {
            u32 mmap_end = mmap->base_addr + mmap->length;

            /* Block is completely covered by kernel */
            if (mmap->base_addr >= kernel_start && mmap_end <= kernel_end);
            /* Kernel covers beginning of block */
            else if (mmap->base_addr >= kernel_start && mmap->base_addr <= kernel_end)
                init_free_block(kernel_end, mmap->length + mmap->base_addr - kernel_end);
            /* Kernel covers end of block */
            else if (mmap_end >= kernel_start && mmap_end <= kernel_end)
                init_free_block(mmap->base_addr, kernel_start - mmap->base_addr);
            /* Kernel is inside the block */
            else if (kernel_start >= mmap->base_addr && kernel_end <= mmap_end)
            {
                init_free_block(mmap->base_addr, kernel_start - mmap->base_addr);
                init_free_block(kernel_end, mmap_end - kernel_end);
            }
            /* Kernel is not overlapping block */
            else
                init_free_block(mmap->base_addr, mmap->length);
        }
        mmap = (mmap_field*) ((u32) mmap + mmap->size + sizeof(mmap->size));
    }

    mm_installed = true;
}

void *malloc(u32 size)
{
    assert(mm_installed, "Memory Manager not installed");

    disable_interrupts();

    assert(size > 0, "Cannot allocate 0 size block");

    for (memory_header *block = first_block; block; block = block->next)
    {
        CHECK(block);

        if (USED(block))
            continue;

        /* Block is just the right size */
        if (block->size == size)
        {
            SET_USED(block);
            enable_interrupts();
            return START(block);
        }
        /* Block is bigger, and big enough to be split */
        else if (block->size > size + HSIZE + sizeof(char))
        {
            memory_header *leftover = (memory_header*) ((u32) START(block) + size);
            SET_FREE(leftover);
            leftover->size = block->size - size - HSIZE;
            leftover->next = block->next;

            block->next = leftover;
            block->size = size;
            SET_USED(block);
            enable_interrupts();
            return START(block);
        }
    }
    panic("Out of memory");
    return NULL;
}

void merge()
{
    memory_header *block = first_block;
    while (block && block->next)
    {
        CHECK(block);
        CHECK(block->next);

        /* Make sure block->next physically follows block */
        if ((memory_header*) ((u32) START(block) + block->size) != block->next)
        {
            block = block->next;
            continue;
        }

        if (FREE(block) && FREE(block->next))
        {
            block->size += block->next->size + HSIZE;
            block->next->magic = 0;
            block->next = block->next->next;
            continue;
        }

        block = block->next;
    }
}

void free(void *block)
{
    assert(mm_installed, "Memory Manager not installed");

    disable_interrupts();

    memory_header *header = HEADER(block);
    CHECK(header);
    SET_FREE(header);

    merge();

    enable_interrupts();
}

void *realloc(void *old, u32 size)
{
    assert(mm_installed, "Memory Manager not installed");

    disable_interrupts();

    void *new = malloc(size);
    memory_header *header = HEADER(old);
    CHECK(header);
    memcpy(new, old, (size < header->size) ? size : header->size);
    free(old);

    enable_interrupts();
    return new;
}
