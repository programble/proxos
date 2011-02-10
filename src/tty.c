#include <tty.h>

#define assert_tty_installed() assert(tty_ram, "TTY not installed")

u16 *tty_ram;

color tty_fg_color;
bool tty_fg_bright;
color tty_bg_color;
bool tty_bg_bright;

struct { u8 x; u8 y; } cursor;

void tty_update_cursor()
{
    u32 temp = cursor.y * TTY_COLS + cursor.x;
    outportb(0x3D4, 14);
    outportb(0x3D5, temp >> 8);
    outportb(0x3D4, 15);
    outportb(0x3D5, temp);
}

void tty_clear()
{
    assert_tty_installed();

    u16 blank = ' ' | ((((tty_bg_color | (tty_bg_bright << 3)) << 4) | ((tty_fg_color | (tty_fg_bright << 3)) & 0x0F)) << 8);
    for (u8 i = 0; i < TTY_ROWS; i++)
        memsetw(tty_ram + i * TTY_COLS, blank, TTY_COLS);
    cursor.x = 0;
    cursor.y = 0;
    tty_update_cursor();
}

void tty_scroll()
{
    assert_tty_installed();

    u16 blank = ' ' | ((((tty_bg_color | (tty_bg_bright << 3)) << 4) | ((tty_fg_color | (tty_fg_bright << 3)) & 0x0F)) << 8);

    if (cursor.y >= TTY_ROWS)
    {
        /* Move everything up one line */
        u32 position = cursor.y - TTY_ROWS + 1;
        memcpy((u8*) tty_ram, (u8*) (tty_ram + position * TTY_COLS), (TTY_ROWS - position) * TTY_COLS * 2);
        /* Blank the last line */
        memsetw(tty_ram + (TTY_ROWS - position) * TTY_COLS, blank, TTY_COLS);
        cursor.y = TTY_ROWS - 1;
    }
}

void tty_backspace()
{
    /* If at beginning of line, move up one line */
    if (cursor.x == 0)
    {
        cursor.y--;
        cursor.x = TTY_COLS -1;
    }
    /* otherwise, move back one */
    else
        cursor.x--;
    tty_update_cursor();
}

void tty_putch(u8 c)
{
    assert_tty_installed();

    switch (c)
    {
    case '\b':
        tty_backspace();
        tty_putch(' ');
        tty_backspace();
        return;
    case '\t':
        cursor.x -= (cursor.x % 8) + 8;
        break;
    case '\n':
        cursor.y++;
    case '\r':
        cursor.x = 0;
        break;
    default:
    {
        u16 attributes = ((tty_bg_color | (tty_bg_bright << 3)) << 4) | ((tty_fg_color | (tty_fg_bright << 3)) & 0x0F);
        u16 *location = tty_ram + (cursor.y * TTY_COLS + cursor.x);
        *location = c | (attributes << 8);
        cursor.x++;
    }
    }

    /* EOL */
    if (cursor.x >= TTY_COLS)
    {
        cursor.x -= TTY_COLS;
        cursor.y++;
    }

    tty_scroll();
    tty_update_cursor();
}

void tty_puts(const string s)
{
    for (u32 i = 0; i < strlen(s); i++)
        tty_putch(s[i]);
}

void tty_install()
{
    tty_ram = (u16*) 0xB8000;
    tty_fg_color = gray;
    tty_fg_bright = false;
    tty_bg_color = black;
    tty_bg_bright = false;
    tty_clear();
    tty_puts(":: TTY installed\n");
}
