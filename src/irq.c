/*  Copyright 2010 Curtis McEnroe <programble@gmail.com>
 *
 *  This file is part of AmusOS.
 *
 *  AmusOS is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 * 
 *  AmusOS is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with AmusOS.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <irq.h>

extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

void *irq_handlers[16] = {NULL};

void irq_install_handler(int irq, void (*handler)(registers*))
{
    irq_handlers[irq] = handler;
}

void irq_uninstall_handler(int irq)
{
    irq_handlers[irq] = NULL;
}

void irq_handler(registers *r)
{
    void (*handler)(registers*);

    handler = irq_handlers[r->int_no - 32];
    if (handler)
        handler(r);

    if (r->int_no >= 40)
        outportb(0xA0, 0x20);

    outportb(0x20, 0x20);
}

void irq_remap()
{
    outportb(0x20, 0x11);
    outportb(0xA0, 0x11);
    outportb(0x21, 0x20);
    outportb(0xA1, 0x28);
    outportb(0x21, 0x04);
    outportb(0xA1, 0x02);
    outportb(0x21, 0x01);
    outportb(0xA1, 0x01);
    outportb(0x21, 0x0);
    outportb(0xA1, 0x0);
}

void irq_install()
{
    irq_remap();

    idt_set_gate(32, (unsigned) irq0, 0x08, 0x8E);
    idt_set_gate(33, (unsigned) irq1, 0x08, 0x8E);
    idt_set_gate(34, (unsigned) irq2, 0x08, 0x8E);
    idt_set_gate(35, (unsigned) irq3, 0x08, 0x8E);
    idt_set_gate(36, (unsigned) irq4, 0x08, 0x8E);
    idt_set_gate(37, (unsigned) irq5, 0x08, 0x8E);
    idt_set_gate(38, (unsigned) irq6, 0x08, 0x8E);
    idt_set_gate(39, (unsigned) irq7, 0x08, 0x8E);
    idt_set_gate(40, (unsigned) irq8, 0x08, 0x8E);
    idt_set_gate(41, (unsigned) irq9, 0x08, 0x8E);
    idt_set_gate(42, (unsigned) irq10, 0x08, 0x8E);
    idt_set_gate(43, (unsigned) irq11, 0x08, 0x8E);
    idt_set_gate(44, (unsigned) irq12, 0x08, 0x8E);
    idt_set_gate(45, (unsigned) irq13, 0x08, 0x8E);
    idt_set_gate(46, (unsigned) irq14, 0x08, 0x8E);
    idt_set_gate(47, (unsigned) irq15, 0x08, 0x8E);
}
