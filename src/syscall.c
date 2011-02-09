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

#include <syscall.h>

#include <tty.h>
#include <mm.h>
#include <timer.h>
#include <keyboard.h>

void syscall_nop()
{
    return;
}

SYSCALL_RETURN_WRAP(u8, get_cursor_x);
SYSCALL_RETURN_WRAP(u8, get_cursor_y);

SYSCALL_RETURN_WRAP(u32, get_cursor_height);

void SYSCALL_WRAPPER(malloc)(void **val, u32 size)
{
    *val = malloc(size);
}

void SYSCALL_WRAPPER(realloc)(void **val, void *old, u32 size)
{
    *val = realloc(old, size);
}

SYSCALL_RETURN_WRAP(key_event, get_key_event);

void SYSCALL_WRAPPER(get_key_event_nonblocking)(bool *val, key_event *dest)
{
    *val = get_key_event_nonblocking(dest);
}

static void *syscalls[SYSCALL_COUNT] =
{
    /* 0 */ SYSCALL_WRAPPER(malloc),
    /* 1 */ free,
    /* 2 */ SYSCALL_WRAPPER(realloc),
    /* 3 */ cls,
    /* 4 */ putch,
    /* 5 */ set_foreground_color,
    /* 6 */ set_background_color,
    /* 7 */ get_foreground_color,
    /* 8 */ get_background_color,
    /* 9 */ set_cursor_x,
    /* 10 */ set_cursor_y,
    /* 11 */ SYSCALL_WRAPPER(get_cursor_x),
    /* 12 */ SYSCALL_WRAPPER(get_cursor_y),
    /* 13 */ show_cursor,
    /* 14 */ hide_cursor,
    /* 15 */ set_cursor_height,
    /* 16 */ SYSCALL_WRAPPER(get_cursor_height),
    /* 17 */ SYSCALL_WRAPPER(get_key_event),
    /* 18 */ SYSCALL_WRAPPER(get_key_event_nonblocking),
    /* 19 */ sleep,
};

void syscall_handler(registers *r)
{
    assert(r->eax < SYSCALL_COUNT, "Invalid system call");
    
    void (*syscall)(u32, u32, u32, u32, u32);
    syscall = syscalls[r->eax];
    /* Sleep fix */
    asm("sti");
    syscall(r->ebx, r->ecx, r->edx, r->esi, r->edi);
}

extern void isr_syscall();

void syscall_install()
{
    idt_set_gate(SYSCALL_INTERRUPT, (unsigned) isr_syscall, 0x08, 0x8E);
}
