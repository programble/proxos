/*  Copyright 2010 Curtis McEnroe <programble@gmail.com>
 *
 *  This file is part of AmusOS.
 *
 *  AmusOS is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  AmusOS is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with AmusOS.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <timer.h>

void timer_handler(registers*);

void timer_phase(u32 hz)
{
    u32 divisor = 1193180 / hz;
    outportb(0x43, 0x36);
    outportb(0x40, divisor & 0xFF);
    outportb(0x40, divisor >> 8);
}

void timer_install()
{
    timer_ticks = 0;
    timer_phase(100);
    irq_install_handler(0, timer_handler);
}

timer_hook *first_hook = NULL;

void timer_handler(registers *r)
{
    timer_ticks++;

    for (timer_hook *hook = first_hook; hook; hook = hook->next)
    {
        if (timer_ticks % hook->ticks == 0)
            hook->callback();
    }
}

void timer_add_hook(u32 ticks, void (*callback)(void))
{
    timer_hook *hook = malloc(sizeof(timer_hook));
    hook->ticks = ticks;
    hook->callback = callback;
    hook->next = first_hook;
    first_hook = hook;
}

void timer_remove_hook(void (*callback)(void))
{
    if (first_hook->callback == callback)
    {
        timer_hook *hook = first_hook;
        first_hook = hook->next;
        free(hook);
    }
    else
    {
        for (timer_hook *hook = first_hook; hook->next; hook = hook->next)
        {
            if (hook->next->callback == callback)
            {
                timer_hook *next = hook->next;
                hook->next = next->next;
                free(next);
            }
        }
    }
}

void sleep(u32 ticks)
{
    u32 start = timer_ticks;
    while (timer_ticks < start + ticks)
        __asm__ __volatile__("hlt");
}
