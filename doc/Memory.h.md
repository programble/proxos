# Memory.h

Functions relating to the allocation and freeing of heap memory.

The memory manager is currently a flat-memory model first-fit
manager. Each block of memory is preceded by a memory header which
contains information about the memory block. Each header holds the
free/used flag of its block, the size of its block, and pointers to
the previous and next memory headers. Merging free blocks after a call
to `Memory_free` takes advantage of this doubly-linked list.

## `void *Memory_allocate(u32 size)`

Allocates a block of memory of size `size` on the
heap.

Memory is allocated on a first-fit basis. The first free memory block
found that is either the same size as the requested size or larger
will be allocated. In the latter case, the memory block will be split
into one memory block of the requested size and another block of the
remaining size.

When a suitable block is found, its header's flag will be set to used
and the memory block will be returned.

Call `Memory_free` on the returned block to free it for re-use.

## `void Memory_free(void *memory)`

Frees a block of memory `memory` on the heap.

The block's header's flag will be set to free and will then be merged
with any adjacent free blocks.

## `void *Memory_reallocate(void *memory, u32 size)`

Allocates a new block of memory of size `size` by
calling `Memory_allocate`, copies the contents of `memory` to the new
block of memory, then calls `Memory_free` on `memory`. It returns the
newly allocated block of memory.

## `void Memory_headerDump()`

Dumps a list of all memory headers to any text
output available. It outputs each header's address, magic number/flag
field, the addresses of its previous and next headers, and the size of
its memory block.

## Technical

### Flag field

The flag field in memory headers doubles as a magic number that is
checked every time a header is used to detect memory header
corruption. Also, the other 31 bits would have gone to waste
otherwise. Only the least significant bit is ever flipped. It
represents the free/used state of the memory block, 1 being free and 0
being used.

### Merging

When `Memory_free` merges memory blocks, it looks at the previous and
next headers of the block that has just been freed, and merges them by
adding their sizes if they are physically adjacent.

### Initialization

When initialized, the memory manager adds free blocks of memory as
listed in the multiboot info, making sure that the memory does not
overlap with the kernel in any way.

### Allocating

Note that in order for a block of memory to be split, its size needs
to be big enough to be split between the requested size and another
header with a block big enough for one byte.
