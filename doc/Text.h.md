# Text.h

Functions that abstract the `Terminal` and `Serial` text outputs.

These functions will output to every text output that has been
initialized. The functions call `Init_initialized` to determine if the
output has been initialized.

## `void Text_printChar(char c)`

Outputs a single character to all available text outputs.

## `void Text_printString(const String s)`

Outputs a string of characters to all available text outputs. Note
that this function calls `Terminal_printString` and
`Serial_printString` on `s`, and does not call `Text_printChar` on
each character in `s`.
