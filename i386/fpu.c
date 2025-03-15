#include <fpu.h>

#include <cpu.h>
#include <idt.h>

void fpu_exception_handler(struct isr_frame* frame) {
  /* TODO: not implemented */
}

bool init_fpu(void) {
  int no_fpu = 0x5A5A;
  wrcr0((rdcr0() & ~(size_t)0b1100) | 0b10);
  asm volatile ("fninit");
  asm volatile ("fnstsw %0" : "=m"(no_fpu) : : "memory");
  if (no_fpu) wrcr0((rdcr0() & ~(size_t)0b10) | 0b100);
  else set_cpu_isr(16, fpu_exception_handler);
  return !no_fpu;
}

void save_fpu(struct fpu_state* state) {
  asm volatile ("fnsave %0" : "=m"(*state) : : "memory");
}

void load_fpu(struct fpu_state* state) {
  asm volatile ("frstor %0" : : "m"(*state) : "memory");
}
