# Keyboard.h

Functions for getting input from the keyboard.

## `enum Keyboard_EventType`

    typedef enum
    {
        Keyboard_EventType_down,
        Keyboard_EventType_up
    } Keyboard_EventType;

Describes the type of keyboard event. Pretty much self-explanatory.

## `struct Keyboard_Event`

    typedef struct
    {
        Keyboard_EventType type;
        u32 code;
        char character;
        Bool shiftPressed;
        Bool controlPressed;
        Bool altPressed;
    } Keyboard_Event;

Describes a keyboard event.

The `type` member is the type of keyboard event.

The `code` member is the scancode of the pressed key. This scancode
has been modified to be the same for both a key down even and a key up
event of the same key.

The `character` member is the character representation of the key
pressed on the current keymap. Non-printable keys will have a
`character` member value of 0.

The `shiftPressed`, `controlPressed` and `altPressed` members are the
pressed states of each modifier key.

## `Keyboard_Event *Keyboard_getEvent(Bool blocking)`

Gets a keyboard event from the queue and removes it from the queue. If
there are no events in the queue and `blocking` is `true`, it will
wait until there is an event before returning, otherwise it will
return `NULL`.

The key event returned is allocated on the heap and the caller of this
function is responsible for calling `Memory_free` on it.

## `char Keyboard_getChar(Bool echo)`

Gets a character input from the keyboard. Will block until a key that
is printable is released. If `echo` is `true`, the character will be
echoed using `Text_putChar`.

## `String Keyboard_getString(Bool echo)`

Gets a string of character input from the keyboard until a newline. If
`echo` is `true`, the text will be echoed using `Text_putChar` as it
is entered.

The string returned is allocated on the heap and the caller of this
function is responsible for calling `Memory_free` on it.
