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

#define MM_MAGIC (0xE1AFE909)

memory_header *first_block = NULL;

bool mm_installed = false;

void add_free_block(u32 base, u32 length);

void mm_init(multiboot_header *multiboot)
{
    puts(":: Initializing memory manager\n");
    
    /* Make sure multiboot has mmap_ fields */
    assert(multiboot->flags & (1 << 6));

    mmap_field *mmap = multiboot->mmap_addr;

    u32 kernel_start = (u32) &link_kernel_entry;
    u32 kernel_end = (u32) &link_kernel_end;

    /* Loop through each mmap */
    while ((unsigned) (mmap - multiboot->mmap_addr) < multiboot->mmap_length)
    {
        assert(mmap != NULL);
        
        /* Free block? */
        if (mmap->type == 1)
        {
            u32 mmap_end = mmap->base_addr + mmap->length;
            /* This block is covered by the kernel */
            if (mmap->base_addr >= kernel_start && mmap_end <= kernel_end)
                continue;

            /* Kernel coverse beginning of block */
            else if (mmap->base_addr >= kernel_start && mmap->base_addr <= kernel_end)
            {
                add_free_block(kernel_end, mmap->length + mmap->base_addr - kernel_end);
            }

            /* Kernel covers end of block */
            else if (mmap_end >= kernel_start && mmap_end <= kernel_end)
            {
                add_free_block(mmap->base_addr, kernel_start - mmap->base_addr);
            }

            /* Kernel is in the block */
            else if (kernel_start >= mmap->base_addr && kernel_end <= mmap_end)
            {
                add_free_block(mmap->base_addr, kernel_start - mmap->base_addr);
                add_free_block(kernel_end, mmap_end - kernel_end);
            }

            /* Kernel is not in block */
            else
            {
                add_free_block(mmap->base_addr, mmap->length);
            }
        }
        mmap = (mmap_field*) ((unsigned long) mmap + mmap->size + sizeof(mmap->size));
    }
    puts("  > 0x");
    puts(int_to_str(free_count(), 16));
    puts(" bytes free\n");
    mm_installed = true;
}

memory_header *previous_free_block;
void add_free_block(u32 base, u32 length)
{
    /* Nothing before 0x100000 */
    if (base < 0x100000)
        return;
    /* Block is too small */
    if (length < sizeof(memory_header) + sizeof(char))
        return;

    puts("  > Adding free block at 0x");
    puts(int_to_str(base, 16));
    puts(" size 0x");
    puts(int_to_str(length, 16));
    puts("\n");

    memory_header *header = (memory_header*)base;
    header->start = base + sizeof(memory_header);
    header->size = length - sizeof(memory_header);
    header->magic = MM_MAGIC;
    header->next = NULL;
    header->free = true;

    /* Make sure everything is ok */
    assert(header->start + header->size == base + length);

    if (!first_block)
    {
        first_block = header;
    }
    else
    {
        previous_free_block->next = header;
    }

    previous_free_block = header;
}

void coredump()
{
    puts("\n");
    puts("BLOCK\tHEADER\t\tSTART\t\tEND\t\tSIZE\n");
    puts("-----------------------------------------------------------------\n");
    memory_header *block = first_block;
    while (block)
    {
        puts(block->free ? "FREE" : "USED");
        puts("\t0x");
        puts(int_to_str((u32) block, 16));
        puts("\t0x");
        puts(int_to_str(block->start, 16));
        puts("\t0x");
        puts(int_to_str(block->start + block->size, 16));
        puts("\t0x");
        puts(int_to_str(block->size, 16));
        puts("\n");
        block = block->next;
    }
}

u32 free_count()
{
    u32 count = 0;
    memory_header *block = first_block;
    while (block)
    {
        if (block->free)
        {
            count += block->size;
        }
        block = block->next;
    }
    return count;
}

void *malloc(u32 size)
{
    assert(mm_installed);

    /* Disable interrupts */
    __asm__("cli");

    assert(size > 0);

    for (memory_header *block = first_block; block; block = block->next)
    {
        assert(block->magic == MM_MAGIC);

        if (block->free == false)
            continue;

        if (block->size == size)
        {
            block->free = false;
            __asm__("sti");
            return (void*) block->start;
        }
        else if (block->size > size)
        {
            memory_header *leftover = (memory_header*) (block->start + size);
            leftover->size = block->size - size - sizeof(memory_header);
            leftover->start = block->start + size + sizeof(memory_header);
            leftover->free = true;
            leftover->magic = MM_MAGIC;
            leftover->next = block->next;

            block->next = leftover;
            block->size = size;
            block->free = false;
            __asm__("sti");
            return (void*) block->start;
        }
    }
    panic("Out of memory");
    return NULL;
}

void collapse(memory_header *block)
{
    memory_header *next;
    for (next = block->next; next && next->free; next = next->next)
    {
        block->size += next->size + sizeof(memory_header);
    }
    block->next = next->next;
}

void merge_sweep()
{
    for (memory_header *block = first_block; block; block = block->next)
    {
#if false        
        assert(block->magic == MM_MAGIC);
        if (block->free == false)
            continue; /* Skip used blocks */
        if (!block->next)
            break; /* There are no more blocks */
        if (block->next->free)
        {
            assert(block->next->magic == MM_MAGIC);
            /* Two free blocks beside eachother should merge */
            block->size = block->size + block->next->size + sizeof(memory_header);
            block->next = block->next->next;
        }
#endif
        assert(block->magic == MM_MAGIC);
        if (block->free == false)
            continue; /* Skip used blocks */
        if (!block->next)
            break; /* There are no more blocks */
        if (block->next->free)
            collapse(block);
    }
}

void free(void *memory)
{
    assert(mm_installed);

    __asm__("cli");

    memory_header *header = (memory_header*) (memory - sizeof(memory_header));
    assert(header->magic == MM_MAGIC);
    assert(header->start == (u32) memory);
    assert(header->free == false);
    header->free = true;

    merge_sweep();
    /*collapse(header);*/

    __asm__("sti");
}

void *realloc(void *old, u32 size)
{
    __asm__("cli");
    void *new = malloc(size);
    memory_header *header = (memory_header*) (old - sizeof(memory_header));
    assert(header->magic == MM_MAGIC);
    memcpy(new, old, header->size);
    free(old);
    __asm__("sti");
    return new;
}
