# Serial.h

Functions for text output over serial port.

All functions operate on the COM1 port.

These functions are used for debug output using serial redirection in Qemu.

## `void Serial_putChar(char c)`

Outputs a single character to the COM1 serial port.

## `void Serial_putString(const String s)`

Outputs a string of characters to the COM1 serial port
by calling `Serial_putChar` on each character.
