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

#include <timer.h>

void timer_handler(struct regs *r)
{
    /* To get gcc to shut the fuck up about unused parameter */
    r = r;
    
    timer_ticks++;

    /* Second test */
    /*if (timer_ticks % 100 == 0)
      puts(":");*/
}

void timer_phase(int hz)
{
    int divisor = 1193180 / hz;
    outportb(0x43, 0x36);
    outportb(0x40, divisor & 0xFF);
    outportb(0x40, divisor >> 8);
}

void timer_init()
{
    puts(":: Initializing system timer\n");
    puts("  > Setting tick counter\n");
    /* Set ticks to 0 */
    timer_ticks = 0;
    puts("  > Setting interrupt frequency\n");
    /* Set frequency to 100 */
    timer_phase(100);
    puts("  > Installing IRQ handler\n");
    /* Install handler */
    irq_install_handler(0, timer_handler);
}
