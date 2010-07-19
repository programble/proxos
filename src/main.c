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
    timer_init();
    keyboard_init();
    mm_init(multiboot);

    puts("\n:: Booted with ");
    puts(multiboot->boot_loader_name);
    puts(" (");
    puts(multiboot->cmdline);
    puts(")\n");

    puts(":: Kernel starts at 0x");
    puts(int_to_str((u32) &link_kernel_entry, 16));
    puts(" and ends at 0x");
    puts(int_to_str((u32) &link_kernel_end, 16));
    puts("\n");

    puts(":: ");
    set_text_color_foreground(white);
    puts("Proxos ");
    puts(VERSION);
    puts(" booted");
    set_text_color_foreground(light_gray);
    puts(" (Compiled ");
    puts(COMPILED);
    puts(")\n");

    struct foo
    {
        u32 bar;
        u32 baz;
    };
    struct bar
    {
        u64 baz;
        u64 foo;
    };

    malloc(sizeof(struct foo));
    malloc(sizeof(struct bar));
    malloc(sizeof(struct foo));
    malloc(sizeof(struct bar));
        
    coredump();

    /* Idle loop */
    while (true)
    {
        __asm__ __volatile__("hlt");
    }
}
