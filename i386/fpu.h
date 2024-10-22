#ifndef FPU_H
#define FPU_H

#include <stdint.h>

#include <attributes.h>

struct fpu_float {
  uint64_t significand;
  uint16_t exponent;
}__packed;

struct fpu_state {
  uint32_t control_word;
  uint32_t status_word;
  uint32_t tag_word;
  uint32_t instruction_pointer;
  uint16_t opcode;
  uint16_t instruction_segment;
  uint32_t operand_pointer;
  uint32_t operand_segment;
  struct fpu_float register_stack[8];
}__packed;

int init_fpu(void);

void save_fpu(struct fpu_state* state);
void load_fpu(struct fpu_state* state);

#endif
