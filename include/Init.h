#ifndef __INIT_H__
#define __INIT_H__

#include <Kernel.h>

typedef enum
{
    Init_Component_serial,
    Init_Component_terminal,
    Init_Component_gdt,
    Init_Component_idt,
    Init_Component_isr,
    Init_Component_irq,
    Init_Component_memory,
    Init_Component_time,
    Init_Component_keyboard,
    Init_Component_threading,
    Init_Component__size /* For size of enum; not a real item */
} Init_Component;

Bool Init__initialize(Init_Component driver, const String name);
#define Init_initialize(x) Init__initialize(x, MACRO_STRING(x))
Bool Init_initialized(Init_Component driver);
#define Init_assertInitialized(x) Kernel_assert(Init_initialized(x), MACRO_STRING(x) " not initialized")

#endif
