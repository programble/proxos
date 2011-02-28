#include <Time.h>

#include <Irq.h>
#include <Memory.h>
#include <Threading.h>

u32 Time_ticks = 0;
Time_Callback *Time_Callback_first = NULL;

void Time_phase(u32 hz)
{
    u32 divisor = 1193180 / hz;
    Kernel_outportb(0x43, 0x36);
    Kernel_outportb(0x40, divisor & 0xFF);
    Kernel_outportb(0x40, divisor >> 8);
}

void Time_tickHandler(unused Isr_Registers *r)
{
    Time_ticks++;

    for (Time_Callback *callback = Time_Callback_first; callback; callback = callback->next)
    {
        if (Time_ticks % callback->ticks == 0)
            callback->callback();
    }
}

void Time_addCallback(u32 ticks, void (*callback)())
{
    Time_Callback *thing = Memory_allocate(sizeof(Time_Callback));
    thing->ticks = ticks;
    thing->callback = callback;
    thing->next = Time_Callback_first;
    Time_Callback_first = thing;
}

void Time_removeCallback(void (*callback)())
{
    if (Time_Callback_first->callback == callback)
    {
        Time_Callback *thing = Time_Callback_first;
        Time_Callback_first = thing->next;
        Memory_free(thing);
    }
    else
    {
        for (Time_Callback *thing = Time_Callback_first; thing->next; thing = thing->next)
        {
            if (thing->next->callback == callback)
            {
                Time_Callback *next = thing->next;
                thing->next = next->next;
                Memory_free(next);
            }
        }
    }
}

void Time_sleep(u32 ticks)
{
    u32 start = Time_ticks;
    while (Time_ticks < start + ticks)
        Threading_yield();
}

Bool Time_initialize()
{
    Time_phase(1000);
    Irq_installHandler(0, Time_tickHandler);
    return true;
}
