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

#ifndef __SYSCALL_H__
#define __SYSCALL_H__

#include <isr.h>
#include <idt.h>

#define SYSCALL_INTERRUPT 0x80
#define SYSCALL_COUNT 20

#define SYSCALL_WRAPPER(func) syscall_##func
#define SYSCALL_RETURN_WRAP(type, func) void SYSCALL_WRAPPER(func) (type *val) { *val = func(); }

void syscall_install();

#endif
