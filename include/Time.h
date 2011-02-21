#ifndef __TIME_H__
#define __TIME_H__

#include <Kernel.h>

Bool Time_initialize();

typedef struct Time_Callback
{
    u32 ticks;
    void (*callback)();
    struct Time_Callback *next;
} Time_Callback;

void Time_addCallback(u32 ticks, void (*callback)());
void Time_removeCallback(void (*callback)());

void Time_sleep(u32 ticks);

#endif
