#ifndef __SERIAL_H__
#define __SERIAL_H__

#include <Kernel.h>

Bool Serial_initialize();

void Serial_printChar(char c);
void Serial_printString(const String s);

#endif
