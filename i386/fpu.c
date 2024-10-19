#include <fpu.h>

#include <cpu.h>

int fpu_exists;

struct fpu_state emulated_fpu_state __attribute__((aligned(16)));

int init_fpu(void) {
  emulated_fpu_state.status_word = 0x5A5A;
  wrcr0((rdcr0() & ~(size_t)0b1100) | 0b10);
  asm volatile ("fninit");
  asm volatile ("fnstsw %0" : "=m"(emulated_fpu_state.status_word) : : "memory");
  if (emulated_fpu_state.status_word) {
    fpu_exists = 0;
    wrcr0((rdcr0() & ~(size_t)0b10) | 0b100);
  }
  else fpu_exists = 1;
  return fpu_exists;
}

void save_fpu(struct fpu_state* state) {
  if (fpu_exists) {
    asm volatile ("fnsave %0" : "=m"(*state) : : "memory");
  }
  else *state = emulated_fpu_state;
}

void load_fpu(struct fpu_state* state) {
  if (fpu_exists) {
    asm volatile ("frstor %0" : : "m"(*state) : "memory");
  }
  else emulated_fpu_state = *state;
}
