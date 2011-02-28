#ifndef __LOCKING_H__
#define __LOCKING_H__

#include <Kernel.h>
#include <Init.h>
#include <Threading.h>

typedef enum
{
    Locking_Lock_terminal,
    Locking_Lock_size /* For getting the number of locks; not a real lock */
} Locking_Lock;

static Bool Locking_locks[Locking_Lock_size] = {false};

/* Must be inline! */
always_inline Bool Locking_setLock(Locking_Lock lock)
{
    /* <nixeagle> first argument = pointer to lock, second argument is the what should go in the EAX register... so 0x0 in your case, and the 3rd argument is what you want to set the lock to on success... so in your case 0x1.
    intel manual 2A page 264
    __asm__ __volatile__("lock; cmpxchg"EFREG_OPL" %3, %0; setz %1"
                       : "=m"(*p), "=q"(ret)
                       : "m"(*p), "r" (val_new), "a"(val_old) : "memory");*/
    u8 ret;
    __asm__ __volatile__("lock; cmpxchgl %3, %0; setz %1" : "=m" (Locking_locks[lock]), "=q" (ret) : "m" (Locking_locks[lock]), "r" (0x1), "a" (0x0) : "memory");
    return ret == 1;
}

static inline void Locking_acquireLock(Locking_Lock lock)
{
    while (!Locking_setLock(lock))
    {
        if (Init_initialized(Init_Driver_threading))
            Threading_yield();
    }
}

static inline void Locking_releaseLock(Locking_Lock lock)
{
    Locking_locks[lock] = false;
}

#endif
