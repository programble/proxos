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

#ifndef __TTY_H__
#define __TTY_H__

#include <string.h>

#define TTY_ROWS 25
#define TTY_COLS 80

typedef enum
{
    black,
    blue,
    green,
    cyan,
    red,
    magenta,
    brown,
    gray
} color;

void tty_install();

void cls();
void putch(u8);
void puts(const string);

void set_foreground_color(color, bool);
void set_background_color(color, bool);

void get_foreground_color(color*, bool*);
void get_background_color(color*, bool*);

void set_cursor_x(u8);
void set_cursor_y(u8);

u8 get_cursor_x();
u8 get_cursor_y();

void hide_cursor();
void show_cursor();

void set_cursor_height(u32);
u32 get_cursor_height();

#endif
