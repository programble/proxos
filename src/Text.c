#include <Text.h>

#include <Init.h>
#include <Serial.h>
#include <Terminal.h>

void Text_putChar(char c)
{
    if (Init_initialized(Init_Driver_serial))
        Serial_putChar(c);
    if (Init_initialized(Init_Driver_terminal))
        Terminal_putChar(c);
}

void Text_putString(const String s)
{
    if (Init_initialized(Init_Driver_serial))
        Serial_putString(s);
    if (Init_initialized(Init_Driver_terminal))
        Terminal_putString(s);
}
