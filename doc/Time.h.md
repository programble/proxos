# Time.h

Functions for time-travel.

## `void Time_sleep(u32 ticks)`

Blocks until `ticks` ticks have passed.

## `void Time_addCallback(u32 ticks, void (*callback)())`

Adds `callback` to the callback list. `callback` will be called every
time `ticks` ticks pass.

## `void Time_removeCallback(void (*callback)())`

Removes `callback` from the callback list. If `callback` was added
more than once, this will not remove all of them.

## Technical

When initialized, the timer is set to tick 100 times every second.
