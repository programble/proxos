#ifndef __INIT_H__
#define __INIT_H__

#include <Kernel.h>

typedef enum
{
    Init_Driver_serial,
    Init_Driver_terminal,
    Init_Driver_gdt,
    Init_Driver_idt,
    Init_Driver_isr,
    Init_Driver_irq
} Init_Driver;
#define INIT_DRIVER_COUNT 6

Bool Init__initialize(Init_Driver driver, String name);
#define Init_initialize(x) Init__initialize(x, MACRO_STRING(x))
Bool Init_initialized(Init_Driver driver);
#define Init_assertInitialized(x) Kernel_assert(Init_initialized(x), MACRO_STRING(x) " not initialized")

#endif
