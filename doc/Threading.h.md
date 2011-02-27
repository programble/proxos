# Threading.h

Functions related to threading.

## `Threading_Thread *Threading_fork(void (*function)())`

Creates a new thread with default priority and stack size that runs
the function `function`. The thread is added to the thread list and
will be started automatically. The thread object is returned.

## `void Threading_yield()`

Causes the current thread to yield. This means that the current thread
yields its time slice and threads are switched immediately. Use this
to block while waiting for I/O so that a blocking thread does not
starve other threads of CPU time.

## `void Threading_sleep(u32 duration)`

Puts the current thread to sleep for `duration` milliseconds. This
causes the thread to be skipped when switching threads until its
sleeping time has elapsed.
