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

Bool (*init_functions[Init_Driver__size])() = {Serial_initialize, Terminal_initialize, Gdt_initialize, Idt_initialize, Isr_initialize, Irq_initialize, Memory_initialize, Time_initialize, Keyboard_initialize, Threading_initialize};
Bool initialized[Init_Driver__size] = {false};

Bool Init__initialize(Init_Driver driver, const String name)
{
    if (initialized[driver])
    {
        Text_printString(":: ");
        Text_printString(name);
        Text_printString(" [DONE]\n");
        return true;
    }
    Text_printString(":: ");
    Text_printString(name);
    Text_printString(" [BUSY]");
    Bool status = init_functions[driver]();
    if (status)
        Text_printString("\b\b\b\b\bDONE]\n");
    else
        Text_printString("\b\b\b\b\bFAIL]\n");
    initialized[driver] = status;
    return status;
}

Bool Init_initialized(Init_Driver driver)
{
    return initialized[driver];
}
