#include "idt.h"
#include "pic.h"
#include <stdint.h>

__attribute__((aligned(0x1000)))
static struct idt_entry IDT[256];

extern void _isr0();
extern void _isr1();
extern void _isr2();
extern void _isr3();
extern void _isr4();
extern void _isr5();
extern void _isr6();
extern void _isr7();
extern void _isr8();
extern void _isr9();
extern void _isr10();
extern void _isr11();
extern void _isr12();
extern void _isr13();
extern void _isr14();
extern void _isr15();
extern void _isr16();
extern void _isr17();
extern void _isr18();
extern void _isr19();
extern void _isr20();
extern void _isr21();
extern void _isr22();
extern void _isr23();
extern void _isr24();
extern void _isr25();
extern void _isr26();
extern void _isr27();
extern void _isr28();
extern void _isr29();
extern void _isr30();
extern void _isr31();

extern void _irq0();
extern void _irq1();
extern void _irq2();
extern void _irq3();
extern void _irq4();
extern void _irq5();
extern void _irq6();
extern void _irq7();
extern void _irq8();
extern void _irq9();
extern void _irq10();
extern void _irq11();
extern void _irq12();
extern void _irq13();
extern void _irq14();
extern void _irq15();

extern void _sys();

void _init_idt() {
  _init_pic(32, 40);
  _set_idt_entry(0, (size_t)_isr0, KCODE, 0x8E);
  _set_idt_entry(1, (size_t)_isr1, KCODE, 0x8E);
  _set_idt_entry(2, (size_t)_isr2, KCODE, 0x8E);
  _set_idt_entry(3, (size_t)_isr3, KCODE, 0x8E);
  _set_idt_entry(4, (size_t)_isr4, KCODE, 0x8E);
  _set_idt_entry(5, (size_t)_isr5, KCODE, 0x8E);
  _set_idt_entry(6, (size_t)_isr6, KCODE, 0x8E);
  _set_idt_entry(7, (size_t)_isr7, KCODE, 0x8E);
  _set_idt_entry(8, (size_t)_isr8, KCODE, 0x8E);
  _set_idt_entry(9, (size_t)_isr9, KCODE, 0x8E);
  _set_idt_entry(10, (size_t)_isr10, KCODE, 0x8E);
  _set_idt_entry(11, (size_t)_isr11, KCODE, 0x8E);
  _set_idt_entry(12, (size_t)_isr12, KCODE, 0x8E);
  _set_idt_entry(13, (size_t)_isr13, KCODE, 0x8E);
  _set_idt_entry(14, (size_t)_isr14, KCODE, 0x8E);
  _set_idt_entry(15, (size_t)_isr15, KCODE, 0x8E);
  _set_idt_entry(16, (size_t)_isr16, KCODE, 0x8E);
  _set_idt_entry(17, (size_t)_isr17, KCODE, 0x8E);
  _set_idt_entry(18, (size_t)_isr18, KCODE, 0x8E);
  _set_idt_entry(19, (size_t)_isr19, KCODE, 0x8E);
  _set_idt_entry(20, (size_t)_isr20, KCODE, 0x8E);
  _set_idt_entry(21, (size_t)_isr21, KCODE, 0x8E);
  _set_idt_entry(22, (size_t)_isr22, KCODE, 0x8E);
  _set_idt_entry(23, (size_t)_isr23, KCODE, 0x8E);
  _set_idt_entry(24, (size_t)_isr24, KCODE, 0x8E);
  _set_idt_entry(25, (size_t)_isr25, KCODE, 0x8E);
  _set_idt_entry(26, (size_t)_isr26, KCODE, 0x8E);
  _set_idt_entry(27, (size_t)_isr27, KCODE, 0x8E);
  _set_idt_entry(28, (size_t)_isr28, KCODE, 0x8E);
  _set_idt_entry(29, (size_t)_isr29, KCODE, 0x8E);
  _set_idt_entry(30, (size_t)_isr30, KCODE, 0x8E);
  _set_idt_entry(31, (size_t)_isr31, KCODE, 0x8E);
  _set_idt_entry(32, (size_t)_irq0, KCODE, 0x8E);
  _set_idt_entry(33, (size_t)_irq1, KCODE, 0x8E);
  _set_idt_entry(34, (size_t)_irq2, KCODE, 0x8E);
  _set_idt_entry(35, (size_t)_irq3, KCODE, 0x8E);
  _set_idt_entry(36, (size_t)_irq4, KCODE, 0x8E);
  _set_idt_entry(37, (size_t)_irq5, KCODE, 0x8E);
  _set_idt_entry(38, (size_t)_irq6, KCODE, 0x8E);
  _set_idt_entry(39, (size_t)_irq7, KCODE, 0x8E);
  _set_idt_entry(40, (size_t)_irq8, KCODE, 0x8E);
  _set_idt_entry(41, (size_t)_irq9, KCODE, 0x8E);
  _set_idt_entry(42, (size_t)_irq10, KCODE, 0x8E);
  _set_idt_entry(43, (size_t)_irq11, KCODE, 0x8E);
  _set_idt_entry(44, (size_t)_irq12, KCODE, 0x8E);
  _set_idt_entry(45, (size_t)_irq13, KCODE, 0x8E);
  _set_idt_entry(46, (size_t)_irq14, KCODE, 0x8E);
  _set_idt_entry(47, (size_t)_irq15, KCODE, 0x8E);
  _set_idt_entry(0x80, (size_t)_sys, KCODE, 0x8F);
  lidt(IDT, sizeof(IDT) - 1);
  sti;
}

void _set_idt_entry(uint8_t index, size_t offset, uint16_t selector, uint8_t attributes) {
  IDT[index].reserved = 0;
  IDT[index].selector = selector;
  IDT[index].offset_low = offset & 0xFFFF;
  IDT[index].offset_high = offset >> 16;
  IDT[index].attributes = attributes & 0xEF;
}

void _isr_handler(struct interrupt_frame *frame, struct registers *regs, int isr_index, int error_code) {
}

