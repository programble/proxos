# Terminal.h

Functions for interacting with the hardware terminal.

## `enum Terminal_Color`

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

Enumeration of all colors the hardware terminal is capable of
displaying.

## `void Terminal_clear()`

Clears the hardware terminal by filling it with space characters using
the current colors and sets the hardware cursor position to the top
left corner.

## `void Terminal_printChar(char c)`

Outputs a character `c` to the hardware terminal at the hardware
cursor and advances the cursor by one.

## `void Terminal_printString(const String s)`

Outputs the string `s` to the hardware terminal by calling
`Terminal_printChar` on each character.

## `void Terminal_putChar(char c, u8 x, u8 y)`

Outputs the character `c` to the hardware terminal at the position
`(x, y)`.

## `void Terminal_putString(const String s, u8 x, u8 y)`

Outputs the string `s` to the hardware terminal at the position `(x,
y)`.

## `void Terminal_setForegroundColor(Terminal_Color color, Bool bright)`

Sets the current foreground color of the hardware terminal to
`color`. This will be used when outputting characters to the
terminal. The `bright` parameter determines if the color should be
bright or not.

## `void Terminal_setBackgroundColor(Terminal_Color color, Bool bright)`

Sets the current background color of the hardware terminal to
`color`. This will be used when outputting characters to the
terminal. The `bright` parameter determines if the color should be
bright or not.

## `#define Terminal_setColor(fg, fb, bg, bb) Terminal_setForegroundColor(fg, fb); Terminal_setBackgroundColor(bg, bb)`

## `void Terminal_getForegroundColor(Terminal_Color *color, Bool *bright)`

Sets the values of `*color` and `*bright` to the current foreground
color and brightness of the hardware terminal.

## `void Terminal_getBackgroundColor(Terminal_Color *color, Bool *bright)`

Sets the values of `*color` and `*bright` to the current background
color and brightness of the hardware terminal.

## `#define Terminal_getColor(fg, gb, bg, bb) Terminal_getForegroundColor(fg, fb); Terminal_getBackgroundColor(bg, bb)`

## `void Terminal_setCursorX(u8 x)`

Sets the X position of the hardware cursor to `x`.

## `void Terminal_setCursorY(u8 y)`

Sets the Y position of the hardware cursor to `y`.

## `#define Terminal_setCursorPosition(x, y) Terminal_setCursorX(x); Terminal_setCursorY(y)`

## `u8 Terminal_getCursorX()`

Gets the current X position of the hardware cursor.

## `u8 Terminal_getCursorY()`

Gets the current Y position of the hardware cursor.

## `void Terminal_hideCursor()`

Hides the hardware cursor.

## `void Terminal_showCursor()`

Shows the hardware cursor with the default height.

## `void Terminal_setCursorHeight(u32 height)`

Sets the height of the hardware cursor to `height`.

## `u32 Terminal_getCursorHeight()`

Gets the height of the hardware cursor.
