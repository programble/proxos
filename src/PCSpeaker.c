#include <PCSpeaker.h>

#include <Time.h>

void PCSpeaker_setFrequency(u32 frequency)
{
    u32 temp = 1193180 / frequency;
    Kernel_outportb(0x43, 0xB6);
    Kernel_outportb(0x42, temp);
    Kernel_outportb(0x42, temp >> 8);
}

void PCSpeaker_start()
{
    u32 state = Kernel_inportb(0x61);
    if (state != (state | 0x3))
        Kernel_outportb(0x61, state | 0x3);
}

void PCSpeaker_stop()
{
    Kernel_outportb(0x61, Kernel_inportb(0x61) & 0xFC);
}

void PCSpeaker_beep(u32 frequency, u32 duration)
{
    PCSpeaker_setFrequency(frequency);
    PCSpeaker_start();
    Time_sleep(duration);
    PCSpeaker_stop();
}
