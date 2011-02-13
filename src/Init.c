#include <Init.h>

#include <Terminal.h>
#include <Serial.h>

Bool (*init_functions[INIT_DRIVER_COUNT])() = {Serial_initialize, Terminal_initialize};
Bool initialized[INIT_DRIVER_COUNT] = {false};

Bool Init__initialize(Init_Driver driver, String name)
{
    if (initialized[driver] && initialized[Init_Driver_terminal])
    {
        Terminal_putString(":: ");
        Terminal_putString(name);
        Terminal_putString(" [DONE]\n");
        return true;
    }
    if (initialized[Init_Driver_terminal])
    {
        Terminal_putString(":: ");
        Terminal_putString(name);
        Terminal_putString(" [BUSY]");
    }
    Bool status = init_functions[driver]();
    if (status && initialized[Init_Driver_terminal])
        Terminal_putString("\b\b\b\b\bDONE]\n");
    else if (initialized[Init_Driver_terminal])
        Terminal_putString("\b\b\b\b\bFAIL]\n");
    initialized[driver] = status;
    return status;
}

Bool Init_initialized(Init_Driver driver)
{
    return initialized[driver];
}
