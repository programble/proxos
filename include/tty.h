#ifndef __TTY_H__
#define __TTY_H__

#include <kernel.h>
#include <string.h>

#define TTY_ROWS 25
#define TTY_COLS 80

typedef enum
{
    black,
    blue,
    green,
    cyan,
    red,
    magenta,
    brown,
    gray
} color;

void tty_install();

void tty_clear();
void tty_putch(u8 c);
void tty_puts(const string s);

void tty_set_fg_color(color c, bool bright);
void tty_set_bg_color(color c, bool bright);
#define tty_set_color(fg, fb, bg, bb) tty_set_fg_color(fg, fb); tty_set_bg_color(bg, bb)

void tty_get_fg_color(color *c, bool *bright);
void tty_get_bg_color(color *c, bool *bright);
#define tty_get_color(fg, gb, bg, bb) tty_get_fg_color(fg, fb); tty_get_bg_color(bg, bb)

void tty_set_cursor_x(u8 x);
void tty_set_cursor_y(u8 y);
#define tty_set_cursor_pos(x, y) tty_set_cursor_x(x); tty_set_cursor_y(y)

u8 tty_get_cursor_x();
u8 tty_get_cursor_y();

void tty_hide_cursor();
void tty_show_cursor();

void tty_set_cursor_height(u32 height);
u32 tty_get_cursor_height();

#endif
