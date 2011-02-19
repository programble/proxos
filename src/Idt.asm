global Idt_load
extern idtp ; Idt.c

Idt_load:
    lidt [idtp]
    ret
