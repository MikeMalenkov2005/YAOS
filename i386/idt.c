#include <idt.h>

#include <pic.h>

struct idt_entry IDT[256] __attribute__((aligned(16)));

isr_t ISR_TABLE[32] __attribute__((aligned(16)));

void isr_handler(struct isr_frame* frame) {
  isr_t isr = ISR_TABLE[frame->index];
  if (isr) isr(frame);
}

__attribute__((naked))
void isr_common() {
  asm volatile ("pusha");
  asm volatile ("push %esp");
  asm volatile ("call %P0" : : "i"(isr_handler));
  asm volatile ("add $4, %esp");
  asm volatile ("popa");
  asm volatile ("add $8, %esp");
  asm volatile ("iret");
}

__attribute__((naked))
void irq_common() {
  asm volatile ("pusha");
  asm volatile ("push %esp");
  asm volatile ("call %P0" : : "i"(irq_handler));
  asm volatile ("add $4, %esp");
  asm volatile ("popa");
  asm volatile ("add $4, %esp");
  asm volatile ("iret");
}

#define ISR_NOCODE(n)                           \
__attribute__((naked))                          \
void isr##n() {                                 \
  asm volatile ("push %0" : : "N"(0));          \
  asm volatile ("push %0" : : "N"(n));          \
  asm volatile ("jmp %P0" : : "i"(isr_common)); \
}

#define ISR_WITHCODE(n)                         \
__attribute__((naked))                          \
void isr##n() {                                 \
  asm volatile ("push %0" : : "N"(n));          \
  asm volatile ("jmp %P0" : : "i"(isr_common)); \
}

#define IRQ(n)                                  \
__attribute__((naked))                          \
void irq##n() {                                 \
  asm volatile ("push %0" : : "N"(n));          \
  asm volatile ("jmp %P0" : : "i"(irq_common)); \
}

ISR_NOCODE(0)
ISR_NOCODE(1)
ISR_NOCODE(2)
ISR_NOCODE(3)
ISR_NOCODE(4)
ISR_NOCODE(5)
ISR_NOCODE(6)
ISR_NOCODE(7)
ISR_WITHCODE(8)
ISR_NOCODE(9)
ISR_WITHCODE(10)
ISR_WITHCODE(11)
ISR_WITHCODE(12)
ISR_WITHCODE(13)
ISR_WITHCODE(14)
ISR_NOCODE(15)
ISR_NOCODE(16)
ISR_WITHCODE(17)
ISR_NOCODE(18)
ISR_NOCODE(19)
ISR_NOCODE(20)
ISR_WITHCODE(21)
ISR_NOCODE(22)
ISR_NOCODE(23)
ISR_NOCODE(24)
ISR_NOCODE(25)
ISR_NOCODE(26)
ISR_NOCODE(27)
ISR_NOCODE(28)
ISR_WITHCODE(29)
ISR_WITHCODE(30)
ISR_NOCODE(31)

IRQ(0)
IRQ(1)
IRQ(2)
IRQ(3)
IRQ(4)
IRQ(5)
IRQ(6)
IRQ(7)
IRQ(8)
IRQ(9)
IRQ(10)
IRQ(11)
IRQ(12)
IRQ(13)
IRQ(14)
IRQ(15)

void init_idt(uint16_t kcode_segment) {
  set_idt_entry(0, (uint32_t)isr0, kcode_segment, 0x8E);
  set_idt_entry(1, (uint32_t)isr1, kcode_segment, 0x8E);
  set_idt_entry(2, (uint32_t)isr2, kcode_segment, 0x8E);
  set_idt_entry(3, (uint32_t)isr3, kcode_segment, 0x8E);
  set_idt_entry(4, (uint32_t)isr4, kcode_segment, 0x8E);
  set_idt_entry(5, (uint32_t)isr5, kcode_segment, 0x8E);
  set_idt_entry(6, (uint32_t)isr6, kcode_segment, 0x8E);
  set_idt_entry(7, (uint32_t)isr7, kcode_segment, 0x8E);
  set_idt_entry(8, (uint32_t)isr8, kcode_segment, 0x8E);
  set_idt_entry(9, (uint32_t)isr9, kcode_segment, 0x8E);
  set_idt_entry(10, (uint32_t)isr10, kcode_segment, 0x8E);
  set_idt_entry(11, (uint32_t)isr11, kcode_segment, 0x8E);
  set_idt_entry(12, (uint32_t)isr12, kcode_segment, 0x8E);
  set_idt_entry(13, (uint32_t)isr13, kcode_segment, 0x8E);
  set_idt_entry(14, (uint32_t)isr14, kcode_segment, 0x8E);
  set_idt_entry(15, (uint32_t)isr15, kcode_segment, 0x8E);
  set_idt_entry(16, (uint32_t)isr16, kcode_segment, 0x8E);
  set_idt_entry(17, (uint32_t)isr17, kcode_segment, 0x8E);
  set_idt_entry(18, (uint32_t)isr18, kcode_segment, 0x8E);
  set_idt_entry(19, (uint32_t)isr19, kcode_segment, 0x8E);
  set_idt_entry(20, (uint32_t)isr20, kcode_segment, 0x8E);
  set_idt_entry(21, (uint32_t)isr21, kcode_segment, 0x8E);
  set_idt_entry(22, (uint32_t)isr22, kcode_segment, 0x8E);
  set_idt_entry(23, (uint32_t)isr23, kcode_segment, 0x8E);
  set_idt_entry(24, (uint32_t)isr24, kcode_segment, 0x8E);
  set_idt_entry(25, (uint32_t)isr25, kcode_segment, 0x8E);
  set_idt_entry(26, (uint32_t)isr26, kcode_segment, 0x8E);
  set_idt_entry(27, (uint32_t)isr27, kcode_segment, 0x8E);
  set_idt_entry(28, (uint32_t)isr28, kcode_segment, 0x8E);
  set_idt_entry(29, (uint32_t)isr29, kcode_segment, 0x8E);
  set_idt_entry(30, (uint32_t)isr30, kcode_segment, 0x8E);
  set_idt_entry(31, (uint32_t)isr31, kcode_segment, 0x8E);
  set_idt_entry(32, (uint32_t)irq0, kcode_segment, 0x8E);
  set_idt_entry(33, (uint32_t)irq1, kcode_segment, 0x8E);
  set_idt_entry(34, (uint32_t)irq2, kcode_segment, 0x8E);
  set_idt_entry(35, (uint32_t)irq3, kcode_segment, 0x8E);
  set_idt_entry(36, (uint32_t)irq4, kcode_segment, 0x8E);
  set_idt_entry(37, (uint32_t)irq5, kcode_segment, 0x8E);
  set_idt_entry(38, (uint32_t)irq6, kcode_segment, 0x8E);
  set_idt_entry(39, (uint32_t)irq7, kcode_segment, 0x8E);
  set_idt_entry(40, (uint32_t)irq8, kcode_segment, 0x8E);
  set_idt_entry(41, (uint32_t)irq9, kcode_segment, 0x8E);
  set_idt_entry(42, (uint32_t)irq10, kcode_segment, 0x8E);
  set_idt_entry(43, (uint32_t)irq11, kcode_segment, 0x8E);
  set_idt_entry(44, (uint32_t)irq12, kcode_segment, 0x8E);
  set_idt_entry(45, (uint32_t)irq13, kcode_segment, 0x8E);
  set_idt_entry(46, (uint32_t)irq14, kcode_segment, 0x8E);
  set_idt_entry(47, (uint32_t)irq15, kcode_segment, 0x8E);
  lidt(IDT, sizeof(IDT) - 1);
  init_pic(32);
  sti();
}

void set_idt_entry(int index, uint32_t offset, uint16_t segment, uint8_t attributes) {
  IDT[index].offset_low = offset & 0xFFFF;
  IDT[index].selector = segment;
  IDT[index].reserved = 0;
  IDT[index].attributes = attributes;
  IDT[index].offset_high = offset >> 16;
}

void set_cpu_isr(int index, isr_t isr) {
  if (index < 32) ISR_TABLE[index] = isr;
}
