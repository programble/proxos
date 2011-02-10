#include <kernel.h>
#include <multiboot.h>
#include <tty.h>

void kmain(multiboot_header *multiboot, u32 magic)
{
    assert(magic == MULTIBOOT_BOOTLOADER_MAGIC, "Invalid bootloader magic");

    tty_install();

    tty_puts("\nProxos Kernel\n " VERSION "\n " COMPILED "\n " COMPILER "\n");

    panic("Weeeeee!");
}
