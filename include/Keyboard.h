#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#include <Kernel.h>

typedef enum
{
    Keyboard_EventType_down,
    Keyboard_EventType_up
} Keyboard_EventType;

typedef struct
{
    Keyboard_EventType type;
    u32 code;
    char character;
    Bool shiftPressed;
    Bool controlPressed;
    Bool altPressed;
} Keyboard_Event;

Bool Keyboard_initialize();

Keyboard_Event *Keyboard_getEvent(Bool blocking);

char Keyboard_getChar(Bool echo);
String Keyboard_getString(Bool echo);

#endif
