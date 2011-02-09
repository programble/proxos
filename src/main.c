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

#include <multiboot.h>
#include <gdt.h>
#include <idt.h>
#include <isr.h>
#include <irq.h>
#include <syscall.h>
#include <tty.h>
#include <keyboard.h>
#include <mm.h>
#include <timer.h>
#include <loader.h>

void kmain(multiboot_header *multiboot, u32 magic)
{
    assert(magic == MULTIBOOT_BOOTLOADER_MAGIC, "Invalid bootloader magic");
    
    gdt_install();
    idt_install();
    isr_install();
    irq_install();
    enable_interrupts();

    mm_install(multiboot);
    timer_install();
    tty_install();
    keyboard_install();
    syscall_install();

    puts("AmusOS Kernel\n ");
    puts(VERSION);
#ifdef DEBUG
    puts("-DEBUG");
#endif
    puts("\n ");
    puts(COMPILED "\n ");
    puts(COMPILER "\n");

    assert(multiboot->flags >> 3 & 1, "No game to load");
    assert(multiboot->mods_count, "No game to load");
    assert(multiboot->mods_count == 1, "How do you expect to play two games at once?");

    /*puts("Loading game: ");
    puts(multiboot->mods_addr->name);
    putch('\n');*/

    void (*game)(void) = load_game((void*) multiboot->mods_addr->start);
    game();
    panic("Game exited unexpectedly");
}
