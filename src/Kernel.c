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

void uptimeThread()
{
    extern u32 Time_ticks;
    while (true)
    {
        u8 x = Terminal_getCursorX();
        u8 y = Terminal_getCursorY();
        Terminal_setCursorPosition(70, 0);
        Terminal_putString(String_formatInt(Time_ticks / 1000, 10));
        Terminal_setCursorPosition(x, y);
        Time_sleep(1000);
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
        else if (String_equals(input, "uptime"))
            Threading_fork(uptimeThread);
        else if (String_equals(input, "help"))
            Text_putString("panic, headerdump, beep, forktest, threaddump\n");
        
        Memory_free(input);
    }
}
