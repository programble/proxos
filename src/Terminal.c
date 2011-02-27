#include <Terminal.h>

#include <Init.h>
#include <Locking.h>

#define TTY_COLS 80
#define TTY_ROWS 25

u16 *Terminal_ram = (u16*) 0xB8000;

Terminal_Color Terminal_foregroundColor = Terminal_Color_gray;
Bool Terminal_foregroundBright = false;
Terminal_Color Terminal_backgroundColor = Terminal_Color_black;
Bool Terminal_backgroundBright = false;

struct { u8 x; u8 y; } Terminal_cursor;
u8 Terminal_cursorHeight = 2;

void Terminal_updateCursor()
{
    u32 temp = Terminal_cursor.y * TTY_COLS + Terminal_cursor.x;
    Kernel_outportb(0x3D4, 14);
    Kernel_outportb(0x3D5, temp >> 8);
    Kernel_outportb(0x3D4, 15);
    Kernel_outportb(0x3D5, temp);
}

void Terminal_clear()
{
    u16 blank = ' ' | ((((Terminal_backgroundColor | (Terminal_backgroundBright << 3)) << 4) | ((Terminal_foregroundColor | (Terminal_foregroundBright << 3)) & 0x0F)) << 8);
    Locking_acquireLock(Locking_Lock_terminal);
    for (u8 i = 0; i < TTY_ROWS; i++)
        String_setWord(Terminal_ram + i * TTY_COLS, blank, TTY_COLS);
    Terminal_cursor.x = 0;
    Terminal_cursor.y = 0;
    Terminal_updateCursor();
    Locking_releaseLock(Locking_Lock_terminal);
}

void Terminal_scroll()
{
    u16 blank = ' ' | ((((Terminal_backgroundColor | (Terminal_backgroundBright << 3)) << 4) | ((Terminal_foregroundColor | (Terminal_foregroundBright << 3)) & 0x0F)) << 8);

    if (Terminal_cursor.y >= TTY_ROWS)
    {
        /* Move everything up one line */
        u32 position = Terminal_cursor.y - TTY_ROWS + 1;
        String_copy((u8*) Terminal_ram, (u8*) (Terminal_ram + position * TTY_COLS), (TTY_ROWS - position) * TTY_COLS * 2);
        /* Blank the last line */
        String_setWord(Terminal_ram + (TTY_ROWS - position) * TTY_COLS, blank, TTY_COLS);
        Terminal_cursor.y = TTY_ROWS - 1;
    }
}

void Terminal_moveBack()
{
    /* If at beginning of line, move up one line */
    if (Terminal_cursor.x == 0)
    {
        Terminal_cursor.y--;
        Terminal_cursor.x = TTY_COLS -1;
    }
    /* otherwise, move back one */
    else
        Terminal_cursor.x--;
    Terminal_updateCursor();
}

void Terminal_putChar(u8 c)
{
    Locking_acquireLock(Locking_Lock_terminal);
    Bool backspace = false;
    switch (c)
    {
    case '\t':
        Terminal_cursor.x = Terminal_cursor.x - (Terminal_cursor.x % 8) + 8;
        break;
    case '\n':
        Terminal_cursor.y++;
    case '\r':
        Terminal_cursor.x = 0;
        break;
    case '\b':
        Terminal_moveBack();
        backspace = true;
        c = ' ';
    default:
    {
        u16 attributes = ((Terminal_backgroundColor | (Terminal_backgroundBright << 3)) << 4) | ((Terminal_foregroundColor | (Terminal_foregroundBright << 3)) & 0x0F);
        u16 *location = Terminal_ram + (Terminal_cursor.y * TTY_COLS + Terminal_cursor.x);
        *location = c | (attributes << 8);
        Terminal_cursor.x++;
    }
    }

    if (backspace)
        Terminal_moveBack();

    /* EOL */
    if (Terminal_cursor.x >= TTY_COLS)
    {
        Terminal_cursor.x -= TTY_COLS;
        Terminal_cursor.y++;
    }

    Terminal_scroll();
    Terminal_updateCursor();
    Locking_releaseLock(Locking_Lock_terminal);
}

void Terminal_putString(const String s)
{
    for (u32 i = 0; i < String_length(s); i++)
        Terminal_putChar(s[i]);
}

void Terminal_setForegroundColor(Terminal_Color color, Bool bright)
{
    Terminal_foregroundColor = color;
    Terminal_foregroundBright = bright;
}

void Terminal_setBackgroundColor(Terminal_Color color, Bool bright)
{
    Terminal_backgroundColor = color;
    Terminal_backgroundBright = bright;
}

void Terminal_getForegroundColor(Terminal_Color *color, Bool *bright)
{
    *color = Terminal_foregroundColor;
    *bright = Terminal_foregroundBright;
}

void Terminal_getBackgroundColor(Terminal_Color *color, Bool *bright)
{
    *color = Terminal_backgroundColor;
    *bright = Terminal_backgroundBright;
}

void Terminal_setCursorX(u8 x)
{
    Terminal_cursor.x = x;
    Terminal_updateCursor();
}

void Terminal_setCursorY(u8 y)
{
    Terminal_cursor.y = y;
    Terminal_updateCursor();
}

u8 Terminal_getCursorX()
{
    return Terminal_cursor.x;
}

u8 Terminal_getCursorY()
{
    return Terminal_cursor.y;
}

void Terimal_hideCursor()
{
    Kernel_outportb(0x3D4, 0x0A);
    Kernel_outportb(0x3D5, 16);
}

void Terminal_showCursor()
{
    Kernel_outportb(0x3D4, 0x0A);
    Kernel_outportb(0x3D5, Terminal_cursorHeight);
}

void Terminal_setCursorHeight(u32 height)
{
    Terminal_cursorHeight = 16 - height;
    Terminal_showCursor();
}

u32 Terminal_getCursorHeight()
{
    return 16 - Terminal_cursorHeight;
}

Bool Terminal_initialize()
{
    Terminal_clear();
    return true;
}
