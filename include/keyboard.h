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

#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#include <tty.h>
#include <irq.h>
#include <mm.h>

typedef enum
{
    key_event_down,
    key_event_up
} key_event_type;

typedef struct
{
    key_event_type type;
    u32 keycode;
    char keychar;
    bool shift;
    bool ctrl;
    bool alt;
} key_event;

void keyboard_install();

key_event get_key_event();
bool get_key_event_nonblocking(key_event*);

#endif
