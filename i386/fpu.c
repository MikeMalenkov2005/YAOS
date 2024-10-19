#include <fpu.h>

char fpu_is_emulated;

struct fpu_state emulated_fpu_state __attribute__((aligned(16)));

void init_fpu(void) {
  fpu_is_emulated = 1;
}

void save_fpu(struct fpu_state* state) {
  if (fpu_is_emulated) {
    *state = emulated_fpu_state;
  }
}

void load_fpu(struct fpu_state* state) {
  if (fpu_is_emulated) {
    emulated_fpu_state = *state;
  }
}
