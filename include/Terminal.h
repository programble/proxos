#ifndef __TTY_H__
#define __TTY_H__

#include <Kernel.h>
#include <String.h>

typedef enum
{
    Terminal_Color_black,
    Terminal_Color_blue,
    Terminal_Color_green,
    Terminal_Color_cyan,
    Terminal_Color_red,
    Terminal_Color_magenta,
    Terminal_Color_brown,
    Terminal_Color_gray
} Terminal_Color;

Bool Terminal_initialize();

void Terminal_clear();
void Terminal_putChar(char c, u8 x, u8 y);
void Terminal_putString(const String s, u8 x, u8 y);

void Terminal_printChar(char c);
void Terminal_printString(const String s);

void Terminal_setForegroundColor(Terminal_Color color, Bool bright);
void Terminal_setBackgroundColor(Terminal_Color color, Bool bright);
#define Terminal_setColor(fg, fb, bg, bb) Terminal_setForegroundColor(fg, fb); Terminal_setBackgroundColor(bg, bb)

void Terminal_getForegroundColor(Terminal_Color *color, Bool *bright);
void Terminal_getBackgroundColor(Terminal_Color *color, Bool *bright);
#define Terminal_getColor(fg, gb, bg, bb) Terminal_getForegroundColor(fg, fb); Terminal_getBackgroundColor(bg, bb)

void Terminal_setCursorX(u8 x);
void Terminal_setCursorY(u8 y);
#define Terminal_setCursorPosition(x, y) Terminal_setCursorX(x); Terminal_setCursorY(y)

u8 Terminal_getCursorX();
u8 Terminal_getCursorY();

void Terimal_hideCursor();
void Terminal_showCursor();

void Terminal_setCursorHeight(u32 height);
u32 Terminal_getCursorHeight();

#endif
