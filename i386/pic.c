#include "pic.h"
#include "i386.h"

extern irq_handler *IRQ_HANDLER_TABLE;

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

void _set_pic_mask(uint8_t mpic_mask, uint8_t spic_mask) {
  outb(0x21, mpic_mask);
  outb(0xA1, spic_mask);
}

void _set_irq_handler(int index, irq_handler handler) {
  IRQ_HANDLER_TABLE[index] = handler;
}

