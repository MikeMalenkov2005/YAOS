#include "pic.h"
#include "i386.h"

__attribute__((aligned(16)))
static irq_handler IRQ_HANDLER_TABLE[16];

void _init_pic(uint8_t mpic_remap, uint8_t spic_remap) {
  outb(0x20, 0x11);
  outb(0xA0, 0x11);
  outb(0x21, mpic_remap);
  outb(0xA1, spic_remap);
  outb(0x21, 4);
  outb(0xA1, 2);
  outb(0x21, 1);
  outb(0xA1, 1);
  outb(0x21, 0xFF);
  outb(0xA1, 0xFF);
}

void _irq_handler(struct interrupt_frame *frame, struct registers *regs, int irq_index) {
  irq_handler handler = IRQ_HANDLER_TABLE[irq_index];
  if (handler) handler(frame, regs);
  if (irq_index > 7) outb(0xA0, 0x20);
  outb(0x20, 0x20);
}

void _set_pic_mask(uint8_t mpic_mask, uint8_t spic_mask) {
  outb(0x21, mpic_mask);
  outb(0xA1, spic_mask);
}

void _set_irq_handler(int index, irq_handler handler) {
  IRQ_HANDLER_TABLE[index] = handler;
}

