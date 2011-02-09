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

#include <tty.h>

u16 *vram;

color foreground_color;
bool foreground_bright;
color background_color;
bool background_bright;

struct { u8 x; u8 y; } cursor;

void update_cursor()
{
    u32 temp = cursor.y * TTY_COLS + cursor.x;
    outportb(0x3D4, 14);
    outportb(0x3D5, temp >> 8);
    outportb(0x3D4, 15);
    outportb(0x3D5, temp);
}

void cls()
{
    assert(vram, "TTY not installed");

    u16 blank = ' ' | ((((background_color | (background_bright << 3)) << 4) | ((foreground_color | (foreground_bright << 3)) & 0x0F)) << 8);
    for (u8 i = 0; i < TTY_ROWS; i++)
        memsetw(vram + i * TTY_COLS, blank, TTY_COLS);
    cursor.x = 0;
    cursor.y = 0;
    update_cursor();
}

void scroll()
{
    assert(vram, "TTY not installed");

    u16 blank = ' ' | ((((background_color | (background_bright << 3)) << 4) | ((foreground_color | (foreground_bright << 3)) & 0x0F)) << 8);

    if (cursor.y >= TTY_ROWS)
    {
        /* Move everything up one line */
        u32 temp = cursor.y - TTY_ROWS + 1;
        memcpy((u8*) vram, (u8*) (vram + temp * TTY_COLS), (TTY_ROWS - temp) * TTY_COLS * 2);

        /* Blank the last line */
        memsetw(vram + (TTY_ROWS - temp) * TTY_COLS, blank, TTY_COLS);
        cursor.y = TTY_ROWS - 1;
    }
}

void move_cursor_back()
{
    /* If cursor is at beginning of line, move up one line */
    if (cursor.x == 0)
    {
        cursor.y--;
        cursor.x = TTY_COLS - 1;
    }
    /* Otherwise, just move back one */
    else
        cursor.x--;
    update_cursor();
}

void putch(u8 c)
{
    assert(vram, "TTY not installed");

    /* Backspace */
    switch (c)
    {
    case '\b':
        move_cursor_back();
        putch(' ');
        move_cursor_back();
        return;
    case '\t':
        cursor.x -= (cursor.x % 8) + 8;
        break;
    case '\r':
        cursor.x = 0;
        break;
    case '\n':
        cursor.y++;
        cursor.x = 0;
        break;
    default:
    {
        u16 attributes = ((background_color | (background_bright << 3)) << 4) | ((foreground_color | (foreground_bright << 3)) & 0x0F);
        u16 *location = vram + (cursor.y * TTY_COLS + cursor.x);
        *location = c | (attributes << 8);
        cursor.x++;
    }
    }

    /* End of line? */
    if (cursor.x >= TTY_COLS)
    {
        cursor.x -= TTY_COLS;
        cursor.y++;
    }

    scroll();
    update_cursor();
}

void puts(const string s)
{
    for (u32 i = 0; i < strlen(s); i++)
        putch(s[i]);
}

void set_foreground_color(color c, bool bright)
{
    foreground_color = c;
    foreground_bright = bright;
}

void set_background_color(color c, bool bright)
{
    background_color = c;
    background_bright = bright;
}

void get_foreground_color(color *c, bool *bright)
{
    *c = foreground_color;
    *bright = foreground_bright;
}

void get_background_color(color *c, bool *bright)
{
    *c = background_color;
    *bright = background_bright;
}

void set_cursor_x(u8 x)
{
    cursor.x = x;
    update_cursor();
}

void set_cursor_y(u8 y)
{
    cursor.y = y;
    update_cursor();
}

u8 get_cursor_x()
{
    return cursor.x;
}

u8 get_cursor_y()
{
    return cursor.y;
}

u32 cursor_height;

void hide_cursor()
{
    outportb(0x3D4, 0x0A);
    outportb(0x3D5, 16);
}

void show_cursor()
{
    outportb(0x3D4, 0x0A);
    outportb(0x3D5, cursor_height);
}

void set_cursor_height(u32 h)
{
    cursor_height = 16 - h;
    show_cursor();
}

u32 get_cursor_height()
{
    return 16 - cursor_height;
}

void tty_install()
{
    vram = (u16*) 0xb8000;
    foreground_color = gray;
    background_color = black;
    foreground_bright = false;
    background_bright = false;
    set_cursor_height(2);
    cls();
}
