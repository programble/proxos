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

#include <keyboard.h>

char keymap_us[128] =
{
    0, 27,
    '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, /* Control */
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'',
    '`',
    0, /* Left shift */
    '\\',
    'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',
    0, /* Right shift */
    '*',
    0, /* Alt */
    ' ',
    0, /* Caps lock */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /* F1 - F10 */
    0, /* Num lock */
    0, /* Scroll lock */
    0, /* Home */
    0, /* Up */
    0, /* Page up */
    '-',
    0, /* Left */
    0,
    0, /* Right */
    '+',
    0, /* End */
    0, /* Down */
    0, /* Page down */
    0, /* Insert */
    0, /* Delete */
    0, 0, 0,
    0, /* F11 */
    0, /* F12 */
    0,
};

char keymap_us_shift[128] =
{
    0, 27,
    '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
    '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
    0, /* Control */
    'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"',
    '~',
    0, /* Left shift */
    '|',
    'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?',
    0, /* Right shift */
    '*',
    0, /* Alt */
    ' ',
    0, /* Caps lock */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /* F1 - F10 */
    0, /* Num lock */
    0, /* Scroll lock */
    0, /* Home */
    0, /* Up */
    0, /* Page up */
    '-',
    0, /* Left */
    0,
    0, /* Right */
    '+',
    0, /* End */
    0, /* Down */
    0, /* Page down */
    0, /* Insert */
    0, /* Delete */
    0, 0, 0,
    0, /* F11 */
    0, /* F12 */
    0,
};

void keyboard_handler(struct regs *r)
{
    /* STFU GCC */
    r = r;

    u8 scancode;

    scancode = inportb(0x60);

    if (scancode & 0x80)
    {
        /* Key released */
    }
    else
    {
        if (shift)
        {
            putch(keymap_shift[scancode]);
        }
        else
        {
            putch(keymap[scancode]);
        }
    }
}

void keyboard_init()
{
    shift = 0;
    keymap = keymap_us;
    keymap_shift = keymap_us_shift;
    puts(":: Using US keymap\n");

    irq_install_handler(1, keyboard_handler);
    puts(":: Keyboard driver initialized\n");
}
