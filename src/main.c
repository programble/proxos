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

void kmain(void* mbd, unsigned int magic)
{
    if (magic != 0x2BADB002)
    {
        for (;;);
    }
    
    char *boot_loader_name = (char*) ((long*) mbd)[16];
    
    text_init();
    puts(":: Text-mode VGA initialized\n");

    gdt_init();
    puts(":: GDT initialized\n");
    
    puts(":: Booted with ");
    set_text_color_foreground(white);
    puts(boot_loader_name);
    set_text_color_foreground(light_gray);
    puts("\n");
}
