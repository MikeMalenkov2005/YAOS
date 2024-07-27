#ifndef PIC_H
#define PIC_H

#include <stdint.h>

typedef void (*irq_handler)(void*, void*);

void _init_pic(uint8_t mpic_remap, uint8_t spic_remap);
void _set_pic_mask(uint8_t mpic_mask, uint8_t spic_mask);
void _set_irq_handler(int index, irq_handler handler);

#endif

