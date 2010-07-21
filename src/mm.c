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

#include <mm.h>

memory_header *first_block = NULL;

bool mm_installed = false;

memory_header *init_prev_header = NULL;
void init_free_block(u32 base, u32 length)
{
    if (base < 0x100000)
        return;
    /* Block must be at least capable of holding 1 byte (10 bytes total) */
    if (length < HSIZE + sizeof(char))
        return;

    puts("  > Initializing free block at 0x");
    puts(int_to_str(base, 16));
    puts(" size 0x");
    puts(int_to_str(length, 16));
    puts("\n");

    memory_header *header = (memory_header*) base;
    header->magic = MM_MAGIC;
    header->size = length - HSIZE;
    header->free = 1;
    header->next = NULL;

    if (!first_block)
        first_block = header;
    else
        init_prev_header->next = header;

    init_prev_header = header;
}

void mm_init(multiboot_header *multiboot)
{
    puts(":: Initializing memory manager\n");

    assert(multiboot->flags & (1 << 6));

    u32 kernel_start = (u32) &link_kernel_entry;
    u32 kernel_end = (u32) &link_kernel_end;

    mmap_field *mmap = multiboot->mmap_addr;

    while ((unsigned) (mmap - multiboot->mmap_addr) < multiboot->mmap_length)
    {
        assert(mmap != NULL);

        if (mmap->type == 1) /* Free? */
        {
            u32 mmap_end = mmap->base_addr + mmap->length;
            
            /* Block is completely covered by kernel */
            if (mmap->base_addr >= kernel_start && mmap_end <= kernel_end)
                continue;
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

    puts("  > 0x");
    puts(int_to_str(free_count(), 16));
    puts(" bytes free\n");
    mm_installed = true;
}

void *malloc(u32 size)
{
    assert(mm_installed);

    __asm__("cli");

    assert(size > 0);

    for (memory_header *block = first_block; block; block = block->next)
    {
        check(block);

        if (!block->free)
            continue;

        if (block->size == size)
        {
            block->free = 0;
            __asm__("sti");
            return start(block);
        }
        else if (block->size > size + HSIZE + sizeof(char))
        {
            memory_header *leftover = (memory_header*) (start(block) + size);
            leftover->magic = MM_MAGIC;
            leftover->size = block->size - size - HSIZE;
            leftover->free = 1;
            leftover->next = block->next;

            block->next = leftover;
            block->size = size;
            block->free = 0;
            __asm__("sti");
            return start(block);
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
        check(block);
        check(block->next);

        /* Make sure block->next physically follows block */
        if ((memory_header*) (start(block) + block->size) != block->next)
        {
            block = block->next;
            continue;
        }

        if (block->free && block->next->free)
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
    assert(mm_installed);
    __asm__("cli");

    memory_header *header = header(block);
    check(header);
    header->free = 1;

    merge();

    __asm__("sti");
}

void *realloc(void *old, u32 size)
{
    assert(mm_installed);
    __asm__("cli");
    void *new = malloc(size);
    memory_header *header = header(old);
    check(header);
    memcpy(new, old, header->size);
    free(old);
    __asm__("sti");
    return new;
}

u32 free_count()
{
    u32 count = 0;
    for (memory_header *block = first_block; block; block = block->next)
    {
        if (block->free)
            count += block->size;
    }
    return count;
}

void coredump()
{
    puts("\nBLOCK\tHEADER\t\tSTART\t\tEND\t\tSIZE\n-----------------------------------------------------------------\n");
    for (memory_header *block = first_block; block; block = block->next)
    {
        puts(block->free ? "FREE" : "USED");
        puts("\t0x");
        puts(int_to_str((u32) block, 16));
        puts("\t0x");
        puts(int_to_str((u32) start(block), 16));
        puts("\t0x");
        puts(int_to_str((u32) start(block) + block->size, 16));
        puts("\t0x");
        puts(int_to_str(block->size, 16));
        puts("\n");
    }
}

void headerdump()
{
    puts("\n");
    for (memory_header *block = first_block; block; block = block->next)
    {
        puts("0x");
        puts(int_to_str((u32) block, 16));
        puts(" magic=0x");
        puts(int_to_str(block->magic, 16));
        puts(" size=0x");
        puts(int_to_str(block->size, 16));
        puts(" free=0x");
        puts(int_to_str(block->free, 16));
        puts(" next=0x");
        puts(int_to_str((u32) block->next, 16));
        puts("\n");
    }
}
