#include <Serial.h>

#include <String.h>

#define SERIAL_PORT 0x3f8

Bool Serial_initialize()
{
   Kernel_outportb(SERIAL_PORT + 1, 0x00);
   Kernel_outportb(SERIAL_PORT + 3, 0x80);
   Kernel_outportb(SERIAL_PORT + 0, 0x03);
   Kernel_outportb(SERIAL_PORT + 1, 0x00);
   Kernel_outportb(SERIAL_PORT + 3, 0x03);
   Kernel_outportb(SERIAL_PORT + 2, 0xC7);
   Kernel_outportb(SERIAL_PORT + 4, 0x0B);
   return true;
}

void Serial_printChar(char c)
{
    while (!(Kernel_inportb(SERIAL_PORT + 5) & 0x20));
    Kernel_outportb(SERIAL_PORT, c);
}

void Serial_printString(const String s)
{
    for (u32 i = 0; i < String_length(s); i++)
        Serial_printChar(s[i]);
}

void Serial_putChar(char c)
{
    Serial_printChar(c);
}

void Serial_putString(const String s)
{
    Serial_printString(s);
}
