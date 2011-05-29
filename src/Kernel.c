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

noreturn Kernel_reboot()
{
    u8 x = 0x02;
    while ((x & 0x02) != 0)
        x = Kernel_inportb(0x64);
    Kernel_outportb(0x64, 0xFE);
    while (true)
        Kernel_halt();
}

Bool recursivePanic = false;
noreturn Kernel__panic(const String message, const String function, const String file, const String line)
{
    Kernel_disableInterrupts();
    if (recursivePanic)
        Kernel_halt();
    recursivePanic = true;
    Terminal_setForegroundColor(Terminal_Color_red, true);
    Text_printString("\n\n === PANIC ===\n\n");
    Text_printString(message);
    Text_printString("\n\n");
    Text_printString(function);
    Text_printString("@");
    Text_printString(file);
    Text_printString(":");
    Text_printString(line);
    Text_printString("\n\n");
    Memory_headerDump();
    Text_printString("\n");
    Threading_threadDump();
    Text_printString("\nPress the any key to reboot...");
    u8 firstScancode = Kernel_inportb(0x60);
    while (Kernel_inportb(0x60) == firstScancode);
    firstScancode = Kernel_inportb(0x60);
    while (true)
    {
        u8 scancode = Kernel_inportb(0x60);
        if (!(scancode & 0x80) && scancode != firstScancode)
            Kernel_reboot();
    }
}

void testA()
{
    for (int i = 0; i < 2000; i++)
    {
        Text_printString("A");
        for (int j = 0; j < 50000; j++);
        Memory_free(Memory_allocate(2));
    }
}

void testB()
{
    for (int i = 0; i < 2000; i++)
    {
        Text_printString("B");
        for (int j = 0; j < 50000; j++);
        Memory_free(Memory_allocate(3));
    }
}

void testC()
{
    for (int i = 0; i < 2000; i++)
    {
        Text_printString("C");
        for (int j = 0; j < 50000; j++);
    }
}

noreturn forkBomb()
{
    while (true)
        Threading_fork(forkBomb);
}

void testSleep()
{
    Threading_sleep(5000);
    Text_printString("\nDone sleeping\n");
}

noreturn Kernel_main(multiboot_header *multiboot, u32 magic)
{
    Kernel_assert(magic == MULTIBOOT_BOOTLOADER_MAGIC, "Invalid bootloader magic");
    Kernel_multiboot = multiboot;
    Init_initialize(Init_Component_serial);
    Init_initialize(Init_Component_terminal);
    
    Init_initialize(Init_Component_gdt);
    Init_initialize(Init_Component_idt);
    Init_initialize(Init_Component_isr);
    Init_initialize(Init_Component_irq);
    Kernel_enableInterrupts();

    Init_initialize(Init_Component_memory);
    Init_initialize(Init_Component_time);
    Init_initialize(Init_Component_keyboard);
    Init_initialize(Init_Component_threading);
    
    Text_printString("\nProxos Kernel\n Version " VERSION "\n  " COMPILED "\n  " COMPILER "\n Booted with ");
    Text_printString(multiboot->bootloader_name);
    Text_printString(" ");
    Text_printString(multiboot->cmdline);
    Text_printString("\n 0x");
    Text_printString(String_formatInt((u32) &Kernel_linkStart, 16));
    Text_printString(" - 0x");
    Text_printString(String_formatInt((u32) &Kernel_linkEnd, 16));
    Text_printString("\n\n");

    String lastInput = Memory_allocate(1);
    while (true)
    {
        Text_printString("$ ");
        String input = Keyboard_getString(true);

        if (String_equals(input, "\x90"))
        {
            Memory_free(input);
            input = lastInput;
        }
        
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
            Threading_fork(testC)->priority = 40;
        }
        else if (String_equals(input, "threaddump"))
            Threading_threadDump();
        else if (String_equals(input, "forkbomb"))
            Threading_fork(forkBomb);
        else if (String_equals(input, "sleeptest"))
            Threading_fork(testSleep);
        else if (String_equals(input, "sleep"))
            Threading_sleep(1000);
        else if (String_equals(input, "reboot"))
            Kernel_reboot();
        else if (String_equals(input, "help"))
            Text_printString("panic, headerdump, beep, forktest, threaddump, forkbomb, sleeptest, reboot\n");
        else
            Text_printString("?\n");

        if (lastInput != input)
            Memory_free(lastInput);
        lastInput = input;
    }
}
