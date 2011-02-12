#include <Terminal.h>

#include <Init.h>

#define TTY_COLS 80
#define TTY_ROWS 25

#define Terminal_assertInstalled() Kernel_assert(Terminal_ram == (u16*) 0xB8000, "Terminal not installed")

u16 *Terminal_ram;

Terminal_Color Terminal_foregroundColor;
Bool Terminal_foregroundBright;
Terminal_Color Terminal_backgroundColor;
Bool Terminal_backgroundBright;

struct { u8 x; u8 y; } Terminal_cursor;

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
    Terminal_assertInstalled();

    u16 blank = ' ' | ((((Terminal_backgroundColor | (Terminal_backgroundBright << 3)) << 4) | ((Terminal_foregroundColor | (Terminal_foregroundBright << 3)) & 0x0F)) << 8);
    for (u8 i = 0; i < TTY_ROWS; i++)
        String_setWord(Terminal_ram + i * TTY_COLS, blank, TTY_COLS);
    Terminal_cursor.x = 0;
    Terminal_cursor.y = 0;
    Terminal_updateCursor();
}

void Terminal_scroll()
{
    Terminal_assertInstalled();

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
    Terminal_assertInstalled();

    switch (c)
    {
    case '\b':
        Terminal_moveBack();
        Terminal_putChar(' ');
        Terminal_moveBack();
        return;
    case '\t':
        Terminal_cursor.x -= (Terminal_cursor.x % 8) + 8;
        break;
    case '\n':
        Terminal_cursor.y++;
    case '\r':
        Terminal_cursor.x = 0;
        break;
    default:
    {
        u16 attributes = ((Terminal_backgroundColor | (Terminal_backgroundBright << 3)) << 4) | ((Terminal_foregroundColor | (Terminal_foregroundBright << 3)) & 0x0F);
        u16 *location = Terminal_ram + (Terminal_cursor.y * TTY_COLS + Terminal_cursor.x);
        *location = c | (attributes << 8);
        Terminal_cursor.x++;
    }
    }

    /* EOL */
    if (Terminal_cursor.x >= TTY_COLS)
    {
        Terminal_cursor.x -= TTY_COLS;
        Terminal_cursor.y++;
    }

    Terminal_scroll();
    Terminal_updateCursor();
}

void Terminal_putString(const String s)
{
    for (u32 i = 0; i < String_length(s); i++)
        Terminal_putChar(s[i]);
}

Bool Terminal_initialize()
{
    Terminal_ram = (u16*) 0xB8000;
    Terminal_foregroundColor = Terminal_Color_gray;
    Terminal_foregroundBright = false;
    Terminal_backgroundColor = Terminal_Color_black;
    Terminal_backgroundBright = false;
    Terminal_clear();
    return true;
}
