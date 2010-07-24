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
#include <tty.h>
#include <gdt.h>
#include <idt.h>
#include <isr.h>
#include <irq.h>
#include <timer.h>
#include <keyboard.h>
#include <mm.h>

void kmain(multiboot_header *multiboot, unsigned int magic)
{
    if (magic != 0x2BADB002)
    {
        panic("Bad multiboot magic");
    }

    gdt_init();
    idt_init();
    isr_init();
    irq_init();
    __asm__ __volatile__ ("sti");
    
    tty_init();
    mm_init(multiboot);
    timer_init();
    keyboard_init();

    puts("\n:: Booted with ");
    puts(multiboot->boot_loader_name);
    puts(" (");
    puts(multiboot->cmdline);
    puts(")\n");

    puts(":: ");
    set_text_color_foreground(white);
    puts("Proxos ");
    puts(VERSION);
    puts(" booted");
    set_text_color_foreground(light_gray);
    puts(" (Compiled ");
    puts(COMPILED);
    puts(")\n");

    /* Input loop */
    puts("\n");
    while (true)
    {
        puts(">");
        char *input = gets();
        if (strcmp(input, "coredump") == 0)
            coredump();
        else if (strcmp(input, "headerdump") == 0)
            headerdump();
        else if (strcmp(input, "panic") == 0)
            panic("Yes, my master");
        else if (strcmp(input, "ticks") == 0)
        {
            puts(int_to_str(timer_ticks, 10));
            puts("\n");
        }
        else if (strcmp(input, "halt") == 0)
            halt();
        else if (strcmp(input, "uptime") == 0)
        {
            puts("Up ");
            puts(int_to_str(uptime_hours, 10));
            puts(" hours, ");
            puts(int_to_str(uptime_minutes, 10));
            puts(" minutes, ");
            puts(int_to_str(uptime_seconds, 10));
            puts(" seconds\n");
        }
        free(input);
    }
    
    /* Idle loop */
    while (true)
    {
        __asm__ __volatile__("hlt");
    }
}
