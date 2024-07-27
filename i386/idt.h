#ifndef IDT_H
#define IDT_H

#include "i386.h"

void _init_idt();
void _set_idt_entry(uint8_t index, size_t offset, uint16_t selector, uint8_t attributes);
void _isr_handler(struct interrupt_frame *frame, struct registers *regs, int isr_index, int error_code);

#endif
