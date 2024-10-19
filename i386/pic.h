#ifndef PIC_H
#define PIC_H

#include <cpu.h>

struct irq_frame {
  struct saved_registers state;
  size_t index;
  struct interrupt_frame frame;
};

typedef void (*irqsr_t)(struct irq_frame*);

void init_pic(uint8_t pic1_base, uint8_t pic2_base);

void set_pic_mask(uint8_t pic1_mask, uint8_t pic2_mask);

void set_pic_irqsr(int index, irqsr_t irqsr);

void irq_handler(struct irq_frame* frame);

#endif
