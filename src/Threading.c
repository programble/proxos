#include <Threading.h>

#include <Memory.h>
#include <Time.h>

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

void Threading_endThread()
{
    Threading_currentThread->status = Threading_ThreadStatus_zombie;
    Threading_currentThread->counter = 0;
    asm("sti; hlt");
    Kernel_panic("Thread was not killed");
}

void Threading_switchThreads()
{
    Kernel_assert(Threading_currentThread, "All the threads have run off!");
    
    /* Do nothing if there is only one thread */
    if (Threading_currentThread == Threading_currentThread->next)
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
        __asm__ __volatile__("mov $0x20, %%al; outb %%al, $0x20; sti; jmp *%0" : : "r" (Threading_currentThread->function));
        /* Execution should never reach this point */
        Kernel_panic("Failed to jump to new thread");
        break;
    /* Remove zombie threads from the list */
    case Threading_ThreadStatus_zombie:
        Threading_removeThread(Threading_currentThread->next);
        Threading_currentThread->counter = 0;
        return;
    /* Skip over paused threads */
    case Threading_ThreadStatus_paused:
    {
        /* Move paused thread from after current to before to easily skip it */
        Threading_Thread *pausedThread = Threading_currentThread->next;
        Threading_currentThread->next = pausedThread->next;
        Threading_currentThread->next->previous = Threading_currentThread;
        Threading_currentThread->previous->next = pausedThread;
        Threading_currentThread->previous = pausedThread;
        return;
    }
    }
    Kernel_panic("Something went wrong");
}

Threading_Thread *Threading_fork(void (*function)())
{
    Threading_Thread *thread = Memory_allocate(sizeof(Threading_Thread));
    thread->id = Threading_nextId++;
    thread->stack = Memory_allocate(0x4000);
    thread->priority = 20;
    thread->counter = thread->priority;
    thread->function = function;
    thread->status = Threading_ThreadStatus_ready;
    Threading_addThread(thread);
    return thread;
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
    Threading_threads = 1;
    Time_addCallback(1, Threading_switchThreads);
    return true;
}