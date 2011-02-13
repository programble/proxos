#include <Gdt.h>

Gdt_Entry gdt[3];
Gdt_Pointer gp;

void Gdt_setGate(int num, u64 base, u64 limit, u8 access, u8 gran)
{
    gdt[num].baseLow = (base & 0xFFFF);
    gdt[num].baseMiddle = (base >> 16) & 0xFF;
    gdt[num].baseHigh = (base >> 24) & 0xFF;
    gdt[num].limitLow = (limit & 0xFFFF);
    gdt[num].granularity = ((limit >> 16) & 0x0F);
    gdt[num].granularity |= ((u8) gran & 0xF0);
    gdt[num].access = access;
}

Bool Gdt_initialize()
{
    gp.limit = (sizeof(Gdt_Entry) * 3) - 1;
    gp.base = (u32) &gdt;

    Gdt_setGate(0, 0, 0, 0, 0);
    Gdt_setGate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
    Gdt_setGate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

    Gdt_flush();

    return true;
}
