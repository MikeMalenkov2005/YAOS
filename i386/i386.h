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

inline static void ltr(uint16_t selector) {
  __asm__ volatile ("ltr %w0" : : "r"(selector));
}

extern void lcs(uint16_t cs);

inline static void lds(uint16_t ds) {
  __asm__ volatile ("mov %w0, %%ds" : : "a"(ds));
}

inline static void les(uint16_t es) {
  __asm__ volatile ("mov %w0, %%es" : : "a"(es));
}

inline static void lfs(uint16_t fs) {
  __asm__ volatile ("mov %w0, %%fs" : : "a"(fs));
}

inline static void lgs(uint16_t gs) {
  __asm__ volatile ("mov %w0, %%gs" : : "a"(gs));
}

#define cli __asm__ volatile ("cli")
#define sti __asm__ volatile ("sti")

struct interrupt_frame {
  size_t ip;
  size_t cs;
  size_t flags;
  size_t sp; // user to kernel only 
  size_t ss; // user to kernel only
};

struct registers {
  size_t gs;
  size_t fs;
  size_t es;
  size_t ds;
  size_t di;
  size_t si;
  size_t bp;
  size_t sp; // ignored by popa
  size_t bx;
  size_t dx;
  size_t cx;
  size_t ax;
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

#define KCODE 0x08
#define KDATA 0x10
#define UCODE 0x18
#define UDATA 0x20

struct tss {
  size_t link;
  size_t sp0;
  size_t ss0;
  size_t sp1;
  size_t ss1;
  size_t sp2;
  size_t ss2;
  size_t sp3;
  size_t ss3;
  size_t ip;
  size_t flags;
  size_t ax;
  size_t cx;
  size_t dx;
  size_t bx;
  size_t sp;
  size_t bp;
  size_t si;
  size_t di;
  size_t es;
  size_t cs;
  size_t ss;
  size_t ds;
  size_t fs;
  size_t gs;
  size_t ldtr;
  size_t tflag;
  size_t iopb;
  size_t ssp;
};

#endif

