# Text.h

Functions that abstract the `Terminal` and `Serial` text outputs.

These functions will output to every text output that has been
initialized. The functions call `Init_initialized` to determine if the
output has been initialized.

## `void Text_putChar(char c)`

Outputs a single character to all available text outputs.

## `void Text_putString(const String s)`

Outputs a string of characters to all available text outputs. Note
that this function calls `Terminal_putString` and `Serial_putString`
on `s`, and does not call `Text_putChar` on each character in `s`.
