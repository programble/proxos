u8 inportb(u16 port)
{
    u8 ret;
    asm("inb %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}

void outportb(u16 port, u8 data)
{
    __asm__ __volatile__("outb %1, %0" : : "dN" (port), "a" (data));
}
