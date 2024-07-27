#ifndef I386_H
#define I386_H

#include <stddef.h>
#include <stdint.h>

inline static uint8_t inb(uint16_t port) {
  uint8_t ret;
  __asm__ volatile ("inb %w1, %b0" : "=a"(ret) : "Nd"(port) : "memory");
  return ret;
}

inline static void outb(uint16_t port, uint8_t val) {
  __asm__ volatile ("outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
}

inline static uint16_t inw(uint16_t port) {
  uint16_t ret;
  __asm__ volatile ("inw %w1, %w0" : "=a"(ret) : "Nd"(port) : "memory");
  return ret;
}

inline static void outw(uint16_t port, uint16_t val) {
  __asm__ volatile ("outw %w0, %w1" : : "a"(val), "Nd"(port) : "memory");
}

inline static uint32_t inl(uint16_t port) {
  uint32_t ret;
  __asm__ volatile ("inl %w1, %0" : "=a"(ret) : "Nd"(port) : "memory");
  return ret;
}

inline static void outl(uint16_t port, uint32_t val) {
  __asm__ volatile ("outl %0, %w1" : : "a"(val), "Nd"(port) : "memory");
}

inline static void lgdt(void* base, uint16_t limit) {
  struct {
    uint16_t limit;
    void* base;
  }__attribute__((packed)) gdtr = { limit, base };
  __asm__ volatile ("lgdt %0" : : "m"(gdtr));
}

inline static void lidt(void* base, uint16_t limit) {
  struct {
    uint16_t limit;
    void* base;
  }__attribute__((packed)) idtr = { limit, base };
  __asm__ volatile ("lidt %0" : : "m"(idtr));
}

inline static size_t rdcr0() {
  size_t ret;
  __asm__ volatile ("mov %%cr0, %0" : "=r"(ret));
  return ret;
}

inline static void wrcr0(size_t val) {
  __asm__ volatile ("mov %0, %%cr0" : : "r"(val));
}

inline static size_t rdcr2() {
  size_t ret;
  __asm__ volatile ("mov %%cr2, %0" : "=r"(ret));
  return ret;
}

inline static size_t rdcr3() {
  size_t ret;
  __asm__ volatile ("mov %%cr3, %0" : "=r"(ret));
  return ret;
}

inline static void wrcr3(size_t val) {
  __asm__ volatile ("mov %0, %%cr3" : : "r"(val));
}

inline static size_t rdcr4() {
  size_t ret;
  __asm__ volatile ("mov %%cr4, %0" : "=r"(ret));
  return ret;
}

inline static void wrcr4(size_t val) {
  __asm__ volatile ("mov %0, %%cr4" : : "r"(val));
}

inline static void cli() {
  __asm__ volatile ("cli");
}

inline static void sti() {
  __asm__ volatile ("sti");
}

struct interrupt_frame {
  size_t ip;
  size_t cs;
  size_t flags;
  size_t sp; // user to kernel only 
  size_t ss; // user to kernel only
};

struct idt_entry {
  uint16_t offset_low;
  uint16_t selector;
  uint8_t reserved;
  uint8_t attributes;
  uint16_t offset_high;
}__attribute__((packed));

struct gdt_entry {
  uint16_t limit_low;
  uint16_t base_low;
  uint8_t base_mid;
  uint8_t access_byte;
  uint8_t limit_high;
  uint8_t base_high;
}__attribute__((packed));

#endif

