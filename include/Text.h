#ifndef __TEXT_H__
#define __TEXT_H__

#include <Kernel.h>

deprecated void Text_putChar(char c);
deprecated void Text_putString(const String s);

void Text_printChar(char c);
void Text_printString(const String s);

#endif
