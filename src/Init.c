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

Bool (*init_functions[INIT_DRIVER_COUNT])() = {Serial_initialize, Terminal_initialize, Gdt_initialize, Idt_initialize, Isr_initialize, Irq_initialize, Memory_initialize, Time_initialize};
Bool initialized[INIT_DRIVER_COUNT] = {false};

Bool Init__initialize(Init_Driver driver, String name)
{
    if (initialized[driver])
    {
        Text_putString(":: ");
        Text_putString(name);
        Text_putString(" [DONE]\n");
        return true;
    }
    Text_putString(":: ");
    Text_putString(name);
    Text_putString(" [BUSY]");
    Bool status = init_functions[driver]();
    if (status)
        Text_putString("\b\b\b\b\bDONE]\n");
    else
        Text_putString("\b\b\b\b\bFAIL]\n");
    initialized[driver] = status;
    return status;
}

Bool Init_initialized(Init_Driver driver)
{
    return initialized[driver];
}
