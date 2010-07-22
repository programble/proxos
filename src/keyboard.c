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

typedef struct kbuffer
{
    char data;
    struct kbuffer *previous;
} kbuffer;

kbuffer *kbuffer_first;
kbuffer *kbuffer_last;

void kbuffer_enqueue(char data)
{
    kbuffer *new = malloc(sizeof(kbuffer));
    new->data = data;
    new->previous = NULL;
    if (kbuffer_last)
    {
        kbuffer_last->previous = new;
        kbuffer_last = new;
    }
    else
    {
        kbuffer_first = new;
        kbuffer_last = new;
    }
}

char kbuffer_dequeue()
{
    assert(kbuffer_first);
    char data = kbuffer_first->data;
    kbuffer *first = kbuffer_first;
    kbuffer_first = first->previous;
    if (kbuffer_last == first)
        kbuffer_last = NULL;
    free(first);
    return data;
}

void keyboard_handler(struct regs *r)
{
    /* STFU GCC */
    r = r;

    u8 scancode;

    scancode = inportb(0x60);

    if (scancode & 0x80)
    {
        if (scancode == 0x2a + 0x80)
        {
            /* Left shift release */
            left_shift = 0;
        }
        else if (scancode == 0x36 + 0x80)
        {
            /* Right shift release */
            right_shift = 0;
        }
    }
    else if (scancode == 0x2a)
    {
        /* Left shift press */
        left_shift = 1;
    }
    else if (scancode == 0x36)
    {
        /* Right shift press */
        right_shift = 1;
    }
    else
    {
        if (left_shift || right_shift)
        {
            putch(keymap_shift[scancode]);
            kbuffer_enqueue(keymap_shift[scancode]);
        }
        else
        {
            putch(keymap[scancode]);
            kbuffer_enqueue(keymap[scancode]);
        }
    }
}

void keyboard_init()
{
    puts(":: Initializing keyboard driver\n");
    left_shift = 0;
    right_shift = 0;
    kbuffer_first = NULL;
    kbuffer_last = NULL;
    puts("  > Using US keymap\n");
    keymap = keymap_us;
    keymap_shift = keymap_us_shift;
    puts("  > Installing IRQ handler\n");
    irq_install_handler(1, keyboard_handler);
}

char getch()
{
    /* Wait for a character */
    while (!kbuffer_first)
        __asm__("hlt");

    return kbuffer_dequeue();
}

char *gets()
{
    u32 length = 16;
    char *data = malloc(length);
    for (u32 i = 0;; i++)
    {
        char temp = getch();
        if (i >= length)
        {
            length += 16;
            data = realloc(data, length);
        }
        if (temp == '\n')
        {
            data[i] = 0x0;
            break;
        }
        if (temp == '\b')
        {
            i--;
            i--;
            continue;
        }
        data[i] = temp;
    }
    return data;
}
