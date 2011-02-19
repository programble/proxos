#include <Isr.h>

#include <Idt.h>

/* Isr.asm */
extern void Isr_isr0();
extern void Isr_isr1();
extern void Isr_isr2();
extern void Isr_isr3();
extern void Isr_isr4();
extern void Isr_isr5();
extern void Isr_isr6();
extern void Isr_isr7();
extern void Isr_isr8();
extern void Isr_isr9();
extern void Isr_isr10();
extern void Isr_isr11();
extern void Isr_isr12();
extern void Isr_isr13();
extern void Isr_isr14();
extern void Isr_isr15();
extern void Isr_isr16();
extern void Isr_isr17();
extern void Isr_isr18();
extern void Isr_isr19();
extern void Isr_isr20();
extern void Isr_isr21();
extern void Isr_isr22();
extern void Isr_isr23();
extern void Isr_isr24();
extern void Isr_isr25();
extern void Isr_isr26();
extern void Isr_isr27();
extern void Isr_isr28();
extern void Isr_isr29();
extern void Isr_isr30();
extern void Isr_isr31();

const String Isr_faultTable[] =
{
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",

    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",

    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",

    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

void Isr_handler(Isr_Registers *r)
{
    if (r->interruptNumber < 32)
        Kernel_panic(Isr_faultTable[r->interruptNumber]);
}

Bool Isr_initialize()
{
    Idt_setGate(0, (unsigned) Isr_isr0, 0x08, 0x8E);
    Idt_setGate(1, (unsigned) Isr_isr1, 0x08, 0x8E);
    Idt_setGate(2, (unsigned) Isr_isr2, 0x08, 0x8E);
    Idt_setGate(3, (unsigned) Isr_isr3, 0x08, 0x8E);
    Idt_setGate(4, (unsigned) Isr_isr4, 0x08, 0x8E);
    Idt_setGate(5, (unsigned) Isr_isr5, 0x08, 0x8E);
    Idt_setGate(6, (unsigned) Isr_isr6, 0x08, 0x8E);
    Idt_setGate(7, (unsigned) Isr_isr7, 0x08, 0x8E);

    Idt_setGate(8, (unsigned) Isr_isr8, 0x08, 0x8E);
    Idt_setGate(9, (unsigned) Isr_isr9, 0x08, 0x8E);
    Idt_setGate(10, (unsigned) Isr_isr10, 0x08, 0x8E);
    Idt_setGate(11, (unsigned) Isr_isr11, 0x08, 0x8E);
    Idt_setGate(12, (unsigned) Isr_isr12, 0x08, 0x8E);
    Idt_setGate(13, (unsigned) Isr_isr13, 0x08, 0x8E);
    Idt_setGate(14, (unsigned) Isr_isr14, 0x08, 0x8E);
    Idt_setGate(15, (unsigned) Isr_isr15, 0x08, 0x8E);

    Idt_setGate(16, (unsigned) Isr_isr16, 0x08, 0x8E);
    Idt_setGate(17, (unsigned) Isr_isr17, 0x08, 0x8E);
    Idt_setGate(18, (unsigned) Isr_isr18, 0x08, 0x8E);
    Idt_setGate(19, (unsigned) Isr_isr19, 0x08, 0x8E);
    Idt_setGate(20, (unsigned) Isr_isr20, 0x08, 0x8E);
    Idt_setGate(21, (unsigned) Isr_isr21, 0x08, 0x8E);
    Idt_setGate(22, (unsigned) Isr_isr22, 0x08, 0x8E);
    Idt_setGate(23, (unsigned) Isr_isr23, 0x08, 0x8E);

    Idt_setGate(24, (unsigned) Isr_isr24, 0x08, 0x8E);
    Idt_setGate(25, (unsigned) Isr_isr25, 0x08, 0x8E);
    Idt_setGate(26, (unsigned) Isr_isr26, 0x08, 0x8E);
    Idt_setGate(27, (unsigned) Isr_isr27, 0x08, 0x8E);
    Idt_setGate(28, (unsigned) Isr_isr28, 0x08, 0x8E);
    Idt_setGate(29, (unsigned) Isr_isr29, 0x08, 0x8E);
    Idt_setGate(30, (unsigned) Isr_isr30, 0x08, 0x8E);
    Idt_setGate(31, (unsigned) Isr_isr31, 0x08, 0x8E);

    return true;
}
