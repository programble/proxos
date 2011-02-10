#include <tty.h>

bool recursive_panic = false;

void _panic(const string message, const string function, const string file, const string line)
{
    if (recursive_panic)
        halt();
    recursive_panic = true;
    tty_puts("\n\nColorless green ideas sleep furiously\n\n");
    tty_puts(message);
    tty_puts("\n\n");
    tty_puts(function);
    tty_puts("@");
    tty_puts(file);
    tty_puts(":");
    tty_puts(line);
    halt();
}
