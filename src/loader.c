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

#include <loader.h>

void *load_game(void* game)
{
    Elf32_Ehdr *ehdr = game;

    /* Ensure game is ELF */
    assert((ehdr->e_ident[EI_MAG0] == ELFMAG0 && ehdr->e_ident[EI_MAG1] == ELFMAG1 && ehdr->e_ident[EI_MAG2] == ELFMAG2 && ehdr->e_ident[EI_MAG3] == ELFMAG3), "Game is not an ELF file");

    /* Ensure game is executable */
    assert(ehdr->e_type == ET_EXEC, "Game is not executable");

    /* Ensure there is a program header */
    assert(ehdr->e_phoff != 0, "Game has no program header");

    Elf32_Phdr *phdr = (Elf32_Phdr*) ((u32) game + (u32) ehdr->e_phoff);

    /* Do some crazy shit that I don't know what it actually does */
    for (int i = 0; i < ehdr->e_phnum; i++, phdr++)
    {
        if (phdr->p_type == PT_LOAD)
        {
            memset((u8*) phdr->p_vaddr, 0, phdr->p_memsz);
            memcpy((u8*) phdr->p_vaddr, (u8*) ((u32) game + (u32) phdr->p_offset), phdr->p_filesz);
        }
    }

    return (void*) ehdr->e_entry;
}
