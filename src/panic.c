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

bool recursive_panic = false;

void _panic(const string message, const string function, const string file, const string line)
{
    if (recursive_panic)
        halt();
    recursive_panic = true;
    puts("\n\nERROR\n\n");
    puts(message);
    puts("\n\n");
    puts(function);
    puts("@");
    puts(file);
    puts(":");
    puts(line);
    halt();
}
