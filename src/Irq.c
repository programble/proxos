#include <Irq.h>

#include <Idt.h>

extern void Irq_irq0();
extern void Irq_irq1();
extern void Irq_irq2();
extern void Irq_irq3();
extern void Irq_irq4();
extern void Irq_irq5();
extern void Irq_irq6();
extern void Irq_irq7();
extern void Irq_irq8();
extern void Irq_irq9();
extern void Irq_irq10();
extern void Irq_irq11();
extern void Irq_irq12();
extern void Irq_irq13();
extern void Irq_irq14();
extern void Irq_irq15();

void *Irq_handlers[16] = {NULL};

void Irq_installHandler(u32 irq, void (*handler)(Isr_Registers*))
{
    Irq_handlers[irq] = handler;
}

void Irq_uninstallHandler(u32 irq)
{
    Irq_handlers[irq] = NULL;
}

void Irq_handler(Isr_Registers *r)
{
    void (*handler)(Isr_Registers*) = Irq_handlers[r->interruptNumber - 32];
    if (handler)
        handler(r);

    if (r->interruptNumber >= 40)
        Kernel_outportb(0xA0, 0x20);

    Kernel_outportb(0x20, 0x20);
}

void Irq_remap()
{
    Kernel_outportb(0x20, 0x11);
    Kernel_outportb(0xA0, 0x11);
    Kernel_outportb(0x21, 0x20);
    Kernel_outportb(0xA1, 0x28);
    Kernel_outportb(0x21, 0x04);
    Kernel_outportb(0xA1, 0x02);
    Kernel_outportb(0x21, 0x01);
    Kernel_outportb(0xA1, 0x01);
    Kernel_outportb(0x21, 0x0);
    Kernel_outportb(0xA1, 0x0);
}

Bool Irq_initialize()
{
    Irq_remap();

    Idt_setGate(32, (unsigned) Irq_irq0, 0x08, 0x8E);
    Idt_setGate(33, (unsigned) Irq_irq1, 0x08, 0x8E);
    Idt_setGate(34, (unsigned) Irq_irq2, 0x08, 0x8E);
    Idt_setGate(35, (unsigned) Irq_irq3, 0x08, 0x8E);
    Idt_setGate(36, (unsigned) Irq_irq4, 0x08, 0x8E);
    Idt_setGate(37, (unsigned) Irq_irq5, 0x08, 0x8E);
    Idt_setGate(38, (unsigned) Irq_irq6, 0x08, 0x8E);
    Idt_setGate(39, (unsigned) Irq_irq7, 0x08, 0x8E);
    Idt_setGate(40, (unsigned) Irq_irq8, 0x08, 0x8E);
    Idt_setGate(41, (unsigned) Irq_irq9, 0x08, 0x8E);
    Idt_setGate(42, (unsigned) Irq_irq10, 0x08, 0x8E);
    Idt_setGate(43, (unsigned) Irq_irq11, 0x08, 0x8E);
    Idt_setGate(44, (unsigned) Irq_irq12, 0x08, 0x8E);
    Idt_setGate(45, (unsigned) Irq_irq13, 0x08, 0x8E);
    Idt_setGate(46, (unsigned) Irq_irq14, 0x08, 0x8E);
    Idt_setGate(47, (unsigned) Irq_irq15, 0x08, 0x8E);

    return true;
}
