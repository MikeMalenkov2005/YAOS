#ifndef FPU_H
#define FPU_H

#include <stdint.h>

inline static void fldcw(uint16_t cw) {
  asm volatile ("fldcw %0" : : "m"(cw));
}

struct fpu_state {
  double st[8];
};

void init_fpu(void);

void save_fpu(struct fpu_state* state);
void load_fpu(struct fpu_state* state);

#endif
