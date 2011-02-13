#include <Kernel.h>
#include <Multiboot.h>
#include <Init.h>
#include <Serial.h>
#include <Terminal.h>

u8 Kernel_inportb(u16 port)
{
    u8 ret;
    asm("inb %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}

void Kernel_outportb(u16 port, u8 data)
{
    __asm__ __volatile__("outb %1, %0" : : "dN" (port), "a" (data));
}

Bool recursivePanic = false;
void Kernel__panic(const String message, const String function, const String file, const String line)
{
    if (recursivePanic)
        Kernel_halt();
    recursivePanic = true;
    Terminal_putString("\n\n * Insert a clever panic phrase here *\n\n");
    Terminal_putString(message);
    Terminal_putString("\n\n");
    Terminal_putString(function);
    Terminal_putString("@");
    Terminal_putString(file);
    Terminal_putString(":");
    Terminal_putString(line);
    Kernel_halt();
}

void Kernel_main(multiboot_header *multiboot, u32 magic)
{
    Kernel_assert(magic == MULTIBOOT_BOOTLOADER_MAGIC, "Invalid bootloader magic");
    Init_initialize(Init_Driver_serial);
    Init_initialize(Init_Driver_terminal);
    Terminal_putString("\nProxos Kernel\n " VERSION "\n " COMPILED "\n " COMPILER "\n");
    Kernel_panic("Weee!");
}
