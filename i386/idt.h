#ifndef IDT_H
#define IDT_H

#include <cpu.h>

#include <attributes.h>

inline static void lidt(void* base, uint16_t limit) {
  struct {
    uint16_t limit;
    void* base;
  }__packed idtr = { limit, base };
  asm volatile ("lidt %0" : : "m"(idtr));
}

struct isr_frame {
  struct saved_registers state;
  size_t index;
  size_t error;
  struct interrupt_frame frame;
};

typedef void (*isr_t)(struct isr_frame*);

struct idt_entry {
  uint16_t offset_low;
  uint16_t selector;
  uint8_t reserved;
  uint8_t attributes;
  uint16_t offset_high;
};

void init_idt(uint16_t kcode_segment);

void set_idt_entry(int index, uint32_t offset, uint16_t segment, uint8_t attributes);

void set_cpu_isr(int index, isr_t isr);

#endif
