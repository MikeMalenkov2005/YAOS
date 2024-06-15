#include "kernel.h"

void kprint(const char *string, int color) {
  for (unsigned short *out = (unsigned short*)0xB8000; *string; ++out) {
    *out = *(string++) | (color << 8);
  }
}

int kcall(int fnid, int arg0, int arg1, int arg2, int arg3, int arg4, int arg5) {
  return 0;
}

void kerror(void *position, void *state, const char *message) {
  kprint(message, 4);
}

int kmain() {
  kprint("Yet Another Operating System!!!", 2);
  return 0;
}

