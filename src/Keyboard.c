#include <Keyboard.h>

#include <Memory.h>
#include <Irq.h>
#include <Text.h>
#include <Threading.h>

Bool Keyboard_leftShiftPressed = false;
Bool Keyboard_rightShiftPressed = false;
Bool Keyboard_controlPressed = false;
Bool Keyboard_altPressed = false;

/* TODO: Multiple keymaps */
char Keyboard_keyMap[128] =
{
    0, 27,
    '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, /* Control */
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'',
    '`',
    0, /* Left shift */
    '\\',
    'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',
    0, /* Right shift */
    '*',
    0, /* Alt */
    ' ',
    0, /* Caps lock */
    '\x83', '\x84', '\x85', '\x86', '\x87', '\x88', '\x89', '\x8A', '\x8B', /* F1 - F10 */
    0, /* Num lock */
    0, /* Scroll lock */
    '\x8E', /* Home */
    '\x8F', /* Up */
    '\x90', /* Page up */
    '-',
    '\x91', /* Left */
    0,
    '\x92', /* Right */
    '+',
    '\x93', /* End */
    '\x94', /* Down */
    '\x95', /* Page down */
    '\x96', /* Insert */
    '\x97', /* Delete */
    0, 0, 0,
    '\x98', /* F11 */
    '\x99', /* F12 */
    0,
};

char Keyboard_keyMapShift[128] =
{
    0, 27,
    '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
    '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
    0, /* Control */
    'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"',
    '~',
    0, /* Left shift */
    '|',
    'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?',
    0, /* Right shift */
    '*',
    0, /* Alt */
    ' ',
    0, /* Caps lock */
    '\x83', '\x84', '\x85', '\x86', '\x87', '\x88', '\x89', '\x8A', '\x8B', /* F1 - F10 */
    0, /* Num lock */
    0, /* Scroll lock */
    '\x8E', /* Home */
    '\x8F', /* Up */
    '\x90', /* Page up */
    '-',
    '\x91', /* Left */
    0,
    '\x92', /* Right */
    '+',
    '\x93', /* End */
    '\x94', /* Down */
    '\x95', /* Page down */
    '\x96', /* Insert */
    '\x97', /* Delete */
    0, 0, 0,
    '\x98', /* F11 */
    '\x99', /* F12 */
    0,
};

typedef struct Keyboard_EventQueue
{
    Keyboard_Event *event;
    struct Keyboard_EventQueue *previous;
} Keyboard_EventQueue;

Keyboard_EventQueue *Keyboard_EventQueue_head = NULL;
Keyboard_EventQueue *Keyboard_EventQueue_tail = NULL;

void Keyboard_EventQueue_enqueue(Keyboard_Event *event)
{
    Keyboard_EventQueue *new = Memory_allocate(sizeof(Keyboard_EventQueue));
    new->event = event;
    new->previous = NULL;
    if (Keyboard_EventQueue_tail)
    {
        Keyboard_EventQueue_tail->previous = new;
        Keyboard_EventQueue_tail = new;
    }
    else
    {
        Keyboard_EventQueue_head = new;
        Keyboard_EventQueue_tail = new;
    }
}


Keyboard_Event *Keyboard_EventQueue_dequeue()
{
    if (!Keyboard_EventQueue_head)
        return NULL;
    Keyboard_EventQueue *head = Keyboard_EventQueue_head;
    Keyboard_Event *event = head->event;
    Keyboard_EventQueue_head = head->previous;
    if (Keyboard_EventQueue_tail == head)
        Keyboard_EventQueue_tail = NULL;
    Memory_free(head);
    return event;
}

void Keyboard_handler(Isr_Registers *r)
{
    u8 keyCode = Kernel_inportb(0x60);

    Keyboard_Event *event = Memory_allocate(sizeof(Keyboard_Event));

    if (keyCode & 0x80)
    {
        event->type = Keyboard_EventType_up;
        keyCode = keyCode & ~0x80;
    }
    else
        event->type = Keyboard_EventType_down;

    event->code = keyCode;
    event->character = (Keyboard_leftShiftPressed || Keyboard_rightShiftPressed) ? Keyboard_keyMapShift[keyCode] : Keyboard_keyMap[keyCode];
    event->shiftPressed = Keyboard_leftShiftPressed || Keyboard_rightShiftPressed;
    event->controlPressed = Keyboard_controlPressed;
    event->altPressed = Keyboard_altPressed;

    switch (event->code)
    {
    case 0x2A:
        Keyboard_leftShiftPressed = !event->type;
        break;
    case 0x36:
        Keyboard_rightShiftPressed = !event->type;
        break;
    case 0x1D:
        Keyboard_controlPressed = !event->type;
        break;
    case 0x38:
        Keyboard_altPressed = !event->type;
        break;
    }

    Keyboard_EventQueue_enqueue(event);
}

Keyboard_Event *Keyboard_getEvent(Bool blocking)
{
    if (blocking)
        while (!Keyboard_EventQueue_head)
            Threading_yield();
    return Keyboard_EventQueue_dequeue();
}

char Keyboard_getChar(Bool echo)
{
    Keyboard_Event *event = Keyboard_getEvent(true);
    while (!event || !event->character || event->type == Keyboard_EventType_up)
    {
        Memory_free(event);
        event = Keyboard_getEvent(true);
    }
    char character = event->character;
    if (echo)
        Text_putChar(character);
    Memory_free(event);
    return character;
}

String Keyboard_getString(Bool echo)
{
    u32 length = 16;
    String s = Memory_allocate(length);
    for (u32 i = 0;; i++)
    {
        char c = Keyboard_getChar(echo);
        if (i >= length)
            s = Memory_reallocate(s, (length += 16));
        if (c == '\n')
        {
            s[i] = 0;
            break;
        }
        if (c == '\b')
        {
            i -= (i == 0) ? 0 : 2;
            continue;
        }
        s[i] = c;
    }
    return s;
}

Bool Keyboard_initialize()
{
    Irq_installHandler(1, Keyboard_handler);
    return true;
}
