#include <Text.h>

#include <Init.h>
#include <Serial.h>
#include <Terminal.h>

void Text_printChar(char c)
{
    if (Init_initialized(Init_Component_serial))
        Serial_printChar(c);
    if (Init_initialized(Init_Component_terminal))
        Terminal_printChar(c);
}

void Text_printString(const String s)
{
    if (Init_initialized(Init_Component_serial))
        Serial_printString(s);
    if (Init_initialized(Init_Component_terminal))
        Terminal_printString(s);
}
