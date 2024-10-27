#ifndef SYSTEM_H
#define SYSTEM_H

#include <stddef.h>
#include <stdint.h>

struct saved_registers {
  size_t di;
  size_t si;
  size_t bp;
  size_t sp; // ignored
  size_t bx;
  size_t dx;
  size_t cx;
  size_t ax;
};

struct interrupt_frame {
  size_t ip;
  size_t cs;
  size_t flags;
  size_t sp; // only if cs is user code
  size_t ss; // only if cs is user code
};

inline static size_t rdcr0() {
  size_t ret;
  asm volatile ("mov %%cr0, %0" : "=r"(ret));
  return ret;
}

inline static void wrcr0(size_t val) {
  asm volatile ("mov %0, %%cr0" : : "r"(val));
}

inline static size_t rdcr2() {
  size_t ret;
  asm volatile ("mov %%cr2, %0" : "=r"(ret));
  return ret;
}

inline static size_t rdcr3() {
  size_t ret;
  asm volatile ("mov %%cr3, %0" : "=r"(ret));
  return ret;
}

inline static void wrcr3(size_t val) {
  asm volatile ("mov %0, %%cr3" : : "r"(val));
}

inline static size_t rdcr4() {
  size_t ret;
  asm volatile ("mov %%cr4, %0" : "=r"(ret));
  return ret;
}

inline static void wrcr4(size_t val) {
  asm volatile ("mov %0, %%cr4" : : "r"(val));
}

void lcs(uint16_t cs);

inline static void lds(uint16_t ds) {
  asm volatile ("mov %w0, %%ds" : : "a"(ds));
}

inline static void les(uint16_t es) {
  asm volatile ("mov %w0, %%es" : : "a"(es));
}

inline static void lfs(uint16_t fs) {
  asm volatile ("mov %w0, %%fs" : : "a"(fs));
}

inline static void lgs(uint16_t gs) {
  asm volatile ("mov %w0, %%gs" : : "a"(gs));
}

size_t rdflags();
void wrflags(size_t flags);

#define cli() asm volatile ("cli")
#define sti() asm volatile ("sti")

#endif
