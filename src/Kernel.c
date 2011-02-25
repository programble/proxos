#include <Kernel.h>
#include <Multiboot.h>
#include <Init.h>
#include <Text.h>
#include <Terminal.h>
#include <String.h>
#include <Memory.h>
#include <Time.h>
#include <Keyboard.h>
#include <PCSpeaker.h>
#include <Threading.h>

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
    Kernel_disableInterrupts();
    if (recursivePanic)
        Kernel_halt();
    recursivePanic = true;
    Terminal_setForegroundColor(Terminal_Color_red, true);
    Text_putString("\n\n === PANIC ===\n\n");
    Text_putString(message);
    Text_putString("\n\n");
    Text_putString(function);
    Text_putString("@");
    Text_putString(file);
    Text_putString(":");
    Text_putString(line);
    Text_putString("\n\n");
    Memory_headerDump();
    Kernel_halt();
}

void testA()
{
    for (int i = 0; i < 2000; i++)
    {
        Text_putString("A");
        for (int j = 0; j < 50000; j++);
    }
}

void testB()
{
    for (int i = 0; i < 2000; i++)
    {
        Text_putString("B");
        for (int j = 0; j < 50000; j++);
    }
}

void Kernel_main(multiboot_header *multiboot, u32 magic)
{
    Kernel_assert(magic == MULTIBOOT_BOOTLOADER_MAGIC, "Invalid bootloader magic");
    Kernel_multiboot = multiboot;
    Init_initialize(Init_Driver_serial);
    Init_initialize(Init_Driver_terminal);
    
    Init_initialize(Init_Driver_gdt);
    Init_initialize(Init_Driver_idt);
    Init_initialize(Init_Driver_isr);
    Init_initialize(Init_Driver_irq);
    Kernel_enableInterrupts();

    Init_initialize(Init_Driver_memory);
    Init_initialize(Init_Driver_time);
    Init_initialize(Init_Driver_keyboard);
    Init_initialize(Init_Driver_threading);
    
    Text_putString("\nProxos Kernel\n Version " VERSION "\n  " COMPILED "\n  " COMPILER "\n Booted with ");
    Text_putString(multiboot->bootloader_name);
    Text_putString(" ");
    Text_putString(multiboot->cmdline);
    Text_putString("\n 0x");
    Text_putString(String_formatInt((u32) &Kernel_linkStart, 16));
    Text_putString(" - 0x");
    Text_putString(String_formatInt((u32) &Kernel_linkEnd, 16));
    Text_putString("\n\n");

    while (true)
    {
        Text_putString("$ ");
        String input = Keyboard_getString(true);

        if (String_equals(input, "panic"))
            Kernel_panic("Panic command");
        else if (String_equals(input, "headerdump"))
            Memory_headerDump();
        else if (String_equals(input, "beep"))
            PCSpeaker_beep(1000, 100);
        else if (String_equals(input, "forktest"))
        {
            Threading_fork(testA)->priority = 10;
            Threading_fork(testB)->priority = 5;
        }
        else if (String_equals(input, "threaddump"))
            Threading_threadDump();
        else if (String_equals(input, "windows"))
        {
            Terminal_clear();
            Time_sleep(100);
            Terminal_setBackgroundColor(Terminal_Color_blue, false);
            Terminal_clear();
            Terminal_putString("\n\n\n\n\n\n\n\n");
            for (int i = 0; i < 34; i++)
                Terminal_putChar(' ');
            Terminal_setColor(Terminal_Color_blue, false, Terminal_Color_gray, false);
            Terminal_putString(" Windows ");
            Terminal_setColor(Terminal_Color_gray, true, Terminal_Color_blue, false);
            Terminal_putString("\n\n     A fatal exception 0E has occured at 0028:C0011E36 in UXD UMM(01) +\n");
            Terminal_putString("     00010E36. The current application will be terminated.\n\n");
            Terminal_putString("     *  Press any key to terminate the current application.\n");
            Terminal_putString("     *  Press CTRL+ALT+DEL again to restart your computer. You will\n");
            Terminal_putString("        lose any unsaved information in all applications.\n\n");
            for (int i = 0; i < 27; i++)
                Terminal_putChar(' ');
            Terminal_putString("Press any key to continue ");
            Keyboard_getChar(false);
            Terminal_setColor(Terminal_Color_gray, false, Terminal_Color_black, false);
            Terminal_clear();
        }
        
        Memory_free(input);
    }
}
