/*  Copyright 2010 Curtis McEnroe <programble@gmail.com>
 *
 *  This file is part of Proxos.
 *
 *  Proxos is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 * 
 *  Proxos is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Proxos.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __TTY_H__
#define __TTY_H__

#include <system.h>

#define ROWS 25
#define COLS 80

typedef enum
{
    black   = 0x0,
    blue    = 0x1,
    green   = 0x2,
    cyan    = 0x3,
    red     = 0x4,
    magenta = 0x5,
    brown   = 0x6,
    light_gray    = 0x7,
    dark_gray     = 0x8,
    light_blue    = 0x9,
    light_green   = 0xA,
    light_cyan    = 0xB,
    light_red     = 0xC,
    light_magenta = 0xD,
    light_brown   = 0xE,
    white         = 0xF
} color;

void tty_init();
void clear();
void putch(u8);
void puts(char*);
void set_text_color_foreground(color);
void set_text_color_background(color);
void set_text_color(color, color);

#endif
