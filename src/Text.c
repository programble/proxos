#include <Text.h>

#include <Init.h>
#include <Serial.h>
#include <Terminal.h>

void Text_printChar(char c)
{
    if (Init_initialized(Init_Driver_serial))
        Serial_printChar(c);
    if (Init_initialized(Init_Driver_terminal))
        Terminal_printChar(c);
}

void Text_printString(const String s)
{
    if (Init_initialized(Init_Driver_serial))
        Serial_printString(s);
    if (Init_initialized(Init_Driver_terminal))
        Terminal_printString(s);
}

void Text_putChar(char c)
{
    Text_printChar(c);
}

void Text_putString(const String s)
{
    Text_printString(s);
}
