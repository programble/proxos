#include <Idt.h>

#include <String.h>

Idt_Entry idt[256];
Idt_Pointer idtp;

void Idt_setGate(u8 num, u64 base, u16 sel, u8 flags)
{
    idt[num].baseLow = (base & 0xFFFF);
    idt[num].baseHigh = (base >> 16) & 0xFF;
    idt[num].sel = sel;
    idt[num].flags = flags;
    idt[num].always0 = 0x0;
}

void Idt_setLimit(u16 limit)
{
    idtp.limit = limit;
    Idt_load();
}

Bool Idt_initialize()
{
    idtp.limit = (sizeof(Idt_Entry) * 256) -1;
    idtp.base = (u32) &idt;
    String_set((u8*) &idt, 0, sizeof(Idt_Entry) * 256);
    Idt_load();
    return true;
}
