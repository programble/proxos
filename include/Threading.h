#ifndef __THREADING_H__
#define __THREADING_H__

#include <Kernel.h>

typedef enum
{
    Threading_ThreadStatus_running,
    Threading_ThreadStatus_ready,
    Threading_ThreadStatus_paused,
    Threading_ThreadStatus_zombie
} Threading_ThreadStatus;

typedef struct Threading_Thread
{
    u32 id;
    Threading_ThreadStatus status;
    u32 esp;
    u32 ebp;
    void *stack;
    u32 priority;
    u32 counter;
    void (*function)();
    struct Threading_Thread *next;
    struct Threading_Thread *previous;
} Threading_Thread;

Bool Threading_initialize();
Threading_Thread *Threading_fork(void (*function)());

void Threading_threadDump();

#endif
