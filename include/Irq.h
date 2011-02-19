#ifndef __IRQ_H__
#define __IRQ_H__

#include <Isr.h>

void Irq_installHandler(u32 irq, void (*handler)(Isr_Registers*));
void Irq_uninstallHandler(u32 irq);

Bool Irq_initialize();

#endif
