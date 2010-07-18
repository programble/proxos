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

#include <text.h>

u16 *vram;

color foreground;
color background;

int cursor_x;
int cursor_y;

void move_cursor();

void text_init()
{
    /* Location of video RAM in memory */
    vram = (u16 *) 0xb8000;
    /* Default colors */
    foreground = light_gray;
    background = black;
    /* Clear the screen */
    clear();
    /* Print out status */
    puts(":: Text-mode ");
    set_text_color_foreground(red);
    puts("V");
    set_text_color_foreground(green);
    puts("G");
    set_text_color_foreground(blue);
    puts("A");
    set_text_color_foreground(light_gray);
    puts(" initialized\n");
}

void clear()
{
    u32 blank;

    u16 attrib = (background << 4) | (foreground & 0x0F);
    /* Blank space */
    blank = 0x20 | (attrib << 8);

    for(int i = 0; i < ROWS; i++)
        memsetw(vram + i * COLS, blank, COLS);

    /* Move cursor to top-left */
    cursor_x = 0;
    cursor_y = 0;
    move_cursor();
}

void scroll()
{
    u32 blank;
    u32 temp;

    u16 attrib = (background << 4) | (foreground & 0x0F);
    /* Blank space */
    blank = 0x20 | (attrib << 8);

    if(cursor_y >= ROWS)
    {
        /* Move everything up one line */
        temp = cursor_y - ROWS + 1;
        memcpy((u8*) vram, (u8*) (vram + temp * COLS), (ROWS - temp) * COLS * 2);

        /* Blank the bottom line */
        memsetw(vram + (ROWS - temp) * COLS, blank, COLS);
        cursor_y = ROWS - 1;
    }
}

void putch(u8 c)
{
    /* Backspace */
    if (c == 0x08)
    {
        /* Move the cursor back one */
        if (cursor_x == 0)
        {
            cursor_y--;
            cursor_x = COLS - 1;
        }
        else
        {
            cursor_x--;
        }
        /* Blank it */
        putch(' ');
        /* Move the cursor back one again */
        if (cursor_x == 0)
        {
            cursor_y--;
            cursor_x = COLS - 1;
        }
        else
        {
            cursor_x--;
        }
    }
    /* Tab (must die) */
    else if (c == 0x09)
    {
        cursor_x += 4;
    }
    /* Carriage return */
    else if (c == '\r')
    {
        cursor_x = 0;
    }
    /* Newline */
    else if (c == '\n')
    {
        cursor_y++;
        cursor_x = 0;
    }
    /* Anything else */
    else
    {
        /* Determine attributes */
        u16 attrib = (background << 4) | (foreground & 0x0F);
        /* Determine location in linear memory */
        volatile u16 *location;
        location = vram + (cursor_y * COLS + cursor_x);
        *location = c | (attrib << 8);
        /* Move the cursor */
        cursor_x++;
    }
    
    /* Have we hit the EOL? */
    if (cursor_x >= COLS)
    {
        cursor_x = 0;
        cursor_y++;
    }
    scroll();
    move_cursor();
}

void puts(char *str)
{
    for (int i = 0; i < strlen(str); i++)
        putch((u8) str[i]);
}

void move_cursor(void)
{
    u32 temp;

    temp = cursor_y * COLS + cursor_x;

    outportb(0x3D4, 14);
    outportb(0x3D5, temp >> 8);
    outportb(0x3D4, 15);
    outportb(0x3D5, temp);
}

void set_text_color_foreground(color c)
{
    foreground = c;
}

void set_text_color_background(color c)
{
    background = c;
}

void set_text_color(color fore, color back)
{
    set_text_color_foreground(fore);
    set_text_color_background(back);
}
