#include <pic.h>

#include <io.h>

irqsr_t IRQSR_TABLE[16] __attribute__(());

void init_pic(uint8_t pic1_base, uint8_t pic2_base) {
  outb(0x20, 0x11);
  outb(0xA0, 0x11);
  outb(0x21, pic1_base);
  outb(0xA1, pic2_base);
  outb(0x21, 4);
  outb(0xA1, 2);
  outb(0x21, 1);
  outb(0xA1, 1);
  outb(0x21, 0xFF);
  outb(0xA1, 0xFF);
}

void set_pic_mask(uint8_t pic1_mask, uint8_t pic2_mask) {
  outb(0x21, pic1_mask);
  outb(0xA1, pic2_mask);
}

void set_pic_irqsr(int index, irqsr_t irqsr) {
  if (index < 16) IRQSR_TABLE[index] = irqsr;
}

void irq_handler(struct irq_frame* frame) {
  size_t index = frame->index;
  if (index < 16) {
    irqsr_t irqsr = IRQSR_TABLE[index];
    if (irqsr) irqsr(frame);
    if (index > 7) outb(0xA0, 0x20);
    outb(0x20, 0x20);
  }
}
