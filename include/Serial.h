#ifndef __SERIAL_H__
#define __SERIAL_H__

#include <Kernel.h>

Bool Serial_initialize();

deprecated void Serial_putChar(char c);
deprecated void Serial_putString(const String s);

#endif
