# Locking.h

Functions for acquiring and releasing locks.

## `enum Locking_Lock`

    typedef enum
    {
        Locking_Lock_terminal,
        Locking_Lock_memory
    } Locking_Lock;

Enumeration of the available locks.

## `void Locking_acquireLock(Locking_Lock lock)`

Blocks (calls `Threading_yield` if threading is initialized) until the
lock `lock` is free, then sets it to locked.

## `void Locking_releaseLock(Locking_Lock lock)`

Sets the lock `lock` to free.
