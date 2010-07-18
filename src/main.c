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

#include <system.h>
#include <text.h>
#include <gdt.h>
#include <idt.h>
#include <isrs.h>
#include <irq.h>
#include <timer.h>
#include <keyboard.h>

void kmain(void* mbd, unsigned int magic)
{
    if (magic != 0x2BADB002)
    {
        for (;;);
    }

    gdt_init();
    idt_init();
    isrs_init();
    irq_init();
    __asm__ __volatile__ ("sti");
    
    text_init();
    timer_init();
    keyboard_init();

    /* Just for kicks */
    char *boot_loader_name = (char*) ((long*) mbd)[16];
    puts(":: Booted with ");
    set_text_color_foreground(white);
    puts(boot_loader_name);
    set_text_color_foreground(light_gray);
    puts("\n");

    /* ISR Test */
    /*puts(":: Stand back! I am going to divide by zero!");
      putch(0x65 / 0);*/
    for (;;);
}
