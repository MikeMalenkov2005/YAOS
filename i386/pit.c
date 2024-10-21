#include <pit.h>

#include <pic.h>
#include <io.h>

/* NOT READY */

void pit_irqsr(struct irq_frame* frame) {
}

void init_pit(uint32_t frequency) {
  set_pic_irqsr(0, pit_irqsr);
  if (frequency) {
    uint32_t divider = PIT_BASE_FREQUENCY / frequency;
    divider = divider > UINT16_MAX ? 0 : (divider == 1 ? 2 : divider);
    outb(0x43, 0x34);
    outb(0x40, divider & 0xFF);
    outb(0x40, divider >> 8);
  }
  set_pic_mask(get_pic_mask() & ~1);
}

void pit_sound(uint32_t frequency) {
  if (frequency) {
    uint32_t divider = PIT_BASE_FREQUENCY / frequency;
    divider = divider > UINT16_MAX ? 0 : (divider == 1 ? 2 : divider);
    outb(0x43, 0xB6);
    outb(0x42, divider & 0xFF);
    outb(0x42, divider >> 8);
    outb(0x61, inb(0x61) | 3);
  }
  else outb(0x61, inb(0x61) & 0xFC);
}

