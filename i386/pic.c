#include <pic.h>

#include <io.h>

#include <attributes.h>

irqsr_t IRQSR_TABLE[16] __aligned(16);

void init_pic(uint8_t base) {
  outb(0x20, 0x11);
  pio_delay();
  outb(0xA0, 0x11);
  pio_delay();
  outb(0x21, base);
  pio_delay();
  outb(0xA1, base + 8);
  pio_delay();
  outb(0x21, 4);
  pio_delay();
  outb(0xA1, 2);
  pio_delay();
  outb(0x21, 1);
  pio_delay();
  outb(0xA1, 1);
  pio_delay();
  outb(0x21, 0xFF);
  outb(0xA1, 0xFF);
}

void set_pic_mask(uint16_t mask) {
  outb(0x21, mask & 0xFF);
  outb(0xA1, mask >> 8);
}

uint16_t get_pic_mask(void) {
  return inb(0x21) | ((uint16_t)inb(0xA1) << 8);
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
