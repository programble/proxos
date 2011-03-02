#include <Init.h>

#include <Text.h>
#include <Terminal.h>
#include <Serial.h>
#include <Gdt.h>
#include <Idt.h>
#include <Isr.h>
#include <Irq.h>
#include <Memory.h>
#include <Time.h>
#include <Keyboard.h>
#include <Threading.h>

Bool (*init_functions[Init_Component__size])() = {Serial_initialize, Terminal_initialize, Gdt_initialize, Idt_initialize, Isr_initialize, Irq_initialize, Memory_initialize, Time_initialize, Keyboard_initialize, Threading_initialize};
Bool initialized[Init_Component__size] = {false};

Bool Init__initialize(Init_Component driver, const String name)
{
    if (initialized[driver])
    {
        return true;
    }
    Text_printString(":: ");
    Text_printString(name);
    if (Init_initialized(Init_Component_terminal))
        Terminal_putString("[BUSY]", 73, Terminal_getCursorY());
    Serial_printString(" [BUSY]");
    Bool status = init_functions[driver]();
    if (status)
    {
        if (Init_initialized(Init_Component_terminal))
            Terminal_putString("[DONE]", 73, Terminal_getCursorY());
        Serial_printString("\b\b\b\b\bDONE]\n");
    }
    else
    {
        if (Init_initialized(Init_Component_terminal))
            Terminal_putString("[FAIL]", 73, Terminal_getCursorY());
        Serial_printString("\b\b\b\b\bFAIL]\n");
    }
    if (Init_initialized(Init_Component_terminal))
        Terminal_printChar('\n');
    initialized[driver] = status;
    return status;
}

Bool Init_initialized(Init_Component driver)
{
    return initialized[driver];
}
