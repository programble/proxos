#include <Threading.h>

#include <Memory.h>
#include <Time.h>
#include <Text.h>
#include <String.h>

Threading_Thread *Threading_currentThread;
u32 Threading_threads;
u32 Threading_nextId = 0;

void Threading_addThread(Threading_Thread *thread)
{
    thread->next = Threading_currentThread->next;
    Threading_currentThread->next->previous = thread;
    Threading_currentThread->next = thread;
    thread->previous = Threading_currentThread;

    Threading_threads++;
}

void Threading_removeThread(Threading_Thread *thread)
{
    Kernel_assert(thread->next != thread && thread->previous != thread, "Trying to remove last remaining thread");

    thread->previous->next = thread->next;
    thread->next->previous = thread->previous;

    Memory_free(thread->stack);
    Memory_free(thread);

    Threading_threads--;
}

noreturn Threading_endThread()
{
    Threading_currentThread->status = Threading_ThreadStatus_zombie;
    Threading_currentThread->counter = 0;
    while (true)
        Threading_yield();
}

void Threading_switchThreads()
{
    Kernel_assert(Threading_currentThread, "All the threads have run off!");
    
    /* Do nothing if there is only one thread */
    if (Threading_currentThread == Threading_currentThread->next && Threading_currentThread->status == Threading_ThreadStatus_running)
        return;

    /* If it's still the current thread's turn, do nothing */
    if (Threading_currentThread->counter)
    {
        Threading_currentThread->counter--;
        return;
    }

    /* Current thread's time is up, reset it's time */
    Threading_currentThread->counter = Threading_currentThread->priority;

    switch (Threading_currentThread->next->status)
    {
    /* If the next thread is running, jump to it */
    case Threading_ThreadStatus_running:
        /* Save the stack registers of the current thread */
        __asm__ __volatile__("mov %%esp, %0" : "=r" (Threading_currentThread->esp));
        __asm__ __volatile__("mov %%ebp, %0" : "=r" (Threading_currentThread->ebp));
        Threading_currentThread = Threading_currentThread->next;
        /* Load the stack registers from the new current thread */
        __asm__ __volatile__("mov %0, %%esp" : : "r" (Threading_currentThread->esp));
        __asm__ __volatile__("mov %0, %%ebp" : : "r" (Threading_currentThread->ebp));
        return;
    /* If the next thread is ready, start it and jump to it */
    case Threading_ThreadStatus_ready:
        /* Save the current thread's stack registers */
        __asm__ __volatile__("mov %%esp, %0" : "=r" (Threading_currentThread->esp));
        __asm__ __volatile__("mov %%ebp, %0" : "=r" (Threading_currentThread->ebp));
        Threading_currentThread = Threading_currentThread->next;
        Threading_currentThread->status = Threading_ThreadStatus_running;
        /* Set up the new thread's stack registers */
        Threading_currentThread->esp = (u32) Threading_currentThread->stack + 0x4000;
        Threading_currentThread->ebp = Threading_currentThread->esp;
        __asm__ __volatile__("mov %0, %%esp" : : "r" (Threading_currentThread->esp));
        __asm__ __volatile__("mov %0, %%ebp" : : "r" (Threading_currentThread->ebp));
        /* Set up where we want the thread to end */
        __asm__ __volatile__("push %0" : : "r" (Threading_endThread));
        /* Jump to the new thread's function */
        __asm__ __volatile__("mov $0x20, %%al; outb %%al, $0x20; sti; jmp *%0" : : "r" (Threading_currentThread->function) : "eax");
        /* Execution should never reach this point */
        Kernel_panic("Failed to jump to new thread");
        break;
    /* Remove zombie threads from the list */
    case Threading_ThreadStatus_zombie:
        Threading_removeThread(Threading_currentThread->next);
        Threading_currentThread->counter = 0;
        return;
    case Threading_ThreadStatus_sleeping:
        if (Threading_currentThread->next->wakeTime <= Time_ticks)
        {
            Threading_currentThread->next->status = Threading_ThreadStatus_running;
            Threading_currentThread->next->wakeTime = 0;
            return;
        }
        if (Threading_threads == 1)
            return;
    /* Skip over paused/sleeping threads */
    case Threading_ThreadStatus_paused:
    {
        /* Move paused thread from after current to before to easily skip it */
        Threading_Thread *pausedThread = Threading_currentThread->next;
        Threading_currentThread->next = pausedThread->next;
        Threading_currentThread->next->previous = Threading_currentThread;
        Threading_currentThread->previous->next = pausedThread;
        pausedThread->next = Threading_currentThread;
        pausedThread->previous = Threading_currentThread->previous;
        Threading_currentThread->previous = pausedThread;
        Threading_currentThread->counter = 0;
        return;
    }
    }
    Kernel_panic("Consequences will never be the same");
}

Threading_Thread *Threading_fork(void (*function)())
{
    Threading_Thread *thread = Memory_allocate(sizeof(Threading_Thread));
    thread->id = Threading_nextId++;
    thread->stack = Memory_allocate(0x4000);
    thread->priority = 20;
    thread->counter = thread->priority;
    thread->wakeTime = 0;
    thread->function = function;
    thread->status = Threading_ThreadStatus_ready;
    Threading_addThread(thread);
    return thread;
}

void Threading_yield()
{
    Threading_currentThread->counter = 0;
    asm("hlt");
}

void Threading_sleep(u32 duration)
{
    Threading_currentThread->wakeTime = Time_ticks + duration;
    Threading_currentThread->status = Threading_ThreadStatus_sleeping;
    while (Threading_currentThread->status == Threading_ThreadStatus_sleeping)
        Threading_yield();
}

Bool Threading_initialize()
{
    Threading_currentThread = Memory_allocate(sizeof(Threading_Thread));
    Threading_currentThread->status = Threading_ThreadStatus_running;
    Threading_currentThread->next = Threading_currentThread;
    Threading_currentThread->previous = Threading_currentThread;
    Threading_currentThread->stack = (void*) ((u32) Kernel_stackPointer - 0x4000);
    Threading_currentThread->id = Threading_nextId++;
    Threading_currentThread->priority = 20;
    Threading_currentThread->counter = 20;
    Threading_currentThread->wakeTime = 0;
    Threading_threads = 1;
    Time_addCallback(1, Threading_switchThreads);
    return true;
}

void Threading_threadDump()
{
    Text_printString("  ID\tSTATUS\tPRI\tCOUNTER\tWAKE\tSTACK\t\tFUNCTION\n");
    Threading_Thread *thread = Threading_currentThread;
    for (u32 i = 0; i < Threading_threads; i++)
    {
        Text_printString((thread == Threading_currentThread) ? "->" : "  ");
        Text_printString(String_formatInt(thread->id, 10));
        Text_printString("\t");
        Text_printString(String_formatInt(thread->status, 10));
        Text_printString("\t");
        Text_printString(String_formatInt(thread->priority, 10));
        Text_printString("\t");
        Text_printString(String_formatInt(thread->counter, 10));
        Text_printString("\t");
        Text_printString((thread->wakeTime) ? String_formatInt(thread->wakeTime - Time_ticks, 10) : "AWAKE");
        Text_printString("\t0x");
        Text_printString(String_formatInt((u32) thread->stack, 16));
        Text_printString("\t0x");
        Text_printString(String_formatInt((u32) thread->function, 16));
        Text_printString("\n");
        thread = thread->next;
    }
}
