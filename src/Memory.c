#include <Memory.h>

#include <Multiboot.h>
#include <Text.h>
#include <String.h>

#define MEMORY_HEADER_MAGIC_FREE 0xE1AFE909
#define MEMORY_HEADER_MAGIC_USED 0xE1AFE908

#define Memory_Header_isFree(header) (header->magic & 0x1)
#define Memory_Header_blockStart(header) (void*) ((u32) header + sizeof(Memory_Header))
#define Memory_Header_fromBlock(block) (Memory_Header*) ((u32) block - sizeof(Memory_Header))
#define Memory_Header_assertMagic(header) Kernel_assert(header->magic >> 1 == 0x70D7F484, "Memory header corrupt")
#define Memory_Header_setFree(header) header->magic = MEMORY_HEADER_MAGIC_FREE
#define Memory_Header_setUsed(header) header->magic = MEMORY_HEADER_MAGIC_USED

Memory_Header *Memory_Header_first = NULL;

void Memory_Header_initialize(u32 base, u32 length)
{
    static Memory_Header *previousHeader = NULL;

    /* Don't use memory before the kernel */
    if (base < (u32) &Kernel_linkStart)
        return;

    /* The block must be big enough to hold 1 byte and the header */
    if (length < sizeof(Memory_Header) + sizeof(char))
        return;

    Memory_Header *header = (Memory_Header*) base;
    Memory_Header_setFree(header);
    header->size = length - sizeof(Memory_Header);
    header->next = NULL;

    if (!Memory_Header_first)
        Memory_Header_first = header;
    else
        previousHeader->next = header;

    previousHeader = header;
}

Bool Memory_initialize()
{
    Kernel_assert(Kernel_multiboot->flags & (1 << 6), "No memory map");

    u32 kernelStart = (u32) &Kernel_linkStart;
    u32 kernelEnd = (u32) &Kernel_linkEnd;

    mmap_field *mmap = Kernel_multiboot->mmap_addr;

    while ((u32) (mmap - Kernel_multiboot->mmap_addr) < Kernel_multiboot->mmap_length)
    {
        Kernel_assert(mmap != NULL, "Invalid memory map");

        if (mmap->type == 1)
        {
            u32 mmapEnd = mmap->base_addr + mmap->length;

            /* Block is covered by kernel */
            if (mmap->base_addr >= kernelStart && mmapEnd <= kernelEnd);
            /* Kernel covers beginning of block */
            else if (mmap->base_addr >= kernelStart && mmap->base_addr <= kernelEnd)
                Memory_Header_initialize(kernelEnd, mmap->length + mmap->base_addr - kernelEnd);
            /* Kernel covers end of block */
            else if (mmapEnd >= kernelStart && mmapEnd <= kernelEnd)
                Memory_Header_initialize(mmap->base_addr, kernelStart - mmap->base_addr);
            /* Kernel is inside block */
            else if (kernelStart >= mmap->base_addr && kernelEnd <= mmapEnd)
            {
                Memory_Header_initialize(mmap->base_addr, kernelStart - mmap->base_addr);
                Memory_Header_initialize(kernelEnd, mmapEnd - kernelEnd);
            }
            /* No touching! */
            else
                Memory_Header_initialize(mmap->base_addr, mmap->length);
        }
        mmap = (mmap_field*) ((u32) mmap + mmap->size + sizeof(mmap->size));
    }
    return true;
}

void *Memory_allocate(u32 size)
{
    Kernel_disableInterrupts();

    Kernel_assert(size > 0, "Cannot allocate 0 size block");

    for (Memory_Header *header = Memory_Header_first; header; header = header->next)
    {
        Memory_Header_assertMagic(header);

        if (!Memory_Header_isFree(header))
            continue;

        /* Block is just the right size */
        if (header->size == size)
        {
            Memory_Header_setUsed(header);
            Kernel_enableInterrupts();
            return Memory_Header_blockStart(header);
        }
        /* Block is bigger and can be split */
        else if (header->size > size + sizeof(Memory_Header) + sizeof(char))
        {
            Memory_Header *leftover = (Memory_Header*) ((u32) Memory_Header_blockStart(header) + size);
            Memory_Header_setFree(leftover);
            leftover->size = header->size - size - sizeof(Memory_Header);
            leftover->next = header->next;

            header->next = leftover;
            header->size = size;
            Memory_Header_setUsed(header);
            Kernel_enableInterrupts();
            return Memory_Header_blockStart(header);
        }
    }
    Kernel_panic("Out of memory");
    return NULL;
}

void Memory_free(void *memory)
{
    Kernel_disableInterrupts();

    Memory_Header *header = Memory_Header_fromBlock(memory);
    Memory_Header_assertMagic(header);
    Memory_Header_setFree(header);

    Kernel_enableInterrupts();
}

void *Memory_reallocate(void *memory, u32 size)
{
    Kernel_disableInterrupts();

    void *new = Memory_allocate(size);
    Memory_Header *header = Memory_Header_fromBlock(memory);
    Memory_Header_assertMagic(header);
    String_copy(new, memory, (size < header->size) ? size : header->size);
    Memory_free(memory);

    Kernel_enableInterrupts();
    return new;
}

void Memory_headerDump()
{
    Text_putString("Header\t\tMagic\t\tFree\tSize\t\tNext\n");
    for (Memory_Header *header = Memory_Header_first; header; header = header->next)
    {
        Text_putString("0x");
        Text_putString(String_formatInt((u32) header, 16));
        Text_putString("\t0x");
        Text_putString(String_formatInt(header->magic, 16));
        Text_putString("\t");
        Text_putString(String_formatInt(Memory_Header_isFree(header), 2));
        Text_putString("\t0x");
        Text_putString(String_formatInt(header->size, 16));
        Text_putString("\t\t0x");
        Text_putString(String_formatInt((u32) header->next, 16));
        Text_putString("\n");
    }
}
