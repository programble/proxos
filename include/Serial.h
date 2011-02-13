#ifndef __SERIAL_H__
#define __SERIAL_H__

#include <Kernel.h>

Bool Serial_initialize();

void Serial_putChar(char c);
void Serial_putString(const String s);

#endif
